#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define EXP 665829
#define BASE 100000000000
#define BASELOG 11

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s exponent\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);
    uint64_t dsum = 0;

    uint64_t binom[EXP / BASELOG + 1] = {0};
    binom[0] = 1;
    int len = 1;

    for (int k = 0; k <= n; k++) {
        for (int i = 0; i < len; i++) {
            uint64_t tmp = binom[i];
            for (int j = 0; j < BASELOG; j++) {
                dsum += tmp % 10;
                tmp /= 10;
            }
        }

        for (int i = 0; i < len; i++)
            binom[i] *= 3 * (n - k); // assume 3 * (n - k) < 10^8

        for (int i = 0; i < len || binom[i] != 0; i++) {
            binom[i + 1] += binom[i] / BASE;
            binom[i] %= BASE;
            len += i >= len;
        }

        int div = k + 1;
        uint64_t rem = 0;
        bool allzero = 1;
        for (int i = len - 1; i >= 0; i--) {
            rem = rem * BASE + binom[i];
            binom[i] = rem / div;
            rem %= div;
            allzero &= binom[i] == 0;
            len -= allzero;
        }
    }

    printf("Digital sum of (3*10^%d+1)^%d: %ld\n", EXP, n, dsum);

    if ((dsum - n * 4) % 7 == 0)
        printf("(3*10^%d+1)^%d*10^%ld is Smith.\n", EXP, n, (dsum - n * 4) / 7);
    
    return EXIT_SUCCESS;
}
