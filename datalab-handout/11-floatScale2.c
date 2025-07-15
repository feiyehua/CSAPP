/*
 * @Author       : FeiYehua
 * @Date         : 2025-07-15 21:54:48
 * @LastEditTime : 2025-07-15 22:25:28
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : 11-floatScale2.c
 *      © 2024 FeiYehua
 */
// float
/*
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf)
{
    unsigned magicNumberExp = 0x7F800000u; // Extract exp digits from uf
    unsigned magicNumberFrac = 0x7FFFFFu;
    unsigned exp = magicNumberExp & uf;
    unsigned frac;
    if (exp == magicNumberExp)
    {
        return uf;
    }
    else if (exp == 0) // Denormalized case
    {
        frac = uf & magicNumberFrac;
        frac = (frac << 1); // This includes the change in frac and exp
        uf &= (1 << 31);
        uf |= frac;
    }
    else
    {
        exp >>= 23;
        exp++;
        exp <<= 23;
        uf &= ~magicNumberExp;
        uf |= exp;
    }
    return uf;
}