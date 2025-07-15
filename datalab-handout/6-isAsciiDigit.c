/*
 * @Author       : FeiYehua
 * @Date         : 2025-07-15 13:02:42
 * @LastEditTime : 2025-07-15 13:46:09
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : 6-isAsciiDigit.c
 *      © 2024 FeiYehua
 */
// 3
/*
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x)
{
    // 0x30 == 11 0000, 0x39 == 11 1001
    int smallestBias = ~(47); // evaluate to -48
    int biggestBias = ~(57);  // evaluate to -58
    int smallestInt = (1 << 31);
    int result;
    int sign;
    int l = x + biggestBias;
    int r = x + smallestBias;
    result = (l ^ r) & smallestInt; // The highest bit is 1 if l < 0 and r >= 0
    // The sign also changes when x is small snough.
    // Exclude this situation by judging wether x is smaller than 0.
    sign = !(x & smallestInt); // 1 if x >= 0
    return (!!result) & sign;
}