#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <time.h>
#include "randstate.h"

/*
 * Using Euclid's algorithm to compute the greatest common divisor. 
 */
void gcd(mpz_t d, mpz_t a, mpz_t b) {
    mpz_t t, A, B;
    mpz_inits(t, A, B, NULL);
    mpz_set(A, a);
    mpz_set(B, b);
    while (mpz_cmp_ui(B, 0) != 0) {
        mpz_set(t, B);
        mpz_mod(B, A, B);
        mpz_set(A, t);
    }
    mpz_set(d, A);
    mpz_clears(t, A, B, NULL);
    return;
}
/*
 * Using extended Eucliden lgorithm to computing the multiplicative inverse.
 */
void mod_inverse(mpz_t i, mpz_t a, mpz_t n) {
    mpz_t r1, r2, t1, t2, q, temp;
    mpz_init_set(r1, n);
    mpz_init_set(r2, a);
    mpz_init_set_ui(t1, 0);
    mpz_init_set_ui(t2, 1);
    mpz_inits(q, temp, NULL);
    while (mpz_cmp_ui(r2, 0) != 0) {
        mpz_t temp_r1, temp_t1;
        mpz_inits(temp_r1, temp_t1, NULL);
        mpz_fdiv_q(q, r1, r2);
        mpz_set(temp_r1, r1);
        mpz_set(r1, r2);
        mpz_mul(temp, q, r2);
        mpz_sub(r2, temp_r1, temp);
        mpz_set(temp_t1, t1);
        mpz_set(t1, t2);
        mpz_mul(temp, q, t2);
        mpz_sub(t2, temp_t1, temp);
        mpz_clears(temp_r1, temp_t1, NULL);
    }
    if (mpz_cmp_ui(r1, 1) > 0) {
        mpz_set_ui(i, 0);
        //printf("hello\n");
        mpz_clears(r1, r2, t1, t2, q, temp, NULL);
        return;
    }
    if (mpz_cmp_ui(t1, 0) < 0) {
        mpz_add(t1, t1, n);
        mpz_set(i, t1);
        //printf("nihao\n");
        mpz_clears(r1, r2, t1, t2, q, temp, NULL);
        return;
    }
    mpz_set(i, t1);
    mpz_clears(r1, r2, t1, t2, q, temp, NULL);
    return;
}

/*
 * Fast modular exponentiation
 */
void pow_mod(mpz_t out, mpz_t base, mpz_t exponent, mpz_t modulus) {
    mpz_t v, p, temp, two, d, n;
    mpz_inits(temp, d, n, NULL);
    mpz_init_set_ui(v, 1);
    mpz_init_set_ui(two, 2);
    mpz_set(d, exponent);
    mpz_set(n, modulus);
    mpz_init_set(p, base);
    while (mpz_cmp_ui(d, 0) > 0) {
        if (mpz_odd_p(d) == 1) {
            mpz_mul(temp, v, p);
            mpz_mod(v, temp, n);
        }
        mpz_mul(temp, p, p);
        mpz_mod(p, temp, n);
        mpz_fdiv_q(d, d, two);
    }
    mpz_set(out, v);
    mpz_clears(v, p, temp, two, d, n, NULL);
    return;
}

/*
 *Miller-Rabin primality test, check whether n is a prime number
 */
bool is_prime(mpz_t n, uint64_t iters) {
    if (mpz_cmp_ui(n, 0) == 0) {
        return false;
    }
    if (mpz_cmp_ui(n, 1) == 0) {
        return false;
    }
    if (mpz_cmp_ui(n, 3) == 0) {
        return true;
    }
    mpz_t n_min_one, r;
    mpz_inits(n_min_one, r, NULL);
    mpz_sub_ui(n_min_one, n, 1);
    mp_bitcnt_t s = 2;
    while (mpz_divisible_2exp_p(n_min_one, s)) {
        s++;
    }
    s--;
    mpz_tdiv_q_2exp(r, n_min_one, s);
    mpz_t a, upper_bound, y, two;
    mpz_inits(a, upper_bound, y, two, NULL);
    mpz_set_ui(two, 2);
    for (uint64_t i = 0; i < iters; i++) {
        mpz_t n_temp, j, s_temp;
        mpz_sub_ui(upper_bound, n, 3);
        mpz_urandomm(a, state, upper_bound);
        mpz_add_ui(a, a, 2);
        pow_mod(y, a, r, n);
        mpz_init(n_temp);
        mpz_sub_ui(n_temp, n, 1);
        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, n_temp) != 0) {
            mpz_init_set_ui(j, 1);
            mpz_init(s_temp);
            mpz_set_ui(s_temp, (s - 1));
            while (mpz_cmp(j, s_temp) <= 0 && mpz_cmp(y, n_temp) != 0) { //s-1 shi fou ke xing
                pow_mod(y, y, two, n);
                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(y, j, a, n_temp, s_temp, two, n_min_one, r, upper_bound, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            if (mpz_cmp(y, n_temp) != 0) {
                mpz_clears(y, j, a, n_temp, s_temp, two, n_min_one, r, upper_bound, NULL);
                return false;
            }
            mpz_clears(j, s_temp, NULL);
        }
        mpz_clear(n_temp);
    }
    mpz_clears(y, a, two, n_min_one, r, upper_bound, NULL);
    return true;
}
/*
 *  Loop generate a random number untill we get a prime number, we should plus (2^n-1) before check whether it is a prime number.
 *  This is to make sure we can generate an at least bits number of bits long, and also not too big.
 */
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    do {
        mpz_urandomb(p, state, bits);
        mpz_t temp, two;
        mpz_inits(temp, two, NULL);
        mpz_set_ui(two, 2);
        mpz_pow_ui(temp, two, bits);
        mpz_sub_ui(temp, temp, 1);
        mpz_add(p, p, temp);
        mpz_clears(temp, two, NULL);
    } while (!is_prime(p, iters));
    return;
}
