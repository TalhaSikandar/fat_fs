#ifndef FAT_TABLE_H
#define FAT_TABLE_H

#include <stdint.h>
#include "../include/disk.h"

/*
* attributes: 0 0 0 0 0 0 0 0
  *                         | isReserved
  *                       | isFree
*/



#define ATTRIBUTE_IS_RESERVED    (1 << 0)  // 0x01 (Reserved for future use)
#define ATTRIBUTE_IS_FREE        (1 << 1)  // 0x02 (Cluster is free)

// checking each attribute
#define IS_RESERVED(attributes)  ((attributes) & ATTRIBUTE_IS_RESERVED)
#define IS_FREE(attributes)      ((attributes) & ATTRIBUTE_IS_FREE)

// setting each attribute
#define SET_RESERVED(attributes) ((attributes) |= ATTRIBUTE_IS_RESERVED)
#define SET_FREE(attributes)     ((attributes) |= ATTRIBUTE_IS_FREE)

// clearing each attribute
#define CLEAR_RESERVED(attributes) ((attributes) &= ~ATTRIBUTE_IS_RESERVED)
#define CLEAR_FREE(attributes)     ((attributes) &= ~ATTRIBUTE_IS_FREE)

typedef struct {
    uint32_t next_cluster; 
    uint8_t attributes;  // free/file_system...
} FATEntry; //attributes padded with 1 3 bytes making = 8 bytes

typedef struct {
    FATEntry *entries;  // FAT entries (one per cluster)
    uint32_t total_entries;  // total number of entries (clusters)
} FAT;

FAT *read_fat(Disk *disk, const char* image_file);
void update_fat(FAT *fat, uint32_t cluster, uint32_t next_cluster);
uint32_t allocate_cluster(FAT *fat);
void deallocate_cluster(FAT *fat, uint32_t cluster);

#endif
