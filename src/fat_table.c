
/*
* Copyright - Talha
*/

#include "../include/fat_table.h"

FAT* initialize_fat(Disk* disk, const char* image_file) {
    FAT* fat = (FAT*)malloc(sizeof(FAT));
    if (fat == NULL) {
        printf("Error: Could not allocate memory for FAT.\n");
        return NULL;
    }

    fat->total_entries = disk->num_fat_entries;
    fat->entries = (FATEntry*)malloc(sizeof(FATEntry) * fat->total_entries);
    if (fat->entries == NULL) {
        printf("Error: Could not allocate memory for FAT entries.\n");
        free(fat);
        return NULL;
    }

    for (uint32_t i = 0; i < fat->total_entries; i++) {
        if (i < disk->reserved_clusters_root_dir) {
            // Mark reserved clusters for the root directory
            fat->entries[i].attributes = 0;
            SET_RESERVED(fat->entries[i].attributes);
        } else {
            // Mark other clusters as free
            fat->entries[i].attributes = 0;
            SET_FREE(fat->entries[i].attributes);
        }
        fat->entries[i].next_cluster = 0; // Default to no next cluster
    }

    write_fat(fat, disk, image_file);
    
    return fat;
}

void write_fat(FAT* fat, Disk* disk, const char* image_file) {
    FILE* file = fopen(image_file, "rb+");
    if (file == NULL) {
        printf("Could not open the file for writing FAT.\n");
        return;
    }

    uint32_t fat_offset = calculate_required_sectors(sizeof(Disk), disk->sector_size) * disk->sector_size;
    uint32_t fat_size = sizeof(FATEntry) * fat->total_entries;
    uint32_t fat_sectors = calculate_required_sectors(fat_size, disk->sector_size);

    uint8_t* sector_buffer = (uint8_t*)malloc(disk->sector_size);
    if (sector_buffer == NULL) {
        printf("Error: Could not allocate buffer for FAT sector writing.\n");
        fclose(file);
        return;
    }

    fseek(file, fat_offset, SEEK_SET);
    for (uint32_t i = 0; i < fat_sectors; i++) {
        memset(sector_buffer, 0, disk->sector_size); 

        uint32_t start_index = i * (disk->sector_size / sizeof(FATEntry));
        uint32_t entries_to_copy = (i == fat_sectors - 1)
                                    ? fat->total_entries - start_index
                                    : disk->sector_size / sizeof(FATEntry);

        memcpy(sector_buffer, fat->entries + start_index, entries_to_copy * sizeof(FATEntry));
        fwrite(sector_buffer, disk->sector_size, 1, file);
    }

    free(sector_buffer);
    fclose(file);
    printf("FAT table written successfully to sectors starting at offset %u.\n", fat_offset);
}

FAT* read_fat(Disk *disk, const char* image_file, MinHeap** min_heap) {
    FAT* fat = (FAT*) malloc(sizeof(FAT));
    if (fat == NULL) {
        printf("Error: Could not allocate memory for FAT structure.\n");
        return NULL;
    }

    fat->total_entries = disk->num_fat_entries;
    fat->entries = (FATEntry*) malloc(sizeof(FATEntry) * fat->total_entries);
    if (fat->entries == NULL) {
        printf("Error: Could not allocate memory for FAT entries.\n");
        free(fat);
        return NULL;
    }

    FILE* file = fopen(image_file, "rb");
    if (file == NULL) {
        printf("Error: Could not open the file '%s' for reading.\n", image_file);
        free(fat->entries);
        free(fat);
        return NULL;
    }

    uint32_t fat_offset = calculate_required_sectors(sizeof(Disk), disk->sector_size) * disk->sector_size;

    fseek(file, fat_offset, SEEK_SET);

    uint32_t fat_size = sizeof(FATEntry) * fat->total_entries;
    uint32_t fat_sectors = calculate_required_sectors(fat_size, disk->sector_size);

    uint8_t* buffer = (uint8_t*) malloc(disk->sector_size);
    if (buffer == NULL) {
        printf("Error: Could not allocate buffer for reading FAT.\n");
        fclose(file);
        free(fat->entries);
        free(fat);
        return NULL;
    }

    for (uint32_t i = 0; i < fat_sectors; i++) {
        memset(buffer, 0, disk->sector_size);

        fread(buffer, disk->sector_size, 1, file);

        uint32_t start_index = i * (disk->sector_size / sizeof(FATEntry));
        uint32_t entries_to_copy = (i == fat_sectors - 1) 
                                    ? fat->total_entries - start_index 
                                    : disk->sector_size / sizeof(FATEntry);

        memcpy(fat->entries + start_index, buffer, entries_to_copy * sizeof(FATEntry));
    }

    fclose(file);

    printf("FAT successfully read from the file '%s'.\n", image_file);
    printf("Total FAT Entries: %u\n", fat->total_entries);

    free(buffer); 

    // initializing min_heap
    *min_heap = create_min_heap(fat->total_entries);
    initialize_heap(*min_heap, fat);
    return fat;
}


void print_fat(FAT* fat, uint32_t num_entries_to_print) {
    // Ensure that we do not print more than the available entries uint32_t entries_to_print = (num_entries_to_print > fat->total_entries) ? fat->total_entries : num_entries_to_print;

    // Print table header
    printf("Entry \t\t Next Cluster \t\t Attributes\n");
    printf("--------------------------------------------\n");

    for (uint32_t i = 0; i < num_entries_to_print; i++) {
        printf("%-10u \t %-15u \t 0x%02X\n", i, fat->entries[i].next_cluster, fat->entries[i].attributes);
    }

    printf("\n");
}

