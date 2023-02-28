#include "randstate.h"
#include "ss.h"
#include "numtheory.h"
#include <stdlib.h>
#include <gmp.h>
#include <stdio.h>
#include <string.h>

void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {
    uint64_t pbits_min = nbits / 5;
    uint64_t pbits_max = (2 * nbits) / 5;
    uint64_t range = pbits_max - pbits_min + 1;

    uint64_t pbits = random() % range + pbits_min;
    uint64_t qbits = nbits - (pbits * 2);

    do {
        make_prime(p, pbits, iters);
        make_prime(q, qbits, iters);
        mpz_mul(n, p, p);
        mpz_mul(n, n, q);
    } while (mpz_get_ui(p) % mpz_get_ui(q) - 1 == 0 || mpz_get_ui(q) % mpz_get_ui(p) - 1 == 0
             || mpz_sizeinbase(n, 2) < nbits);
}

void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n%s\n", n, username);
}

void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n%s\n", n, username);
}

void ss_make_priv(mpz_t d, mpz_t pq, const mpz_t p, const mpz_t q) {
    mpz_t n, p_minus_one, q_minus_one, lcm;
    mpz_inits(n, p_minus_one, q_minus_one, lcm, NULL);

    mpz_mul(pq, p, q);
    mpz_mul(n, pq, p);
    mpz_sub_ui(p_minus_one, p, 1);
    mpz_sub_ui(q_minus_one, q, 1);

    gcd(lcm, p_minus_one, q_minus_one);
    mpz_fdiv_q(lcm, p_minus_one, lcm);
    mpz_mul(lcm, lcm, q_minus_one);

    mod_inverse(d, n, lcm);

    mpz_clears(n, p_minus_one, q_minus_one, lcm, NULL);
}

void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n%Zx\n", pq, d);
}

void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n%Zx\n", pq, d);
}

void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n) {
    pow_mod(c, m, n, n);
}

void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n) {
    mpz_t k, next_pow, sqrt_n, m;
    mpz_inits(k, next_pow, sqrt_n, m, NULL);

    mpz_sqrt(sqrt_n, n);
    mpz_set_ui(k, 0);

    do {
        mpz_add_ui(k, k, 1);
        mpz_ui_pow_ui(next_pow, 2, mpz_get_ui(k));
    } while (mpz_cmp(next_pow, sqrt_n) <= 0);

    mpz_sub_ui(k, k, 2);
    mpz_fdiv_q_ui(k, k, 8);

    uint8_t *block = malloc(mpz_get_ui(k) * sizeof(uint8_t));

    block[0] = 0xFF;

    size_t k_minus_one = mpz_get_ui(k) - 1;
    size_t j = 0;

    while ((j = fread(&block[1], sizeof(char), k_minus_one, infile)) > 0) {
        mpz_import(m, j + 1, 1, sizeof(char), 1, 0, block);

        ss_encrypt(m, m, n);

        gmp_fprintf(outfile, "%Zx\n", m);
    }

    free(block);

    mpz_clears(k, next_pow, sqrt_n, m, NULL);
}

void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq) {
    pow_mod(m, c, d, pq);
}

void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t pq, const mpz_t d) {
    mpz_t k, next_pow, c, m;
    mpz_inits(k, next_pow, c, m, NULL);

    mpz_set_ui(k, 0);

    do {
        mpz_add_ui(k, k, 1);
        mpz_ui_pow_ui(next_pow, 2, mpz_get_ui(k));
    } while (mpz_cmp(next_pow, pq) <= 0);

    mpz_sub_ui(k, k, 2);
    mpz_fdiv_q_ui(k, k, 8);

    uint8_t *block = malloc(mpz_get_ui(k) * sizeof(uint8_t));

    while (gmp_fscanf(infile, "%Zx\n", c) > 0) {
        ss_decrypt(m, c, d, pq);

        size_t j = 0;
        mpz_export(block, &j, 1, sizeof(char), 1, 0, m);

        fwrite(&block[1], sizeof(char), j - 1, outfile);
    }

    free(block);

    mpz_clears(k, next_pow, c, m, NULL);
}
