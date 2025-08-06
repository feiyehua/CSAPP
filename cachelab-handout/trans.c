/*
 * @Author       : FeiYehua
 * @Date         : 2013-01-22 23:53:18
 * @LastEditTime : 2025-08-06 21:49:11
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
                for (int k = 0; k < 8; k++)
                {
                    for (int l = 0; l < 4; l++)
                    {
                        // We first update the first 4 rows in B chunk
                        B[j * 8 + l][i * 8 + k] = A[i * 8 + k][j * 8 + l];
                    }
                }
                for (int k = 7; k >= 0; k--)
                {
                    for (int l = 4; l < 8; l++)
                    {
                        B[j * 8 + l][i * 8 + k] = A[i * 8 + k][j * 8 + l];
                    }
                }
            }
        }
        // If i == j, however, 4 same sets are used repeatedly, causing overwhelming conflict misses.
        for (int i = 0; i < 8; i++)
        {
            int tmp1, tmp2;
            for (int k = 0; k < 8; k++)
            {
                for (int l = 0; l < 4; l++)
                {
                    if (l == k)
                    {
                        tmp1 = A[i * 8 + k][i * 8 + l];
                        continue;
                    }
                    if (k - l == 4)
                    {
                        tmp2 = A[i * 8 + k][i * 8 + l];
                        continue;
                    }
                    // We first update the first 4 rows in B chunk
                    B[i * 8 + l][i * 8 + k] = A[i * 8 + k][i * 8 + l];
                }
                if (k < 4)
                    B[i * 8 + k][i * 8 + k] = tmp1;
                else
                    B[i * 8 + k - 4][i * 8 + k] = tmp2;
            }
            for (int k = 7; k >= 0; k--)
            {
                for (int l = 4; l < 8; l++)
                {
                    if (l == k)
                    {
                        tmp1 = A[i * 8 + k][i * 8 + l];
                        continue;
                    }
                    if (l - k == 4)
                    {
                        tmp2 = A[i * 8 + k][i * 8 + l];
                        continue;
                    }
                    B[i * 8 + l][i * 8 + k] = A[i * 8 + k][i * 8 + l];
                }
                if (k >= 4)
                    B[i * 8 + k][i * 8 + k] = tmp1;
                else
                    B[i * 8 + k + 4][i * 8 + k] = tmp2;
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
