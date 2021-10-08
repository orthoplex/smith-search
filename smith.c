#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

#define RANGE 30
#define BUFFER (3 * RANGE)

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s starting_exponent\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    const long EXP = atol(argv[1]);

    // Calculate starting point in base 10
    mpz_t n;
    mpz_init(n);
    mpz_ui_pow_ui(n, 101, EXP);
    long strsize = mpz_sizeinbase(n, 10) + BUFFER + 1;
    char *str = malloc(strsize);
    for (long i = 0; i < strsize; i++)
        str[i] = 0;
    mpz_get_str(str + BUFFER, 10, n);
    mpz_clear(n);
    for (long i = BUFFER; i < strsize - 1; i++)
        str[i] -= '0';

    for (long e = EXP; e < EXP + RANGE; e++) {
        // Calculate digital sum
        long sum = 0;
        for (long i = 0; i < strsize - 1; i++)
            sum += str[i];

        printf("Digital sum of 101^%ld: %ld\n", e, sum);
        if ((sum - 2 * e) % 7 == 0)
            printf("101^%ld*10^%ld is Smith.\n", e, (sum - 2 * e) / 7);

        // Multiply by 101 manually and carry
        for (long i = 0; i < strsize - 3; i++)
            str[i] += str[i + 2];
        for (long i = strsize - 1; i > 0; i--) {
            str[i - 1] += str[i] / 10;
            str[i] %= 10;
        }
    }

    free(str);
    return EXIT_SUCCESS;
}
