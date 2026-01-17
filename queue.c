#include "queue.h"
#include "print.h"

void *init_queue(size_t element_size, unsigned int initial_cap) {
    queue *q = malloc(sizeof(queue));
    if(!q) {
	return NULL;
    }

    q->capacity = initial_cap;
    q->size = 0;
    q->element_size = element_size;

    q->data = malloc(element_size * initial_cap);
    if(!q->data) {
	printf("Failed to allocate data\n");
	free(q);
	return NULL;
    }

    return q;
}

int enqueue(queue *q, void *item) {
    if(q->capacity == q->size) {
	unsigned int new_cap = q->capacity * 2;

	void *new_data = realloc(q->data, new_cap * q->element_size);
	if(!new_data) {
	    return -1;
	}

	q->capacity = new_cap;
	q->data = new_data;
    }
    memcpy((char*)q->data + q->size * q->element_size, item, q->element_size);
    q->size++;
    return 0;
}

void pop(queue *q, void *out_item) {
    if(q->size == 0) {
	return;
    }

    memcpy(out_item, q->data, q->element_size);
    memmove(q->data, (char*) q->data + q->element_size, (q->size -1) * q->element_size);

    q->size--;
}

int empty(queue *q) {
    if(q->size > 0)
	return 0;
    else
	return 1;
}
