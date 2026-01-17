#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    void *data;
    unsigned int capacity;
    unsigned int size;
    size_t element_size;
} queue;

void *init_queue(size_t element_size, unsigned int initial_cap);
int enqueue(queue *q, void *item);
void pop(queue *q, void *out_item);

#endif
