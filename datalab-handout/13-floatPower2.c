/*
 * @Author       : FeiYehua
 * @Date         : 2025-07-16 10:12:37
 * @LastEditTime : 2025-07-16 10:34:55
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : 13-floatPower2.c
 *      © 2024 FeiYehua
 */
/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatPower2(int x)
{
    if (x < -126) // Denormalized cases
    {
        if (x <= -150)
        {
            return 0; // Too small
        }
        else
        {
            return 0x400000 >> (-x - 126);
        }
    }
    else if (x >= 128) // Overflow cases
    {
        return 0x7F800000u;
    }
    else // Can be represent normally
    {
        x += 127;
        x <<= 23;
        return x;
    }
}