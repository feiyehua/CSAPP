/*
 * @Author       : FeiYehua
 * @Date         : 2025-07-15 11:39:19
 * @LastEditTime : 2025-07-15 12:21:56
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : 3-isTmax.c
 *      © 2024 FeiYehua
 */
// 2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x)
{
    // The biggest two's complement number's highest bit is 0, and other bits are 1.
    // 2147483647 + 1 and -1 + 1 will both flip all the bits.
    int a = x + 1;
    int b = ~(a ^ x); // This equals to 0 if x == 2147483647 or -1, none 0 otherwise
    int c = (x + 1);  // This equals to 0 if x == -1, none 0 otherwise
    b = !b;           // This equals to 1 if x == 2147483647 or -1, 0 otherwise
    c = !c;           // This equals to 1 if x == -1, 0 otherwise
    return b ^ c;
}