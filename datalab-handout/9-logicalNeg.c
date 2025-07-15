/*
 * @Author       : FeiYehua
 * @Date         : 2025-07-15 15:58:13
 * @LastEditTime : 2025-07-15 17:03:55
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : 9-logicalNeg.c
 *      © 2024 FeiYehua
 */
// 4
/*
 * logicalNeg - implement the ! operator, using all of
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int logicalNeg(int x)
{
    int a = (x << 16) | x;
    int b = (a << 8) | a;
    int c = (b << 4) | b;
    int d = (c << 2) | c;
    int e = (d << 1) | d; // Put the logic OR result to the most significant bit of e
    int f = (e >> 31) + 1;
    return f;
}