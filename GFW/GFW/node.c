#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "node.h"

Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (n) {
        n->oldspeak = strdup(oldspeak);
        n->newspeak = NULL;
        if (newspeak != NULL) {
            n->newspeak = strdup(newspeak);
        }
        n->left = n->right = NULL;
    }
    return n;
}

void node_delete(Node **n) {
    if (*n) {
        free((*n)->oldspeak);
        if ((*n)->newspeak) {
            free((*n)->newspeak);
        }
        free(*n);
        *n = NULL;
    }
    return;
}

void node_print(Node *n) {
    if (n->oldspeak && n->newspeak) {
        printf("%s->%s\n", n->oldspeak, n->newspeak);
    } else if (n->oldspeak && n->newspeak == NULL) {
        printf("%s\n", n->oldspeak);
    }
    return;
}
/*
int main(){
    Node *hello= node_create("hello","NULL");
    node_print(hello);
    node_delete(&hello);
}
*/
