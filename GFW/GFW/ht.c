#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "ht.h"
#include "salts.h"
#include "speck.h"

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};
/*
 * create a hashtable
 */
HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        ht->size = size;
        ht->salt[0] = SALT_HASHTABLE_LO;
        ht->salt[1] = SALT_HASHTABLE_HI;
        ht->trees = (Node **) calloc(ht->size, sizeof(Node *));
        if (!ht->trees) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}
/*
 *a destructor function of ht
 */
void ht_delete(HashTable **ht) {
    if (*ht && (*ht)->trees) {
        for (uint32_t i = 0; i < (*ht)->size; i++) {
            if ((*ht)->trees[i]) {
                bst_delete(&((*ht)->trees[i]));
            }
        }
        free((*ht)->trees);
        free(*ht);
        *ht = NULL;
    }
    return;
}
/*
 *return the size of ht
 */
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}
/*
 *find whether there is a oldspeak in ht.
 */
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    uint32_t index = hash(ht->salt, oldspeak) % ht->size;
    return bst_find(ht->trees[index], oldspeak);
}
/*
 *insert a node into ht 
 */
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    uint32_t index = hash(ht->salt, oldspeak) % ht->size;
    ht->trees[index] = bst_insert(ht->trees[index], oldspeak, newspeak);
    return;
}
/*
 *count how many trees(bst) in ht
 */
uint32_t ht_count(HashTable *ht) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < ht_size(ht); i++) {
        if (ht->trees[i] != NULL) {
            count++;
        }
    }
    return count;
}
/*
 *calculate the average size of bst
 */
double ht_avg_bst_size(HashTable *ht) {
    uint32_t sum = 0;
    for (uint32_t i = 0; i < ht_size(ht); i++) {
        sum = bst_size(ht->trees[i]);
    }
    double avg = sum / ht_count(ht);
    return avg;
}
/*
 *calculate average height of bst
 */
double ht_avg_bst_height(HashTable *ht) {
    uint32_t sum;
    for (uint32_t i = 0; i < ht_size(ht); i++) {
        sum = bst_height(ht->trees[i]);
    }
    double avg = sum / ht_count(ht);
    return avg;
}

void ht_print(HashTable *ht);
