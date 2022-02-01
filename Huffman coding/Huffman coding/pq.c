#include "node.h"
#include "pq.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

struct PriorityQueue {
    //uint32_t head;
    //uint32_t tail;
    uint32_t size;
    uint32_t capacity;
    Node **items;
};

PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *q = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (q) {
        //q->head = 0 ;
        q->capacity = capacity;
        q->size = 0;
        q->items = (Node **) malloc(sizeof(Node *) * capacity);
        if (!q->items) {
            free(q);
            q = NULL;
        }
    }
    return q;
}
void pq_delete(PriorityQueue **q) {
    if (*q && (*q)->items) {
        free((*q)->items);
        free(*q);
        *q = NULL;
    }
    return;
}

bool pq_empty(PriorityQueue *q) {
    return q->size <= 0;
}

bool pq_full(PriorityQueue *q) {
    return q->size >= q->capacity;
}

uint32_t pq_size(PriorityQueue *q) {
    return q->size;
}
/* put a node into the queue
 * use insert sorting to sort the whole pq
 */
bool enqueue(PriorityQueue *q, Node *n) {
    if (pq_full(q)) {
        return false;
    }
    q->items[q->size] = n;

    Node *temp;
    if (q->size > 0) {
        //for (uint32_t i = q->size;i>0;i--){
        for (uint32_t i = q->size; i > 0; i--) {
            if (q->items[i]->frequency > q->items[i - 1]->frequency) {
                temp = q->items[i - 1];
                q->items[i - 1] = q->items[i];
                q->items[i] = temp;
            }
        }
    }
    q->size++;
    return true;
}
/* get a node out of the queue
 */
bool dequeue(PriorityQueue *q, Node **n) {
    if (pq_empty(q)) {
        return false;
    }
    //q->tail=q->size-1;
    q->size--;
    *n = q->items[q->size];

    return true;
}
void pq_print(PriorityQueue *q) {
    for (uint32_t i = 0; i < q->size; i++) {
        printf("symbol=%c,frequency=%ld <\n", q->items[i]->symbol, q->items[i]->frequency);
    }
}
