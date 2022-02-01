#include <stdint.h>
#include <gmp.h>
#include "randstate.h"

gmp_randstate_t state; //declare the variable

/*
 * use seed to set the state
 */
void randstate_init(uint64_t seed) {
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);
}
/*
 * initialzie the state
 */
void randstate_clear(void) {
    gmp_randclear(state);
}
