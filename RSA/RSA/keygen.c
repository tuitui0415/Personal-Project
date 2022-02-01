#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gmp.h>

#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"

#define OPTIONS "b:i:n:d:s:vh"

int main(int argc, char **argv) {
    int opt = 0;
    uint64_t nbits = 256;
    uint64_t iters = 50;
    char *public = "rsa.pub";
    char *private = "rsa.priv";
    uint64_t seed = time(NULL);
    int no_input = true;
    int b_flag = false;
    int i_flag = false;
    int n_flag = false;
    int d_flag = false;
    int s_flag = false;
    int v_flag = false;
    int h_flag = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        no_input = false;
        switch (opt) {
        case 'b':
            b_flag = true;
            nbits = atoi(optarg);
            break;
        case 'i':
            i_flag = true;
            iters = atoi(optarg);
            break;
        case 'n':
            n_flag = true;
        public
            = optarg;
            break;
        case 'd':
            d_flag = true;
        private
            = optarg;
            break;
        case 's':
            s_flag = true;
            seed = atoi(optarg);
            break;
        case 'v': v_flag = true; break;
        case 'h': h_flag = true; break;
        }
    }
    if (h_flag == true) {
        printf("keygenerator: Usage and synopsis\n");
        printf("-b: specifies the minimum bits needed for the public modulus n. \n");
        printf(
            "-i: specifies the number of Miller-Rabin iterations for testing primes (default:50\n");
        printf("-n pbfile: specifies the public key file (default: rsa.pub).\n");
        printf("-d pvfile: specifies the private key file (default: rsa.prv).\n");
        printf("-s: specifies the random seed for the random state initialization (default: the "
               "seconds since the Unix epoch, given by time(NULL).\n");
        printf("-v: enables verbose output\n");
        printf("-h: displays program synopsis and usa\n");
        return 0;
    }
    if (no_input == true) {
        printf("Keygenerator: Usage and synopsis\n");
        printf("-b: specifies the minimum bits needed for the public modulus n. \n");
        printf(
            "-i: specifies the number of Miller-Rabin iterations for testing primes (default:50\n");
        printf("-n pbfile: specifies the public key file (default: rsa.pub).\n");
        printf("-d pvfile: specifies the private key file (default: rsa.prv).\n");
        printf("-s: specifies the random seed for the random state initialization (default: the "
               "seconds since the Unix epoch, given by time(NULL).\n");
        printf("-v: enables verbose output\n");
        printf("-h: displays program synopsis and usa\n");
    }
    FILE *fp_pubc;
    FILE *fp_priv;
    int fd_priv;
    fp_pubc = fopen(public, "w");
    if (fp_pubc == NULL) {
        printf("open public file failed");
        randstate_clear();
        return 0;
    }
    fp_priv = fopen(private, "w");
    if (fp_pubc == NULL) {
        printf("open private file failed");
        randstate_clear();
        return 0;
    }
    /*
     *set the permission mode of private file
     */
    fd_priv = fileno(fp_priv);
    fchmod(fd_priv, 0600);
    randstate_init(seed);
    mpz_t p, q, n, e, d, s, sig;
    mpz_inits(p, q, n, e, d, s, sig, NULL);
    /*
     *make public key and private key,sign the username to the public key file and write two keys to two different files.
     */
    rsa_make_pub(p, q, n, e, nbits, iters);
    rsa_make_priv(d, e, p, q);
    char *username = getenv("USER");
    mpz_set_str(sig, username, 62);
    rsa_sign(s, sig, d, n);
    rsa_write_pub(n, e, s, username, fp_pubc);
    rsa_write_priv(n, d, fp_priv);
    if (v_flag == true) {
        printf("username=%s\n", username);
        gmp_printf("s:%d bits = %Zd\n", mpz_sizeinbase(n, 2), s);
        gmp_printf("p:%d bits = %Zd\n", mpz_sizeinbase(e, 2), p);
        gmp_printf("q:%d bits = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n:%d bits = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("e:%d bits = %Zd\n", mpz_sizeinbase(e, 2), e);
        gmp_printf("d:%d bits = %Zd\n", mpz_sizeinbase(q, 2), d);
    }
    fclose(fp_pubc);
    fclose(fp_priv);
    randstate_clear();
    mpz_clears(p, q, n, e, d, s, sig, NULL);
    return 0;
}
