/*
 * @Author       : FeiYehua
 * @Date         : 2025-07-15 15:12:16
 * @LastEditTime : 2025-07-15 15:32:23
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : 7-conditional.c
 *      © 2024 FeiYehua
 */
/*
 * conditional - same as x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z)
{
    int condition = !x; // If x == 0, condition = 1; else condition = 0
    int result;
    condition = ~condition + 1; // If x == 0, condition = -1, else condition = 0;
    // If x == 0, then condition == -1, we should return z
    result = (condition & z) + ((~condition) & y);
    return result;
}