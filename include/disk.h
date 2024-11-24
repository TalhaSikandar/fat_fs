/*
* Copyright - Talha
* DISK.h (Structure for boot_sector)
*/
#ifndef DISK_H
#define DISK_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
* 2^32: 2GB
* sector: It will help in reading the image sector by sector (mirroring an origianl implementation) in the RAM.
* cluser: just like pages, it helps to define modularity and constant size overall.
* reserved_sectors: Helps to read total FAT
*/

typedef struct {
    uint32_t sector_size;  // size in bytes
    uint32_t total_sectors;    // total sectors in the disk
    uint32_t sectors_per_cluster;  // number of sectors per cluster
    uint32_t cluster_size;
    uint32_t num_fat_entries;  // size of each FAT which will be calculated using total entries in fat
    uint32_t disk_size;  // total size of the disk in bytes
    uint32_t reserved_clusters_root_dir; // helping with total_files and hence calculating fat size, will be in the start of fat
    uint32_t clusters_per_file;
    uint64_t data_index;
    bool  isBootable;
} Disk;


Disk *initialize_disk(uint32_t _disk_size, 
                      uint32_t _sector_size,
                      uint32_t _sectors_per_cluster, 
                      uint8_t  _isBootable); // initialize_disk using parameters

Disk *initialize_disk(FILE* config_file); // initialize_disk using config file

void read_disk(Disk *disk,
               uint32_t sector); // Will read the disk(boot_sector)

void write_disk(Disk *disk,
                uint32_t sector); // Will write to disk if found any modification to root_dir_cluster

void free_disk(Disk *disk);  // clear disk struct

#endif // DISK_H
