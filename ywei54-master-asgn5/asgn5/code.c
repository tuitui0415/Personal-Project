#include "defines.h"
#include "code.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
/*initial the code, create an array to store the bit for 
 *each symbol.
 */
Code code_init(void) {
    Code *cc = (Code *) malloc(sizeof(Code));
    cc->top = 0;
    for (int i = 0; i < 256 / 8; i++) {
        cc->bits[i] = 0;
    }
    return *cc;
}

/*return code size to get the sizeo of the code
 */
uint32_t code_size(Code *c) {
    return c->top;
}

/*
 * set the top of the code return true and false
 */
bool code_empty(Code *c) {
    if (c->top == 0) {
        return true;
    } else {
        return false;
    }
}

/*
 *if the pointer of the code is equal or more than 256
 * return true else return false
 */
bool code_full(Code *c) {
    if (c->top > 255) {
        return true;
    } else {
        return false;
    }
}

/*
 *set a byte with a specific bit in position, use "|" calculate the create byte with
 the origin byte, set a specific bit.
 */
bool code_set_bit(Code *c, uint32_t i) {
    if (i < 256) {
        c->bits[i / 8] |= (0x1 << i % 8);
        return true;
    } else
        return false;
}

/*
 *set a byte with a specific bit in position, use "&"and "not" calculate the create byte with
 the origin byte, set a specific bit.
 */

bool code_clr_bit(Code *c, uint32_t i) {
    if (i < 256) {
        c->bits[i / 8] &= ~(0x1 << i % 8);
        return true;
    } else
        return false;
}

/*
 *right move bit i, compare to 1, get the number of that bit.
 */
bool code_get_bit(Code *c, uint32_t i) {
    if (i < 256 && (((c->bits[i / 8] >> i % 8) & 0x1) == 1)) {
        return true;
    } else
        return false;
}

/*
 * push a 0 or 1 to the last bit.
 */
bool code_push_bit(Code *c, uint8_t bit) {
    if (code_full(c)) {
        return false;
    } else {
        if (bit == 1) {
            code_set_bit(c, c->top);
            //c->top++;
        }
        if (bit == 0) {
            code_clr_bit(c, c->top);
        }
        c->top++;
        return true;
    }
}
/*
 * pop the last bit to bit.
 */
bool code_pop_bit(Code *c, uint8_t *bit) {
    if (code_empty(c)) {
        return false;
    } else {
        c->top--;
        *bit = (code_get_bit(c, c->top));
        code_clr_bit(c, c->top);
        return true;
    }
}

/*void code_print(Code *c){
    for (int i=0;i<c->top;i++){
        if(code_get_bit==true){
            printf("%c", "1");
        else{
            printf("%c","0");
        }
    }
    printf("\n");
}
*/
