#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "stack.h"

struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};
Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s) {
        s->top = 0;
        s->capacity = capacity;
        s->items = (Node **) malloc(sizeof(Node *) * capacity);
        if (!s->items) {
            free(s);
            s = NULL;
        }
    }
    return s;
}

void stack_delete(Stack **s) {
    if (*s && (*s)->items) {
        free((*s)->items);
        free(*s);
        *s = NULL;
    }
    return;
}

bool stack_empty(Stack *s) {
    bool empty = (s->top == 0);
    return empty;
}

bool stack_full(Stack *s) {
    bool full = (s->top == s->capacity);
    return full;
}

uint32_t stack_size(Stack *s) {
    uint32_t size = s->top;
    return size;
}

bool stack_push(Stack *s, Node *n) {
    if (stack_full(s)) {
        return false;
    } else {
        s->items[s->top] = n;
        s->top += 1;
        return true;
    }
}

bool stack_pop(Stack *s, Node **n) {
    if (stack_empty(s)) {
        return false;
    } else {
        s->top -= 1;
        *n = s->items[s->top];
        return true;
    }
}
void stack_print(Stack *s);
