#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bf.h"
#include "salts.h"
#include "speck.h"

struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};
/*
 *get the salt for hash value, create a bf using bv_create().
 */
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {
        bf->primary[0] = SALT_PRIMARY_LO;
        bf->primary[1] = SALT_PRIMARY_HI;
        bf->secondary[0] = SALT_SECONDARY_LO;
        bf->secondary[1] = SALT_TERTIARY_HI;
        bf->tertiary[0] = SALT_TERTIARY_LO;
        bf->tertiary[1] = SALT_TERTIARY_HI;
        bf->filter = bv_create(size);
    }
    return bf;
}
/*
 *destructor for bf
 */
void bf_delete(BloomFilter **bf) {
    if (bf && *bf) {
        bv_delete(&((*bf)->filter));
        free(*bf);
        *bf = NULL;
    }
    return;
}
/*
 *get the length of the bit as the bf size
 */
uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}
/*
 *set three diffrent bits, using hash()calculate the hashvalue as the index.
 */
void bf_insert(BloomFilter *bf, char *oldspeak) {
    uint32_t p = hash(bf->primary, oldspeak);
    p = p % bf_size(bf);
    bv_set_bit(bf->filter, p);

    uint32_t s = hash(bf->secondary, oldspeak);
    s = s % bf_size(bf);
    bv_set_bit(bf->filter, s);

    uint32_t t = hash(bf->tertiary, oldspeak);
    t = t % bf_size(bf);
    bv_set_bit(bf->filter, t);
    return;
}
/*
 *similar as bf_insert, use get_bit prove the position have that bit.
 */
bool bf_probe(BloomFilter *bf, char *oldspeak) {
    bool a;
    uint32_t p = hash(bf->primary, oldspeak);
    p = p % bf_size(bf);
    a = bv_get_bit(bf->filter, p);

    uint32_t s = hash(bf->secondary, oldspeak);
    s = s % bf_size(bf);
    a = bv_get_bit(bf->filter, s);

    uint32_t t = hash(bf->tertiary, oldspeak);
    t = t % bf_size(bf);
    a = bv_get_bit(bf->filter, t);

    return a;
}
/*
 *use get_bit check the bits have been set to one in total.
 */
uint32_t bf_count(BloomFilter *bf) {
    uint32_t a = bf_size(bf);
    uint32_t count = 0;
    for (uint32_t i = 0; i < a; i++) {
        if (bv_get_bit(bf->filter, i)) {
            count += 1;
        }
    }
    return count;
}

void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
}
/*
int main(){
    
    BloomFilter *bf= bf_create(1000);
    bf_insert(bf,"hello");
    bf_insert(bf,"wtf");
    bf_insert(bf,"okok");
    //bf_insert(bf,"goodbye");
    //uint32_t a= bf_count(bf);
    //printf("%d\n",bf_count);
    bf_print(bf);
}
*/
