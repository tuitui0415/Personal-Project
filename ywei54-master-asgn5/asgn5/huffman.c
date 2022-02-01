#include "node.h"
#include "code.h"
#include "pq.h"
#include "defines.h"
#include "stack.h"
#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

Node *build_tree(uint64_t hist[static ALPHABET]) {

    int count = 0;
    for (int i = 0; i < 256; i++) {
        if (hist[i] != 0) {
            count++;
        }
    } //get the number of different symbols
    PriorityQueue *q = NULL;
    q = pq_create(count);
    for (int i = 0; i < 256; i++) {
        if (hist[i] != 0) {
            Node *n = (Node *) malloc(sizeof(Node));
            memset(n, 0, sizeof(Node));
            n = node_create(i, hist[i]);
            enqueue(q, n);
        }
    }

    while (pq_size(q) != 1) {
        Node *n = (Node *) malloc(sizeof(Node));
        dequeue(q, &n->left);
        dequeue(q, &n->right);
        n = node_join(n->left, n->right);
        enqueue(q, n);
    } //dequeue the 2 smallest node
    Node *dian;
    dequeue(q, &dian);
    return dian; // get the root node
}

void build(Node *root, Code table[static ALPHABET], Code c) {
    uint8_t *bit = (uint8_t *) malloc(sizeof(uint8_t));
    ;

    if (root->left == 0 && root->right == 0) {
        table[root->symbol] = c;
    } else {
        code_push_bit(&c, 0);
        build(root->left, table, c);
        code_pop_bit(&c, bit);
        code_push_bit(&c, 1);
        build(root->right, table, c);
        code_pop_bit(&c, bit);
    } //use this made function to recurse the build code
}
void build_codes(Node *root, Code table[static ALPHABET]) {
    Code c = code_init();
    build(root, table, c); //use self-made function
}

void dump_tree(int outfile, Node *root) {
    if (root != 0) {
        dump_tree(outfile, root->left);
        dump_tree(outfile, root->right);
        //do recurse when meet different situation
        if (root->left == 0 && root->right == 0) {
            char LL = { 'L' };
            write(outfile, &LL, 1);
            write(outfile, &(root->symbol), 1);
        } else {
            write(outfile, "I", 1);
        } //do different action when meet 'i' & 'L'
    }
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
    /*
	 * use dumped tree rebuild huffman tree
	 */
    Stack *s = stack_create(nbytes);
    for (int i = 0; i < nbytes; i++) {
        if (tree[i] == 'L') {
            Node *n = node_create(tree[i + 1], 0);
            stack_push(s, n);

        } else if (tree[i] == 'I') {
            Node *right = node_create('$', 0);
            Node *left = node_create('$', 0);
            stack_pop(s, &right);
            stack_pop(s, &left);
            Node *sum = node_join(left, right);
            stack_push(s, sum);
        }
    }
    Node *root = node_create('$', 0);
    stack_pop(s, &root);
    return root;
}

void delete_tree(Node **root) {
    if (*root) {
        free(*root);
        *root = NULL;
    }
    return;
}