void initialize_heap(MinHeap *heap, FAT *fat) {
    for (uint32_t i = 0; i < fat->total_entries; i++) {
        if (IS_FREE(fat->entries[i].attributes)) {
            min_heap_push(heap, i);
        }
    }
}

uint32_t allocate_cluster(FAT *fat, MinHeap *heap) {
    if (heap->size == 0) return -1; 
    uint32_t cluster = min_heap_pop(heap);
    CLEAR_FREE(fat->entries[cluster].attributes);
    fat->entries[cluster].next_cluster = -1; 
    return cluster;
}

void deallocate_cluster(FAT *fat, MinHeap *heap, uint32_t cluster) {
    if (IS_RESERVED(fat->entries[cluster].attributes)) return; 
    SET_FREE(fat->entries[cluster].attributes);
    fat->entries[cluster].next_cluster = 0; 
    min_heap_push(heap, cluster);
}

uint64_t calculate_cluster_offset(Disk *disk, uint32_t cluster) {
    return disk->data_index + (cluster * disk->cluster_size);
}
void update_fat(FAT *fat, uint32_t cluster, uint32_t next_cluster) {
    fat->entries[cluster].next_cluster = next_cluster;
    CLEAR_FREE(fat->entries[cluster].attributes);
}
char* read_clustor(const char* image_file, Disk *disk, uint32_t cluster_no, uint32_t *size) {
    uint64_t offset = calculate_cluster_offset(disk, cluster_no);
    FILE *image = fopen(image_file, "rb");
    if (!image) return NULL;

    char *buffer = (char*)malloc(disk->cluster_size);
    fseek(image, offset, SEEK_SET);
    fread(buffer, 1, disk->cluster_size, image);
    fclose(image);

    *size = disk->cluster_size;
    return buffer;
}

void write_clustor(const char* image_file, Disk *disk, FAT *fat, MinHeap *heap, const char *data, uint32_t size) {
  uint32_t clusters_needed = calculate_required_clusters(size, disk->cluster_size);

  uint32_t prev_cluster = -1;

  for (uint32_t i = 0; i < clusters_needed; i++) {
      uint32_t current_cluster = allocate_cluster(fat, heap);
      if (current_cluster == (uint32_t)-1) {
          printf("Error: Not enough space to write data\n");
          return;
      }

      uint64_t offset = calculate_cluster_offset(disk, current_cluster);

      FILE *image = fopen(image_file, "rb+");
      if (!image) {
          printf("Error: Could not open disk image\n");
          return;
      }

      fseek(image, offset, SEEK_SET);

      uint32_t bytes_to_write = (i == clusters_needed - 1) ? (size % disk->cluster_size) : disk->cluster_size;
      fwrite(data + (i * disk->cluster_size), 1, bytes_to_write, image);
      printf("Offset is: %u\n", offset);
      printf("Wrote %u Bytes in the clustor :%u\n", bytes_to_write, current_cluster);
      fclose(image);

      if (prev_cluster != (uint32_t)-1) {
          update_fat(fat, prev_cluster, current_cluster);
      }

      prev_cluster = current_cluster;
  }
  printf("Wrote %u Bytes in clustors :%u\n", size, clusters_needed);
  if (prev_cluster != (uint32_t)-1) {
      fat->entries[prev_cluster].next_cluster = (uint32_t)-1;
      SET_LAST(fat->entries[prev_cluster].attributes);
  }
}

// void write_clustor(Disk *disk, FAT *fat, MinHeap *heap, const char *data, uint32_t size) {
//     uint32_t clusters_needed = calculate_required_clusters(size, disk->cluster_size);
//
//     uint32_t prev_cluster = -1;
//     uint32_t data_written = 0;  // Tracks the total bytes written
//
//     for (uint32_t i = 0; i < clusters_needed; i++) {
//         uint32_t current_cluster = allocate_cluster(fat, heap);
//         if (current_cluster == (uint32_t)-1) {
//             printf("Error: Not enough space to write data\n");
//             return;
//         }
//
//         for (uint32_t sector = 0; sector < disk->sectors_per_cluster; sector++) {
//             // Calculate the offset for the current sector
//             uint64_t offset = calculate_cluster_offset(disk, current_cluster) + (sector * disk->sector_size);
//
//             FILE *image = fopen("disk.img", "r+b");
//             if (!image) {
//                 printf("Error: Could not open disk image\n");
//                 return;
//             }
//
//             fseek(image, offset, SEEK_SET);
//
//             uint32_t bytes_to_write = (size - data_written > disk->sector_size) ? disk->sector_size : (size - data_written);
//
//             fwrite(data + data_written, 1, bytes_to_write, image);
//             fclose(image);
//
//             data_written += bytes_to_write;
//
//             if (data_written >= size) {
//                 break;
//             }
//         }
//
//         if (prev_cluster != (uint32_t)-1) {
//             update_fat(fat, prev_cluster, current_cluster);
//         }
//
//         prev_cluster = current_cluster;
//
//         if (data_written >= size) {
//             break;
//         }
//     }
//
//     if (prev_cluster != (uint32_t)-1) {
//         fat->entries[prev_cluster].next_cluster = (uint32_t)-1;
//         SET_LAST(fat->entries[prev_cluster].attributes);
//     }
// }
