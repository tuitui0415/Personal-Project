#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include <ctype.h>

#include "bf.h"
#include "bst.h"
#include "node.h"
#include "bv.h"
#include "ht.h"
#include "parser.h"
#include "messages.h"
#include "salts.h"
#include "speck.h"

#define OPTIONS "ht:f:s"
#define WORD    "[a-zA-Z0-9_'-]+" //a word(a-zA-Z0-9_) with (or without) ' or - with multiple word

int main(int argc, char **argv) {
    int opt = 0;
    uint32_t size_h = 65536;
    uint32_t size_b = 1048576;
    int h_flag = false;
    int s_flag = false;
    //uint64_t lookups=0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': h_flag = true; break;
        case 't': size_h = atoi(optarg); break;
        case 'f': size_b = atoi(optarg); break;
        case 's': s_flag = true; break;
        }
    }
    if (h_flag == true) {
        printf("banhammer: Usage and synopsis\n");
        printf("-h prints out the program usage. Refer to the reference program in the resources "
               "repository for what to print. \n");
        printf("-t size specifies that the hash table will have size entries (the default will be "
               "2^16 ).\n");
        printf("-f size specifies that the Bloom filter will have size entries (the default will "
               "be 2^20 ).\n");
        printf("-s will enable the printing of statistics to stdout. \n");
        return 0;
    }
    BloomFilter *bf = bf_create(size_b);
    HashTable *ht = ht_create(size_h);
    /*
     *open badspeak and newspeak, read each word from them.
     */
    FILE *fp_bad = fopen("badspeak.txt", "r");
    FILE *fp_new = fopen("newspeak.txt", "r");
    char badword[4096];
    int a = 0;

    while (a != -1) {
        a = fscanf(fp_bad, "%s", badword);
        bf_insert(bf, badword);
        ht_insert(ht, badword, NULL);
    }

    a = 0;
    char oldword[4096]; //array for words
    char newword[4096];
    while (a != -1) {
        a = fscanf(fp_new, "%s %s", oldword, newword);
        bf_insert(bf, oldword);
        ht_insert(ht, oldword, newword);
    }
    /*
	 *regex part, get the word from passage.
	 */
    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        return 1;
    }
    char *word = NULL;
    Node *root_havenew = bst_create();
    Node *root_nonew = bst_create();
    while ((word = next_word(stdin, &re)) != NULL) {
        /*
        for (int i = 0; i<4096; i++) {
            word[i] = tolower(word[i]);
        }
*/
        /*
	 *check whether the word is in hte bf and ht, do action when get the result
	 */
        if (bf_probe(bf, word)) {
            Node *check_word = ht_lookup(ht, word);
            if (check_word) {
                if (check_word->newspeak) {
                    root_havenew
                        = bst_insert(root_havenew, check_word->oldspeak, check_word->newspeak);
                } else {
                    root_nonew = bst_insert(root_nonew, check_word->oldspeak, NULL);
                }
            }
        }
    }

    if (bst_size(root_havenew) && bst_size(root_nonew)) {
        fprintf(stdout, "%s", mixspeak_message);
        bst_print(root_nonew);
        bst_print(root_havenew);
    }
    if (bst_size(root_havenew) == 0 && bst_size(root_nonew)) {
        fprintf(stdout, "%s", badspeak_message);
        bst_print(root_nonew);
    }
    if (bst_size(root_havenew) && bst_size(root_nonew) == 0) {
        fprintf(stdout, "%s", goodspeak_message);
        bst_print(root_havenew);
    }
    if (s_flag == true) {
        fprintf(stdout, "Average binary search tree size=%f\n", ht_avg_bst_size(ht));
        fprintf(stdout, "Average binary search tree height = %f\n", ht_avg_bst_height(ht));
        //fprintf(stdout,"Average branches traversed=%lu",branches/lookups);
        fprintf(stdout, "Hash table load=%.6u]n", 100 * (ht_count(ht) / ht_size(ht)));
        fprintf(stdout, "Bloom filter load =%.6u\n", 100 * (bf_count(bf) / bf_size(bf)));
    }
    /*
     *close file and free memeory
     */
    clear_words();
    regfree(&re);
    bf_delete(&bf);
    ht_delete(&ht);
    bst_delete(&root_havenew);
    bst_delete(&root_nonew);
    fclose(fp_bad);
    fclose(fp_new);
    return 0;
}
