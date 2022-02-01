#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "node.h"
/*
 * create the node
 */
Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *n = (Node *) malloc(sizeof(Node));
    memset(n, 0, sizeof(Node));
    if (n) {

        n->symbol = symbol;
        n->frequency = frequency;
    }
    return n;
}
/*
 * delete the node
 */
void node_delete(Node **n) {
    if (*n) {
        free(*n);
        *n = NULL;
    }
    return;
}
/*join the node
 */
Node *node_join(Node *left, Node *right) {
    uint8_t sum_symbol = '$';
    uint64_t tempfrequency;

    tempfrequency = left->frequency + right->frequency;
    Node *sum = node_create(sum_symbol, tempfrequency);
    sum->left = left;
    sum->right = right;
    return sum;
}

void node_print(Node *n);
