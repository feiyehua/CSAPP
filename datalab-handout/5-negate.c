/*
 * @Author       : FeiYehua
 * @Date         : 2025-07-15 12:58:31
 * @LastEditTime : 2025-07-15 13:01:26
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : 5-negate.c
 *      © 2024 FeiYehua
 */
/*
 * negate - return -x
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x)
{
    return ~x + 1;
}