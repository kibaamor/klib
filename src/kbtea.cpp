#include "../include/kbtea.h"

namespace klib {

#define DELTA 0x9e3779b9
#define MX (((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4)) ^ ((sum ^ y) + (key[(p & 3) ^ e] ^ z)))
void btea(uint32_t* data, int32_t num, const uint32_t key[4])
{
    uint32_t y, z, sum;
    uint32_t rounds, e;
    int32_t p;
    if (num > 1) { /* Coding Part */
        rounds = 6 + 52 / num;
        sum = 0;
        z = data[num - 1];
        do {
            sum += DELTA;
            e = (sum >> 2) & 3;
            for (p = 0; p < num - 1; p++) {
                y = data[p + 1];
                z = data[p] += MX;
            }
            y = data[0];
            z = data[num - 1] += MX;
        } while (--rounds);
    } else if (num < -1) { /* Decoding Part */
        num = -num;
        rounds = 6 + 52 / num;
        sum = rounds * DELTA;
        y = data[0];
        do {
            e = (sum >> 2) & 3;
            for (p = num - 1; p > 0; p--) {
                z = data[p - 1];
                y = data[p] -= MX;
            }
            z = data[num - 1];
            y = data[0] -= MX;
            sum -= DELTA;
        } while (--rounds);
    }
}

} // namespace klib
