/*
 * @Author       : FeiYehua
 * @Date         : 2025-07-15 12:22:53
 * @LastEditTime : 2025-07-15 12:45:58
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : 4-allOddBits.c
 *      © 2024 FeiYehua
 */
/*
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x)
{
    int magicNumber=(0xAA<<8)+0xAA; // equals to 1010 1010 1010 1010
    int result = magicNumber;
    result&=(x&magicNumber); // result == magicNumber if the lower 16 bits of x == 0xAAAA
    x>>=16;
    result&=(x&magicNumber); // result == magicNumber if the lower and upper 16 bits of x == 0xAAAA
    // Now, if all odd-numbered bits in x set to 1, result == 0xAAAA
    result^=magicNumber;
    return !result;
}