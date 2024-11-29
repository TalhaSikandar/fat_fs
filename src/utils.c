#include "../include/utils.h"

uint32_t calculate_required_sectors(uint32_t entry_size, uint32_t sector_size) {
  uint32_t disk_struct_sectors = entry_size / sector_size;
  if (entry_size % sector_size != 0) {
    disk_struct_sectors += 1;
  }
  return disk_struct_sectors;
}

uint32_t calculate_required_clusters(uint32_t entry_size, uint32_t cluster_size) {
    uint32_t required_clusters = entry_size / cluster_size;
    if (entry_size % cluster_size != 0) {
        required_clusters += 1;
    }
    return required_clusters;
}
