/*
 * @Author       : FeiYehua
 * @Date         : 2025-07-15 17:08:02
 * @LastEditTime : 2025-07-15 21:53:15
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : 10-howManyBits.c
 *      © 2024 FeiYehua
 */
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x)
{
    // We must have a highest bit, 0 or 1, depending on the sign of x.
    // With 1 bit, we can represent 0 and -1.
    // With 2 bit, we can represent -2 and 1.
    // We can find that x and ~x requires the same number of bits to represent.
    // We just need to count the number of continuous 0 (or 1, if x < 0) from its most significant bit.
    int a = 1 << 31;
    int b = !(a & x);     // 0 if x < 0, 1 if x >= 0
    int c = b + (~1) + 1; // -1 if x < 0, 0 if x >= 0
    int d = x ^ c;        // Revert x if x < 0
    // Let's divide x into 2 chunks: the first 16 bits and the last 16 bits.
    int e = 0xff;
    int f = ((e << 8) + e); // f = 0xffff
    int g = (d >> 16) ^ f;
    int h = !((g << 16) + f + 1); // 1 if the most significant 16 bits of d are 0s, 0 otherwise
    int count = ((~h) + 1) & 16;  // 16 if the most significant 16 bits of d are 0s, 0 otherwise
    d >>= (~count) + 17;          // Keep the least 16 bits

    count += !(d); // count = 1, if d == 0
    d >>= 1;
    count += !(d);
    d >>= 1;
    count += !(d);
    d >>= 1;
    count += !(d);
    d >>= 1;
    count += !(d);
    d >>= 1;
    count += !(d);
    d >>= 1;
    count += !(d);
    d >>= 1;
    count += !(d);
    d >>= 1;
    count += !(d);
    d >>= 1;
    count += !(d);
    d >>= 1;
    count += !(d);
    d >>= 1;
    count += !(d);
    d >>= 1;
    count += !(d);
    d >>= 1;
    count += !(d);
    d >>= 1;
    count += !(d);
    d >>= 1;
    count += !(d);

    // count == Continuous 0 from the most significant bit

    return 32 + (~count) + 2;
}