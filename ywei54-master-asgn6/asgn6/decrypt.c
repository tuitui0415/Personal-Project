#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gmp.h>

#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"

#define OPTIONS "i:o:n:vh"

int main(int argc, char **argv) {
    int opt = 0;
    FILE *fp_in = stdin;
    FILE *fp_out = stdout;

    char *priv = "rsa.priv";
    int no_input = true;
    int i_flag = false;
    int o_flag = false;
    int n_flag = false;
    int v_flag = false;
    int h_flag = false;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        no_input = false;
        switch (opt) {
        case 'i':
            i_flag = true;
            fp_in = fopen(optarg, "r");
            break;
        case 'o':
            o_flag = true;
            fp_out = fopen(optarg, "w");
            break;
        case 'n':
            n_flag = true;
            priv = optarg;
            break;
        case 'v': v_flag = true; break;
        case 'h': h_flag = true; break;
        }
    }
    if (h_flag == true) {
        printf("Encryptor: Usage and synopsis\n");
        printf("-i: specifies the input file to encrypt (default:stdin)\n");
        printf("-o: specifies the output file to encrypt (default:stdin)\n");
        printf("-n: specifies the file containing the public key(default: rsa.pub)\n");
        printf("-v: enables verbose output\n");
        printf("-h: displays program synopsis and usa\n");
        return 0;
    }
    if (no_input == true) {
        printf("Encryptor: Usage and synopsis\n");
        printf("-i: specifies the input file to encrypt (default:stdin)\n");
        printf("-o: specifies the output file to encrypt (default:stdin)\n");
        printf("-n: specifies the file containing the public key(default: rsa.pub)\n");
        printf("-v: enables verbose output\n");
        printf("-h: displays program synopsis and usa\n");
    }
    /*
     * open private file to get private key, use private key decrypt the encrypted file. 
     */
    FILE *fp_priv = fopen(priv, "r");
    mpz_t n, d;
    mpz_inits(n, d, NULL);
    rsa_read_priv(n, d, fp_priv);
    if (v_flag == true) {
        gmp_printf("n:%d bits = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("e:%d bits = %Zd\n", mpz_sizeinbase(d, 2), d);
    }
    rsa_decrypt_file(fp_in, fp_out, n, d);
    mpz_clears(n, d, NULL);
    fclose(fp_priv);
    fclose(fp_in);
    fclose(fp_out);
    return 0;
}
