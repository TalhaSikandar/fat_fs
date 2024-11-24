/*
* Copyright - Talha
*/
#include "../include/disk.h"
#include "../include/fat_table.h"
#include "../include/directory.h"

#include <stdbool.h>
#include <fcntl.h>   // For open, O_CREAT, O_WRONLY, etc.
#include <unistd.h>  // For close

Disk* initialize_disk(uint32_t _disk_size, 
                      uint32_t _sector_size,
                      uint32_t _sectors_per_cluster, 
                      bool _isBootable,
                      uint32_t _root_directory_clusters, 
                      uint32_t _clusters_per_file, 
                      const char* image_name) {
    printf("Initialization started...\n");

    Disk* d = (Disk*) malloc(sizeof(Disk));
    if (d == NULL) {
        printf("Error: Memory allocation failed for Disk structure.\n");
        return NULL;
    }

    d->disk_size           = _disk_size;
    d->sector_size         = _sector_size;
    d->sectors_per_cluster = _sectors_per_cluster;
    d->isBootable          = _isBootable;
    d->clusters_per_file   = _clusters_per_file;

    printf("Disk size: %u bytes\n", d->disk_size);
    printf("Sector size: %u bytes\n", d->sector_size);
    printf("Sectors per cluster: %u\n", d->sectors_per_cluster);

    // calculate total sectors and cluster size
    d->total_sectors       = d->disk_size / d->sector_size;
    d->cluster_size        = d->sector_size * d->sectors_per_cluster;

    printf("Total sectors: %u\n", d->total_sectors);
    printf("Cluster size: %u bytes\n", d->cluster_size);

    // calculate reserved clusters for root directory and maximum files
    d->reserved_clusters_root_dir = _root_directory_clusters;
    uint64_t total_cluster_size = d->reserved_clusters_root_dir * d->cluster_size;
    uint64_t max_files = total_cluster_size / sizeof(DirectoryEntry);

    printf("Reserved clusters for root directory: %u\n", d->reserved_clusters_root_dir);
    printf("Total cluster size for root directory: %llu bytes\n", total_cluster_size);
    printf("Maximum files that can be stored: %llu\n", max_files);

    // calculate the number of FAT entries
    d->num_fat_entries = max_files * d->clusters_per_file;
    d->num_fat_entries += d->reserved_clusters_root_dir;

    printf("Number of FAT entries: %u\n", d->num_fat_entries);

    // calculate the total FAT table size
    uint64_t total_fat_table_size = sizeof(FATEntry) * d->num_fat_entries;
    d->data_index = total_fat_table_size + sizeof(Disk);

    printf("Total FAT table size: %llu bytes\n", total_fat_table_size);
    printf("Data index (start of actual data): %llu bytes\n", d->data_index);

    // check if the total size of FAT table, root directory, and data exceeds disk size
    uint64_t total_used_size = total_fat_table_size + sizeof(Disk);
    if (total_used_size > d->disk_size) {
        printf("Error: The total disk size exceeds the allocated disk size.\n");
        free(d); 
        return NULL;
    }
    
    save_to_file(d, image_name);
    printf("Initialization complete.\n");
    return d;
}

void save_to_file(Disk* d, const char* file_name) {
    FILE* file = fopen(file_name, "wb");
    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
        free(d);
        return;
    }

    // Allocate the disk image by writing zeros
    uint8_t zero = 0;
    for (uint64_t i = 0; i < d->disk_size; ++i) {
        if (fwrite(&zero, sizeof(uint8_t), 1, file) != 1) {
            printf("Error: Failed to allocate the disk.\n");
            fclose(file);
            free(d);
            return;
        }
    }

    printf("Successfully allocated %s\n", file_name);
    fclose(file);

    write_disk(file_name, d);
}

Disk* initialize_disk_ffile(const char* _config_file, const char* image_file) {
  FILE* config_file = fopen(_config_file, "r");
    if (config_file == NULL) {
        printf("Error: Configuration file is NULL.\n");
        return NULL;
    }

    uint32_t disk_size, sector_size, sectors_per_cluster, root_directory_clusters, clusters_per_file;
    bool isBootable;
    char file_name[256];

    if (fscanf(config_file, "disk_size = %u\n", &disk_size) != 1) {
        printf("Error: Failed to read disk_size from config file.\n");
        return NULL;
    }

    if (fscanf(config_file, "sector_size = %u\n", &sector_size) != 1) {
        printf("Error: Failed to read sector_size from config file.\n");
        return NULL;
    }

    if (fscanf(config_file, "sectors_per_cluster = %u\n", &sectors_per_cluster) != 1) {
        printf("Error: Failed to read sectors_per_cluster from config file.\n");
        return NULL;
    }

    if (fscanf(config_file, "isBootable = %d\n", &isBootable) != 1) {
        printf("Error: Failed to read isBootable from config file.\n");
        return NULL;
    }

    if (fscanf(config_file, "root_directory_clusters = %u\n", &root_directory_clusters) != 1) {
        printf("Error: Failed to read root_directory_clusters from config file.\n");
        return NULL;
    }

    if (fscanf(config_file, "clusters_per_file = %u\n", &clusters_per_file) != 1) {
        printf("Error: Failed to read clusters_per_file from config file.\n");
        return NULL;
    }


    return initialize_disk(disk_size, sector_size, sectors_per_cluster, isBootable, 
                           root_directory_clusters, clusters_per_file, image_file);
}
void print_disk_info(const Disk* d) {
    if (d == NULL) {
        printf("Error: Disk structure is NULL.\n");
        return;
    }

    printf("Disk Information:\n");
    printf("------------------\n");
    printf("Sector Size: %u bytes\n", d->sector_size);
    printf("Total Sectors: %u\n", d->total_sectors);
    printf("Sectors Per Cluster: %u\n", d->sectors_per_cluster);
    printf("Cluster Size: %u bytes\n", d->cluster_size);
    printf("Number of FAT Entries: %u\n", d->num_fat_entries);
    printf("Disk Size: %u bytes\n", d->disk_size);
    printf("Reserved Clusters for Root Directory: %u\n", d->reserved_clusters_root_dir);
    printf("Clusters Per File: %u\n", d->clusters_per_file);
    printf("Data Index: %llu bytes\n", (unsigned long long)d->data_index);
    printf("Bootable: %s\n", d->isBootable ? "Yes" : "No");
    printf("------------------\n");
}

void read_disk(const char* image_file, Disk* d) {
  FILE* file = fopen("image_file", "r");
  if (file == NULL) {
    printf("Unable to read the file");
  }

  fread(d, sizeof(Disk), 1, file);

  return;

}
void write_disk(const char* image_file, Disk *d) {
    FILE* file = fopen(image_file, "w");

    fseek(file, 0, SEEK_SET); // Move to the start of the file
    if (fwrite(d, sizeof(Disk), 1, file) != 1) {
        printf("Error: Failed to write Disk structure to file.\n");
        fclose(file);
        free(d);
        return;
    }

    fclose(file);
    printf("Disk structure written to %s\n", image_file);
}
void free_disk(Disk *disk) {
  free(disk);
  return;
}

