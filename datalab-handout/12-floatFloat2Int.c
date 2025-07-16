/*
 * @Author       : FeiYehua
 * @Date         : 2025-07-15 22:26:46
 * @LastEditTime : 2025-07-16 10:06:52
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : 12-floatFloat2Int.c
 *      © 2024 FeiYehua
 */
/*
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf)
{
    unsigned magicNumberExp = 0x7F800000u; // Extract exp digits from uf
    unsigned magicNumberFrac = 0x7FFFFFu;
    int exp = magicNumberExp & uf;
    unsigned frac = magicNumberFrac & uf;
    int sign = (!(uf & 0x80000000u)) == 0 ? -1 : 1;
    int result = 0;
    if (exp == magicNumberExp)
    {
        return 0x80000000u;
    }
    else
    {
        exp >>= 23;
        exp -= 127;
        // The nuance in denormalized cases are too small to affect the final result
        frac |= 0x800000u;
        frac <<= 8;
        if (exp <= -2)
        {
            return 0;
        }
        else if (exp >= 32)
        {
            return 0x80000000u;
        }
        while (exp >= 0)
        {
            result += !!(frac & 0x80000000u) << exp;
            frac <<= 1;
            exp--;
        }
        // Here exp == -1
        if (frac == 0x80000000u)
        {
            result += (result % 2);
        }
    }
    return result * sign;
}