/*
 * @Author       : FeiYehua
 * @Date         : 2025-07-15 11:33:22
 * @LastEditTime : 2025-07-15 11:34:35
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : 2-tmin.c
 *      © 2024 FeiYehua
 */
/*
 * tmin - return minimum two's complement integer
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void)
{

    // The highest bit is 1, and the rest are 0.
    return 1 << 31;
}