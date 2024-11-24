#ifndef FAT_FS_H
#define FAT_FS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/disk.h"
#include "../include/fat_table.h"
#include "../include/utils.h"
#include "../include/directory.h"

// Structure definitions
typedef struct {
    Disk *disk;              // The associated disk
    FAT *fat;                // The File Allocation Table
    DirectoryEntry *root_dir; // Root directory entry array
    uint32_t num_files;      // Number of files in the root directory
    uint32_t num_used_files; // Total count for used files
} FileSystem;

FileSystem *initialize_filesystem(Disk *disk);
int create_file(FileSystem *fs, const char *filename, uint32_t size);
void write_data(FileSystem *fs, DirectoryEntry *entry, const char *data);
void read_data(FileSystem *fs, DirectoryEntry *entry, char *buffer);

#endif
