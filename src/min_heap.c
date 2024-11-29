#include "../include/min_heap.h"
MinHeap* create_min_heap(uint32_t capacity) {
    MinHeap *heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->heap = (uint32_t*)malloc(sizeof(uint32_t) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void min_heap_push(MinHeap *heap, uint32_t value) {
    if (heap->size >= heap->capacity) return;
    uint32_t i = heap->size++;
    while (i > 0) {
        uint32_t parent = (i - 1) / 2;
        if (heap->heap[parent] <= value) break;
        heap->heap[i] = heap->heap[parent];
        i = parent;
    }
    heap->heap[i] = value;
}

uint32_t min_heap_pop(MinHeap *heap) {
    if (heap->size == 0) return -1;
    uint32_t root = heap->heap[0];
    uint32_t last = heap->heap[--heap->size];
    uint32_t i = 0;
    while (i * 2 + 1 < heap->size) {
        uint32_t left = i * 2 + 1;
        uint32_t right = i * 2 + 2;
        uint32_t smallest = left;
        if (right < heap->size && heap->heap[right] < heap->heap[left]) smallest = right;
        if (last <= heap->heap[smallest]) break;
        heap->heap[i] = heap->heap[smallest];
        i = smallest;
    }
    heap->heap[i] = last;
    return root;
}
