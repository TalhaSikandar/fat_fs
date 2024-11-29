#ifndef FAT_TABLE_H
#define FAT_TABLE_H

#include <stdint.h>
#include "../include/disk.h"
#include "../include/min_heap.h"

/*
* attributes: 0 0 0 0 0 0 0 0
  *                         | isReserved
  *                       | isFree
  *                     | isLast
*/



#define ATTRIBUTE_IS_RESERVED    (1 << 0)  // 0x01 (Reserved for future use)
#define ATTRIBUTE_IS_FREE        (1 << 1)  // 0x02 (Cluster is free)
#define ATTRIBUTE_IS_LAST        (1 << 2)  // 0x02 (Cluster is Last)

// checking each attribute
#define IS_RESERVED(attributes)  ((attributes) & ATTRIBUTE_IS_RESERVED)
#define IS_FREE(attributes)      ((attributes) & ATTRIBUTE_IS_FREE)
#define IS_LAST(attributes)      ((attributes) & ATTRIBUTE_IS_LAST)

// setting each attribute
#define SET_RESERVED(attributes) ((attributes) |= ATTRIBUTE_IS_RESERVED)
#define SET_FREE(attributes)     ((attributes) |= ATTRIBUTE_IS_FREE)
#define SET_LAST(attributes)     ((attributes) |= ATTRIBUTE_IS_LAST)

// clearing each attribute
#define CLEAR_RESERVED(attributes) ((attributes) &= ~ATTRIBUTE_IS_RESERVED)
#define CLEAR_FREE(attributes)     ((attributes) &= ~ATTRIBUTE_IS_FREE)
#define CLEAR_LAST(attributes)     ((attributes) &= ~ATTRIBUTE_IS_LAST)

typedef struct {
    uint32_t next_cluster; 
    uint8_t attributes;  // free/file_system...
} FATEntry; //attributes padded with 1 3 bytes making = 8 bytes

typedef struct {
    FATEntry *entries;  // FAT entries (one per cluster)
    uint32_t total_entries;  // total number of entries (clusters)
} FAT;


FAT* initialize_fat(Disk* disk, const char* image_file);
FAT *read_fat(Disk *disk, const char* image_file, MinHeap** min_heap);
void write_fat(FAT* fat, Disk* disk, const char* image_file);
void print_fat(FAT* fat, uint32_t num_entries_to_print);

void update_fat(FAT *fat, uint32_t cluster, uint32_t next_cluster);
uint32_t allocate_cluster(FAT *fat, MinHeap *heap);
void deallocate_cluster(FAT *fat, MinHeap *heap, uint32_t cluster);

char* read_clustor(const char* image_file, Disk *disk, uint32_t cluster_no, uint32_t *size);
void write_clustor(const char* image_file, Disk *disk, FAT *fat, MinHeap *heap, const char *data, uint32_t size);

void initialize_heap(MinHeap *heap, FAT *fat);

uint64_t calculate_cluster_offset(Disk *disk, uint32_t cluster);
#endif
