/*
 * @Author       : FeiYehua
 * @Date         : 2025-07-14 22:53:27
 * @LastEditTime : 2025-07-15 10:58:16
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : bitXor.c
 *      © 2024 FeiYehua
 */
// 1
/*
 * bitXor - x^y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y)
{

    // (~x)&(~y) set all bits equal to 0 in both x and y to 1
    int a = (~x) & (~y);
    // x&y set all bits equal to 0 in both x and y to 1
    int b = x & y;
    // revert the two results a and b, and a&b equals to x^y.
    a = ~a;
    b = ~b;

    return a & b;
}