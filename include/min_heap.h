#ifndef MIN_HEAP_H
#define MIN_HEAP_H


#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint32_t *heap;
    uint32_t size;
    uint32_t capacity;
} MinHeap;

MinHeap* create_min_heap(uint32_t capacity) ;
void min_heap_push(MinHeap *heap, uint32_t value) ;
uint32_t min_heap_pop(MinHeap *heap) ;
#endif
