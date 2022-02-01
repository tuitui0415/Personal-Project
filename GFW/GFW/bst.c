#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "bst.h"
//based on eugene's section and Lecture eighteen
Node *bst_create(void) {
    return NULL;
}

static uint32_t max(uint32_t x, uint32_t y) {
    return x > y ? x : y;
}
/*
 *get the longest node from root.
 */
uint32_t bst_height(Node *root) {
    if (root == NULL) {
        return 0;
    } else {
        return 1 + max(bst_height(root->left), bst_height(root->right));
    }
}
/*
 * similar to bst_height, walk through the tree, return the value of size.
 */
uint32_t bst_size(Node *root) {
    if (root == NULL) {
        return 0;
    } else {
        return 1 + bst_size(root->left) + bst_size(root->left);
    }
}
/*
 *compare each oldspeak using strcmp, return NULL if not find the node.
 */
Node *bst_find(Node *root, char *oldspeak) {
    if (root) {
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            return bst_find(root->left, oldspeak);
        } else if (strcmp(root->oldspeak, oldspeak) < 0) {
            return bst_find(root->right, oldspeak);
        }
    }
    return root;
}
/*
 *insert a node into bst, if there is no node, create a root node.
 */
Node *bst_insert(Node *root, char *oldspeak, char *newspeak) {
    if (root) {
        int a = strcmp(root->oldspeak, oldspeak);
        if (a > 0) {
            root->left = bst_insert(root->left, oldspeak, newspeak);
        }
        if (a < 0) {
            root->right = bst_insert(root->right, oldspeak, newspeak);
        }
        return root;
    }
    root = node_create(oldspeak, newspeak);
    return root;
}

void bst_print(Node *root) {
    if (root) {
        bst_print(root->left);
        node_print(root);
        bst_print(root->left);
    }
}
/*
 * a destructor funtion
 */
void bst_delete(Node **root) {
    if (*root) {
        bst_delete(&(*root)->left);
        bst_delete(&(*root)->right);
        node_delete(root);
    }
}

/*
int main(){
    Node *root = NULL;
    root=bst_insert(root, "hello", "nihao"); 
    //printf("What");
    root=bst_insert(root,">",NULL);
    root=bst_insert(root,"what","shenme");
    root=bst_insert(root,"dsadas",NULL);
    root=bst_insert(root,"sdad","sda");
    bst_print(root);
}
*/
