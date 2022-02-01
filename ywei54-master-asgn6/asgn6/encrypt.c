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

    char *pub = "rsa.pub";
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
            if (fp_in == NULL) {
                printf("open failed");
                return 0;
            }
            break;
        case 'o':
            o_flag = true;
            fp_out = fopen(optarg, "w");
            break;
        case 'n':
            n_flag = true;
            pub = optarg;
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
    FILE *fp_pub = fopen(pub, "r");
    if (fp_pub == NULL) {
        printf("open public failed");
        return 0;
    }
    mpz_t n, e, s, m;
    char username[128] = { 0 };
    mpz_inits(n, e, s, m, NULL);
    /*
     *get necessary message from public key file.
     */
    rsa_read_pub(n, e, s, username, fp_pub);
    if (v_flag == true) {
        printf("username=%s\n", username);
        gmp_printf("s:%d bits = %Zd\n", mpz_sizeinbase(n, 2), s);
        gmp_printf("n:%d bits = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("e:%d bits = %Zd\n", mpz_sizeinbase(e, 2), e);
    }
    /*
     *check whether the signature match s
     */
    mpz_set_str(m, username, 62);
    if (rsa_verify(m, s, e, n) == false) {
        mpz_clears(n, e, s, m, NULL);
        fclose(fp_pub);
        fclose(fp_in);
        fclose(fp_out);
        printf("sign verify fail");
        return 0;
    }

    rsa_encrypt_file(fp_in, fp_out, n, e);
    mpz_clears(n, e, s, m, NULL);
    fclose(fp_pub);
    fclose(fp_in);
    fclose(fp_out);

    return 0;
}
