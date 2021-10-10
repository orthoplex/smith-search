#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define EXP 879313
#define BASE 10000000000
#define HALFBASE 100000
#define BASELOG 10

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s exponent\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    uint8_t dsum_cache[HALFBASE] = {0};
    for (uint32_t i = 0; i < HALFBASE; i++)
        for (uint32_t tmp = i; tmp; tmp /= 10)
            dsum_cache[i] += tmp % 10;

    uint32_t n = atoi(argv[1]);
    uint64_t dsum = 0;

    uint64_t binom[EXP / BASELOG + 1] = {0};
    binom[0] = 1;
    uint32_t len = 1;

    for (uint32_t k = 0; k <= n; k++) {
        // update digital sum
        for (int i = 0; i < len; i++)
            dsum += dsum_cache[binom[i] % HALFBASE] + dsum_cache[binom[i] / HALFBASE];

        // multiply
        for (int i = 0; i < len; i++)
            binom[i] *= 6 * (n - k); // assume 3 * (n - k) < 10^8

        // carry
        for (int i = 0; i < len - 1; i++) {
            binom[i + 1] += binom[i] / BASE;
            binom[i] %= BASE;
        }
        if (binom[len - 1] > BASE) {
            binom[len] += binom[len - 1] / BASE;
            binom[len - 1] %= BASE;
            len++;
        }

        // divide
        uint32_t div = k + 1;
        uint64_t rem = 0;
        if (binom[len - 1] < div) {
            rem = binom[len - 1];
            binom[len - 1] = 0;
            len--;
        }
        for (int i = len - 1; i >= 0; i--) {
            rem = rem * BASE + binom[i];
            binom[i] = rem / div;
            rem %= div;
        }
    }

    printf("Digital sum of (6*10^%d+1)^%d: %ld\n", EXP, n, dsum);

    if ((dsum - n * 7) % 7 == 0)
        printf("(6*10^%d+1)^%d*10^%ld is Smith.\n", EXP, n, (dsum - n * 7) / 7);
    
    return EXIT_SUCCESS;
}
