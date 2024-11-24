/*
* Copyright - Talha
*/
#include "../include/disk.h"
#include "../include/fat_table.h"
#include "../include/directory.h"
#include <stdbool.h>


Disk *initialize_disk(uint32_t _disk_size, 
                      uint32_t _sector_size,
                      uint32_t _sectors_per_cluster, 
                      bool  _isBootable,
                      uint32_t _root_directory_clustors, 
                      uint32_t _clusters_per_file,
                      const char* file_name) {
  Disk* d = (Disk*) malloc (sizeof(Disk));

  d->disk_size           = _disk_size;
  d->sector_size         = _sector_size;
  d->sectors_per_cluster = _sectors_per_cluster;
  d->isBootable          = _isBootable;
  d->clusters_per_file   = _clusters_per_file;

  d->total_sectors       = d->disk_size / d->sector_size;
  d->cluster_size        = d->sector_size * d->sectors_per_cluster;

  // calculating total_files that can occupy specifed clustors for root directory
  d->reserved_clusters_root_dir = _root_directory_clustors;
  uint64_t total_clustor_size = d->reserved_clusters_root_dir * d->cluster_size;
  uint64_t max_files = total_clustor_size / sizeof(DirectoryEntry);

  // size of FAT table calculations
  d->num_fat_entries = max_files * d->clusters_per_file; 
  d->num_fat_entries += d->reserved_clusters_root_dir;
  uint64_t total_fat_table_size = sizeof(FATEntry) * d->num_fat_entries;

  // actual_data index
  d->data_index = total_fat_table_size + sizeof(Disk);

}

Disk *initialize_disk(FILE* config_file); // initialize_disk using config file

void read_disk(Disk *disk,
               uint32_t sector); // Will read the disk(boot_sector)

void write_disk(Disk *disk,
                uint32_t sector); // Will write to disk if found any modification to root_dir_cluster

void free_disk(Disk *disk);  // clear disk struct

