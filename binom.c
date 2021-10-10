#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s exponent\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int EXP = 665829;
    int n = atoi(argv[1]);
    char remstr[EXP + 1];
    long dsum = 0;
    mpz_t s, binom, mod, rem;
    mpz_inits(s, binom, mod, rem, NULL);
    mpz_set_ui(binom, 1);
    mpz_ui_pow_ui(mod, 10, EXP);

    for (int k = 0; k <= n; k++) {
        mpz_add(s, s, binom); 
        mpz_fdiv_qr(s, rem, s, mod);
        mpz_get_str(remstr, 10, rem);
        for (int i = 0; remstr[i]; i++)
            dsum += remstr[i] - '0';
        mpz_mul_ui(binom, binom, 3 * (n - k));
        mpz_fdiv_q_ui(binom, binom, k + 1);
    }

    while (mpz_sgn(s) != 0) {
        mpz_fdiv_qr_ui(s, rem, s, 10);
        dsum += mpz_get_ui(rem);
    }

    mpz_clears(s, binom, mod, rem, NULL);

    printf("Digital sum of (3*10^%d+1)^%d: %ld\n", EXP, n, dsum);

    if ((dsum - n * 4) % 7 == 0)
        printf("(3*10^%d+1)^%d*10^%ld is Smith.\n", EXP, n, (dsum - n * 4) / 7);
    
    return EXIT_SUCCESS;
}
