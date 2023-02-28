#include "numtheory.h"
#include "randstate.h"
#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

void gcd(mpz_t g, const mpz_t a, const mpz_t b) {
    mpz_t t, _b, _a;
    mpz_inits(t, _b, _a, NULL);

    mpz_set(_b, b);
    mpz_set(_a, a);

    while (mpz_cmp_ui(_b, 0) != 0) {
        mpz_set(t, _b);
        mpz_mod(_b, _a, _b);
        mpz_set(_a, t);
    }

    mpz_set(g, _a);

    mpz_clears(t, _b, _a, NULL);
}

void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n) {
    mpz_t r, _r, t, _t, q, temp;
    mpz_inits(r, _r, t, _t, q, temp, NULL);

    mpz_set(r, n);
    mpz_set(_r, a);
    mpz_set_ui(t, 0);
    mpz_set_ui(_t, 1);

    while (mpz_cmp_ui(_r, 0) != 0) {
        mpz_fdiv_q(q, r, _r);

        mpz_set(temp, r);
        mpz_set(r, _r);

        mpz_mul(_r, q, _r);
        mpz_sub(_r, temp, _r);

        mpz_set(temp, t);
        mpz_set(t, _t);

        mpz_mul(_t, q, _t);
        mpz_sub(_t, temp, _t);
    }

    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(o, 0);
        return;
    }

    if (mpz_cmp_si(t, 0) < 0) {
        mpz_add(t, t, n);
    }

    mpz_set(o, t);

    mpz_clears(r, _r, t, _t, q, temp, NULL);
}

void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n) {
    mpz_t v, p, d_;
    mpz_inits(v, p, d_, NULL);

    mpz_set_ui(v, 1);
    mpz_set(p, a);
    //mpz_mod(p, a, n);
    mpz_set(d_, d);

    while (mpz_cmp_ui(d_, 0) > 0) {
        if (mpz_get_ui(d_) % 2 != 0) {
            mpz_mul(v, v, p);
            mpz_mod(v, v, n);
        }
        mpz_mul(p, p, p);
        mpz_mod(p, p, n);

        mpz_fdiv_q_ui(d_, d_, 2);
    }

    mpz_set(o, v);

    mpz_clears(v, p, d_, NULL);
}

bool is_prime(const mpz_t n, uint64_t iters) {
    //write n-1 = (2^s)*r such that r is odd
    mpz_t r, s, a, y, i, j, two;
    mpz_inits(r, s, a, y, i, j, two, NULL);

    mpz_sub_ui(r, n, 1);
    mpz_set_ui(s, 0);
    mpz_set_ui(i, 1);
    mpz_set_ui(two, 2);

    while (mpz_get_ui(r) % 2 == 0) {
        mpz_fdiv_q_ui(r, r, 2);
        mpz_add_ui(s, s, 1);
    }

    while (mpz_cmp_ui(i, iters) <= 0) {
        mpz_sub_ui(a, n, 3);
        mpz_urandomm(a, state, a);
        mpz_add_ui(a, a, 2);

        pow_mod(y, a, r, n);

        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp_si(y, mpz_get_si(n) - 1) != 0) {
            mpz_set_ui(j, 1);
            while (mpz_cmp(j, s) < 0 && mpz_cmp_si(y, mpz_get_si(n) - 1) != 0) {
                pow_mod(y, y, two, n);
                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(r, s, a, y, i, j, two, NULL);
                    return false;
                }

                mpz_add_ui(j, j, 1);
            }

            if (mpz_get_si(y) != mpz_get_si(n) - 1) {
                mpz_clears(r, s, a, y, i, j, two, NULL);
                return false;
            }
        }

        mpz_add_ui(i, i, 1);
    }

    mpz_clears(r, s, a, y, i, j, two, NULL);

    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    do {
        mpz_urandomb(p, state, bits + 1);
    } while (!is_prime(p, iters) || mpz_sizeinbase(p, 2) <= bits);
}
