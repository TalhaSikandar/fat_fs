/*
* Copyright - Talha
*/

#include "../include/disk.h"
#include "../include/directory.h"
#include "../include/fat_table.h"

// Structures Sizes
// Size of Disk: 48 bytes
// Size of DirectoryEntry: 76 bytes
// Size of FATEntry: 8 bytes

// int main() {
//     // Disk* d = initialize_disk_ffile("fs1.config", "fs1.img");
//     // print_disk_info(d);
//     Disk* d_disk = read_disk("fs1.img");
//     print_disk(d_disk);
//     // FAT* fat = initialize_fat(d_disk, "fs1.img");
//     MinHeap* mh;
//     FAT* fat_disk = read_fat(d_disk, "fs1.img", mh);
//     print_fat(fat_disk, 17);
//     free(fat_disk->entries);
//     free(fat_disk);
//     free(d_disk);
//     return 0;
// }
//
int main() {
  Disk* d_disk = read_disk("fs1.img");
  print_disk(d_disk);
  MinHeap* mh;
  FAT* fat_disk = read_fat(d_disk, "fs1.img", &mh);
  print_fat(fat_disk, 17);

  const char* test_data = "this is some thing important\0";
  write_clustor("fs1.img", d_disk, fat_disk, mh, test_data, strlen(test_data));

  free(fat_disk->entries);
  free(fat_disk);
  free(d_disk);

  return 0;
}

