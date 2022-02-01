#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"
/*
 * generate two prime numbers p and q, their product n and e which is number coprime with fi n.
 */
void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    uint64_t p_bits = (rand() % (nbits / 2)) + (nbits / 4);
    uint64_t q_bits = nbits - p_bits;
    make_prime(p, p_bits, iters);
    make_prime(q, q_bits, iters);

    //mpz_set_ui(p,89);
    //mpz_set_ui(q,97);

    mpz_mul(n, p, q);
    mpz_t p_temp, q_temp, totient_n;
    mpz_inits(p_temp, q_temp, totient_n, NULL);
    mpz_sub_ui(p_temp, p, 1);
    mpz_sub_ui(q_temp, q, 1);
    mpz_mul(totient_n, p_temp, q_temp);

    //gmp_printf("totient in pub=%Zd\n",totient_n);

    mpz_t cd;
    mpz_init(cd);
    do {
        mpz_urandomb(e, state, nbits);
        gcd(cd, e, totient_n);
    } while (mpz_cmp_ui(cd, 1) != 0);
    mpz_clears(p_temp, q_temp, cd, totient_n, NULL);
    return;
}
/*
 * write e , n , signature , username to public key file.
 */
void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n);
    gmp_fprintf(pbfile, "%Zx\n", e);
    gmp_fprintf(pbfile, "%Zx\n", s);
    fprintf(pbfile, "%s\n", username);
    return;
}
/*
 *read n, e, signature ,username from public file.
 */
void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx", n);
    gmp_fscanf(pbfile, "%Zx", e);
    gmp_fscanf(pbfile, "%Zx", s);
    fscanf(pbfile, "%s", username);
    return;
}
/*
 * generate d using e, p,q
 */
void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    mpz_t p_temp, q_temp, totient;
    mpz_inits(p_temp, q_temp, totient, NULL);
    mpz_sub_ui(p_temp, p, 1);
    mpz_sub_ui(q_temp, q, 1);

    mpz_mul(totient, p_temp, q_temp);
    //gmp_printf("totient in pv=%Zd",totient);
    //mpz_set_ui(e,703);
    mod_inverse(d, e, totient);
    mpz_clears(p_temp, q_temp, totient, NULL);
    return;
}
/*
 * write n ,d, to private key file
 */
void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", n);
    gmp_fprintf(pvfile, "%Zx\n", d);
    return;
}
/*
 *read n,d, from private key file
 */
void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx", n);
    gmp_fscanf(pvfile, "%Zx", d);
    return;
}
/*
 *use pow_mode function encrypt data.
 */
void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    pow_mod(c, m, e, n);
    return;
}
/*
 * compute the block size, loop read data to block,use mpz_import convert to mpz_t, than encrypt the block
 */
void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    size_t k;
    mpz_t m;
    mpz_init(m);
    k = mpz_sizeinbase(n, 2);
    k = (k - 1) / 8;
    //k=k/8;
    uint8_t *buf = (uint8_t *) malloc(k);
    size_t j = 1;
    //printf("k=%zu\n",k);
    while (!feof(infile)) {
        buf[0] = 0Xff;
        mpz_t c;
        mpz_init(c);
        j = fread(&buf[1], 1, k - 1, infile);
        printf("j=%zu", j);
        //printf("j=%zu",j);
        j = j + 1;
        mpz_import(m, j, 1, sizeof(buf[0]), 1, 0, buf);
        //gmp_printf("m=%Zd",m);
        rsa_encrypt(c, m, e, n);
        gmp_fprintf(outfile, "%Zx\n", c);
        for (size_t i = 0; i < k; i++) {
            buf[i] = 0;
        }
        mpz_clear(c);
    }
    mpz_clears(m, NULL);
    free(buf);
    return;
}
/*
 *Use pow_mod decrypt encrypted data
 */
void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    pow_mod(m, c, d, n);
    return;
}
/*
 * compute the block size , loop scan data to block, use mpz_export convert c back to mpz_t, than decrypt the block.
 */
void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    size_t k;
    mpz_t m, c;
    mpz_inits(m, c, NULL);
    k = mpz_sizeinbase(n, 2);
    k = (k - 1) / 8;
    //k=k/8;
    uint8_t *buf = (uint8_t *) malloc(k);
    size_t j = k;
    //printf("k=%zu\n",k);
    while (j == k) {

        gmp_fscanf(infile, "%Zx", c);
        // gmp_printf("c=%Zd", c);
        rsa_decrypt(m, c, d, n);
        mpz_export(buf, &j, k, 1, 1, 0, m);
        fwrite(&buf[1], 1, j - 1, outfile);
        //printf("j=%zu", j);
    }
    mpz_clears(m, c, NULL);
    free(buf);
    return;
}
/*
 * use the same method as encrypt sign the signature
 */
void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    pow_mod(s, m, d, n);
    return;
}
/*
 * use s to compute t check whether t equal to the m to check the signature.
 */
bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    mpz_t temp;
    mpz_init(temp);
    pow_mod(temp, s, e, n);
    if (mpz_cmp(temp, m) == 0) {
        mpz_clears(temp);
        return true;
    } else
        mpz_clear(temp);
    return false;
}
