
/*
* Copyright - Talha
*/

#include "../include/fat_table.h"


FAT *read_fat(Disk *disk, const char* image_file) {
    
  FAT* fat = (FAT*) malloc(sizeof(FAT));
  fat->total_entries = disk->num_fat_entries;
  fat->entries = (FATEntry*) malloc(sizeof(FATEntry) * fat->total_entries);

  FILE* file = fopen(image_file, "rb");
  if (file == NULL) {
    printf("Could not read the file for FAT entries\n");
  }

  fseek(file, sizeof(Disk), SEEK_SET);

  for(uint32_t i = 0; i < fat->total_entries; i++) {
    fread(&fat->entries[i], sizeof(FATEntry), 1, file);
  }
  fclose(file);

  printf("FAT successfully read from the file '%s'.\n", image_file);
  printf("Total FAT Entries: %u\n", fat->total_entries);

  return fat;
}
