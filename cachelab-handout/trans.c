/*
 * @Author       : FeiYehua
 * @Date         : 2013-01-22 23:53:18
 * @LastEditTime : 2025-08-07 19:20:43
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : trans.c
 *      © 2024 FeiYehua
 */
/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

typedef unsigned long long u_int64_t;

u_int64_t getSetIndex(int *t)
{
    return (((u_int64_t)(t)) >> 5) & ((1 << 5) - 1);
}

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */
// The cache block size is 1 << 5 = 32 byte (8 int in each), and have 32 sets in total.
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    if (M == 32 && N == 32)
    {
        int tmp;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                for (int k = 0; k < 8; k++)
                {
                    for (int l = 0; l < 8; l++)
                    {
                        if (k == l)
                        {
                            continue;
                        }
                        tmp = A[i * 8 + k][j * 8 + l];
                        B[j * 8 + l][i * 8 + k] = tmp;
                    }
                    // We can find that there is a 0x40000 byte gap between A and B.
                    // So, accessing elements on the diagonal causes a conflict miss.
                    // We can defer moving the diagonal elements.
                    tmp = A[i * 8 + k][j * 8 + k];
                    B[j * 8 + k][i * 8 + k] = tmp;
                }
            }
        }
    }
    else if (M == 64 && N == 64)
    {
        // In the case of M == 64 && N==64, things becomes more harsh.
        // Each row in B contains 256 byte, making a 1 0000 0000 (2) or 0x100 gap.
        // So at most 4(in row)*8(in column) ints can be cached.

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (i == j)
                {
                    continue;
                }
                for (int k = 0; k < 4; k++)
                {
                    for (int l = 0; l < 4; l++)
                    {
                        // We first update the first 4 rows in B chunk
                        B[j * 8 + l][i * 8 + k] = A[i * 8 + k][j * 8 + l];
                    }
                    // We can try to make full use of the cached elements of A array.
                    // We can move the last 4 elements in the firtst 4 rows in A to the last 4 columns in the first 4 rows in B array,
                    // Then move them to the correct places afterwards.
                    // In total, this takes us 18 misses to perform a 8*8 chunk transpose.
                    for (int l = 0; l < 4; l++)
                    {
                        // We put the data in B with a special order to prevent potential conflict misses when moving
                        if (l == 0)
                        {
                            B[j * 8 + 1][i * 8 + k + 4] = A[i * 8 + k][j * 8 + l + 4];
                        }
                        else if (l == 1)
                        {
                            B[j * 8 + 0][i * 8 + k + 4] = A[i * 8 + k][j * 8 + l + 4];
                        }
                        else if (l == 2)
                        {
                            B[j * 8 + 3][i * 8 + k + 4] = A[i * 8 + k][j * 8 + l + 4];
                        }
                        else
                        {
                            B[j * 8 + 2][i * 8 + k + 4] = A[i * 8 + k][j * 8 + l + 4];
                        }
                    }
                }
                for (int k = 4; k < 8; k++)
                {
                    // Put the 4th and 6th rows into cache, poping the 0th and 2nd row out
                    int l = 0;
                    B[j * 8 + l + 4][i * 8 + k - 4] = A[i * 8 + k][j * 8 + l + 1];
                    l = 2;
                    B[j * 8 + l + 4][i * 8 + k - 4] = A[i * 8 + k][j * 8 + l + 1];
                }
                for (int k = 0; k < 4; k++)
                {
                    int l = 0;
                    // Swap the 1st and 4th, 3rd and 6th rows
                    int tmp = 0;
                    tmp = B[j * 8 + l + 1][i * 8 + k + 4];
                    B[j * 8 + l + 1][i * 8 + k + 4] = B[j * 8 + l + 4][i * 8 + k];
                    B[j * 8 + l + 4][i * 8 + k] = tmp;
                    l = 2;
                    tmp = B[j * 8 + l + 1][i * 8 + k + 4];
                    B[j * 8 + l + 1][i * 8 + k + 4] = B[j * 8 + l + 4][i * 8 + k];
                    B[j * 8 + l + 4][i * 8 + k] = tmp;
                }
                for (int k = 4; k < 8; k++)
                {
                    // Put the 5th and 7th rows into cache, poping the 1th and 3rd row out
                    int l = 1;
                    B[j * 8 + l + 4][i * 8 + k - 4] = A[i * 8 + k][j * 8 + l - 1];
                    l = 3;
                    B[j * 8 + l + 4][i * 8 + k - 4] = A[i * 8 + k][j * 8 + l - 1];
                }
                for (int k = 4; k < 8; k++)
                {
                    for (int l = 4; l < 8; l++)
                    {
                        // Update the bottomleft elements in B
                        B[j * 8 + l][i * 8 + k] = A[i * 8 + k][j * 8 + l];
                    }
                }
                for (int k = 0; k < 4; k++)
                {
                    int l = 0;
                    // Swap the 0th and 5th, 2nd and 7th rows
                    int tmp = 0;
                    tmp = B[j * 8 + l][i * 8 + k + 4];
                    B[j * 8 + l][i * 8 + k + 4] = B[j * 8 + l + 5][i * 8 + k];
                    B[j * 8 + l + 5][i * 8 + k] = tmp;
                    l = 2;
                    tmp = B[j * 8 + l][i * 8 + k + 4];
                    B[j * 8 + l][i * 8 + k + 4] = B[j * 8 + l + 5][i * 8 + k];
                    B[j * 8 + l + 5][i * 8 + k] = tmp;
                }
            }
        }
        // If i == j, however, 4 same sets are used repeatedly, causing overwhelming conflict misses.
        for (int i = 0; i < 8; i++)
        {
            // Use the same update method as none-diagonal cases, but with special care dealing with the diagnonal elements
            int tmp1, tmp2;
            int j = i;
            for (int k = 0; k < 4; k++)
            {
                for (int l = 0; l < 4; l++)
                {
                    if (l == k)
                    {
                        tmp1 = A[i * 8 + k][j * 8 + l];
                        continue;
                    }
                    // We first update the first 4 rows in B chunk
                    B[j * 8 + l][i * 8 + k] = A[i * 8 + k][j * 8 + l];
                }

                for (int l = 0; l < 4; l++)
                {
                    if (k + l == 1 || k + l == 5)
                    {
                        // Conflict misses cases
                        tmp2 = A[i * 8 + k][j * 8 + l + 4];
                        continue;
                    }
                    // We put the data in B with a special order to prevent potential conflict misses when moving
                    if (l == 0)
                    {
                        B[j * 8 + 1][i * 8 + k + 4] = A[i * 8 + k][j * 8 + l + 4];
                    }
                    else if (l == 1)
                    {
                        B[j * 8 + 0][i * 8 + k + 4] = A[i * 8 + k][j * 8 + l + 4];
                    }
                    else if (l == 2)
                    {
                        B[j * 8 + 3][i * 8 + k + 4] = A[i * 8 + k][j * 8 + l + 4];
                    }
                    else
                    {
                        B[j * 8 + 2][i * 8 + k + 4] = A[i * 8 + k][j * 8 + l + 4];
                    }
                }
                B[j * 8 + k][i * 8 + k] = tmp1;
                B[j * 8 + k][i * 8 + k + 4] = tmp2;
            }

            for (int k = 4; k < 8; k++)
            {
                for (int l = 4; l < 8; l++)
                {
                    if (l == k)
                    {
                        tmp1 = A[i * 8 + k][j * 8 + l];
                        continue;
                    }
                    // We first update the first 4 rows in B chunk
                    B[j * 8 + l][i * 8 + k] = A[i * 8 + k][j * 8 + l];
                }

                for (int l = 0; l < 4; l++)
                {
                    if (k + l == 9 || k + l == 5)
                    {
                        // Conflict misses cases
                        tmp2 = A[i * 8 + k][j * 8 + l];
                        continue;
                    }
                    // We put the data in B with a special order to prevent potential conflict misses when moving
                    if (l == 0)
                    {
                        B[j * 8 + 5][i * 8 + k - 4] = A[i * 8 + k][j * 8 + l];
                    }
                    else if (l == 1)
                    {
                        B[j * 8 + 4][i * 8 + k - 4] = A[i * 8 + k][j * 8 + l];
                    }
                    else if (l == 2)
                    {
                        B[j * 8 + 7][i * 8 + k - 4] = A[i * 8 + k][j * 8 + l];
                    }
                    else
                    {
                        B[j * 8 + 6][i * 8 + k - 4] = A[i * 8 + k][j * 8 + l];
                    }
                }
                B[j * 8 + k][i * 8 + k] = tmp1;
                B[j * 8 + k][i * 8 + k - 4] = tmp2;
            }

            for (int k = 0; k < 4; k++)
            {
                int l = 0;
                // Swap the 1st and 4th, 3rd and 6th rows
                int tmp = 0;
                tmp = B[j * 8 + l + 1][i * 8 + k + 4];
                B[j * 8 + l + 1][i * 8 + k + 4] = B[j * 8 + l + 4][i * 8 + k];
                B[j * 8 + l + 4][i * 8 + k] = tmp;
                l = 2;
                tmp = B[j * 8 + l + 1][i * 8 + k + 4];
                B[j * 8 + l + 1][i * 8 + k + 4] = B[j * 8 + l + 4][i * 8 + k];
                B[j * 8 + l + 4][i * 8 + k] = tmp;
            }
            for (int k = 0; k < 4; k++)
            {
                int l = 0;
                // Swap the 0th and 5th, 2nd and 7th rows
                int tmp = 0;
                tmp = B[j * 8 + l][i * 8 + k + 4];
                B[j * 8 + l][i * 8 + k + 4] = B[j * 8 + l + 5][i * 8 + k];
                B[j * 8 + l + 5][i * 8 + k] = tmp;
                l = 2;
                tmp = B[j * 8 + l][i * 8 + k + 4];
                B[j * 8 + l][i * 8 + k + 4] = B[j * 8 + l + 5][i * 8 + k];
                B[j * 8 + l + 5][i * 8 + k] = tmp;
            }
        }
    }
    else
    {
        int tmp;
        for (int i = 0; i < (N / 16); i++)
        {
            for (int j = 0; j < (M / 16); j++)
            {
                for (int k = 0; k < 16; k++)
                {
                    int tl = -1;
                    int tl2 = -1;
                    int tmp2;
                    for (int l = 0; l < 16; l++)
                    {
                        if (getSetIndex(&B[j * 16 + l][i * 16 + k]) == getSetIndex(&A[i * 16 + k][j * 16 + l]))
                        {
                            if (tl == -1)
                            {
                                tl = l;
                                tmp = A[i * 16 + k][j * 16 + l];
                            }
                            else if (tl2 == -1)
                            {
                                tl2 = l;
                                tmp2 = A[i * 16 + k][j * 16 + l];
                            }
                            continue;
                        }
                        B[j * 16 + l][i * 16 + k] = A[i * 16 + k][j * 16 + l];
                    }
                    // We can find that there is a 0x40000 byte gap between A and B.
                    // So, accessing elements on the diagonal causes a conflict miss.
                    // We can defer moving the diagonal elements.
                    if (tl != -1)
                    {
                        B[j * 16 + tl][i * 16 + k] = tmp;
                    }
                    if (tl2 != -1)
                    {
                        B[j * 16 + tl2][i * 16 + k] = tmp2;
                    }
                }
            }
        }
        for (int i = (N / 16) * 16; i < N; i++)
        {
            for (int j = 0; j < M; j++)
            {
                tmp = A[i][j];
                B[j][i] = tmp;
            }
        }
        for (int i = 0; i < (N / 16) * 16; i++)
        {
            for (int j = (M / 16) * 16; j < M; j++)
            {
                tmp = A[i][j];
                B[j][i] = tmp;
            }
        }
    }
}

/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);
}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; ++j)
        {
            if (A[i][j] != B[j][i])
            {
                return 0;
            }
        }
    }
    return 1;
}
