#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "bv.h"
#define BITS_PER_UNIT 8
//based on professor long's work and the asgn5 code.c
struct BitVector {
    uint32_t length;
    uint8_t *vector;
};
/*
 *create a bit vector to manipulate each bit in bytes.
 */
BitVector *bv_create(uint32_t length) {
    BitVector *v = (BitVector *) malloc(sizeof(BitVector));
    if (v) {
        uint32_t bytes = length / BITS_PER_UNIT + (length % BITS_PER_UNIT ? 1 : 0);
        v->vector = (uint8_t *) calloc(bytes, sizeof(uint8_t));
        v->length = length;
        return v;
    } else {
        return (BitVector *) 0;
    }
}
/*
 *delete the bit vector
 */
void bv_delete(BitVector **bv) {
    if ((*bv) && (*bv)->vector) {
        free((*bv)->vector);
    }
    if ((*bv)) {
        free(*bv);
    }
    (*bv) = NULL;
    return;
}
/*
 *get the length of the bitvector
 */
uint32_t bv_length(BitVector *bv) {
    return bv ? bv->length : 0;
}
/*
 *set i bit to one
 */
bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (i >= bv->length) {
        return false;
    } else {
        bv->vector[i / BITS_PER_UNIT] |= (0x1 << i % BITS_PER_UNIT);
    }
    return true;
}
/*
 *set i bit to zero
 */
bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i >= bv->length) {
        return false;
    } else {
        bv->vector[i / BITS_PER_UNIT] &= ~(0x1 << i % BITS_PER_UNIT);
    }
    return true;
}
/*
 *get the i bit , check whether it is one or zero
 */
bool bv_get_bit(BitVector *bv, uint32_t i) {
    if (i >= bv->length) {
        return false;
    } else {
        return (bv->vector[i / BITS_PER_UNIT] >> i % BITS_PER_UNIT) & 0x1;
    }
}
void bv_print(BitVector *bv) {
    for (uint32_t i = 0; i < bv->length; i++) {
        uint8_t a = bv_get_bit(bv, i);
        printf("%d\n", a);
    }
    return;
}
/*
int main(){
    BitVector *bv =  bv_create(10);
    bv_length(bv);
    bv_set_bit(bv,4);
    if(bv_get_bit(bv,4)){
        printf("get success!\n");
    }
    if(bv_get_bit(bv,5)){
        printf("get success again!\n");
    }
    bv_print(bv);
    bv_delete(&bv);
}
*/
