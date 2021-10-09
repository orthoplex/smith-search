#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s exponent\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    const int EXP = 665829;
    const int N = atoi(argv[1]);

    mpz_t s, binom, rem;
    mpz_inits(s, binom, rem, NULL);
    mpz_set_ui(binom, 1);
    long dsum = 0;

    for (int k = 0; k <= N; k++) {
        mpz_add(s, s, binom); 
        
        for (int j = 0; j < EXP && mpz_sgn(s) != 0;) {
            if (j + 18 <= EXP) {
                mpz_fdiv_qr_ui(s, rem, s, 1000000000000000000);
                for (long lrem = mpz_get_ui(rem); lrem; lrem /= 10)
                    dsum += lrem % 10;
                j += 18;
            } else {
                mpz_fdiv_qr_ui(s, rem, s, 10);
                dsum += mpz_get_ui(rem);
                j += 1;
            }
        }

        mpz_mul_ui(binom, binom, 3 * (N - k));
        mpz_fdiv_q_ui(binom, binom, k + 1);
    }

    while (mpz_sgn(s) != 0) {
        mpz_fdiv_qr_ui(s, rem, s, 10);
        dsum += mpz_get_ui(rem);
    }

    mpz_clears(s, binom, rem, NULL);

    printf("Digital sum of (3*10^%d+1)^%d: %ld\n", EXP, N, dsum);

    if ((dsum - N * 4) % 7 == 0)
        printf("(3*10^%d+1)^%d*10^%ld is Smith.\n", EXP, N, (dsum - N * 4) / 7);
    
    return EXIT_SUCCESS;
}
