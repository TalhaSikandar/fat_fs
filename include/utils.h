#include <stdint.h>
#include <memory.h>

uint32_t calculate_required_sectors(uint32_t entry_size, uint32_t sector_size);
uint32_t calculate_required_clusters(uint32_t entry_size, uint32_t cluster_size);
