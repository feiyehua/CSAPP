/*
 * @Author       : FeiYehua
 * @Date         : 2025-07-15 15:33:05
 * @LastEditTime : 2025-07-15 15:55:31
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : 8-isLessOrEqual.c
 *      © 2024 FeiYehua
 */
/*
 * isLessOrEqual - if x <= y  then return 1, else return 0
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y)
{
    long long a = x;
    long long b = y;
    long long c = b + ~a + 1; // If y >= x, c >= 0, then we return 1.
    long long d = 1;
    long long magicNumber = d << 63;
    // If c >= 0, the most significant bit of ~c is 1, so result evaluate to a none 0 number.
    long long result = magicNumber & (~c);
    return !!result;
}