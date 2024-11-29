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

int main() {
  Disk* d = initialize_disk_ffile("fs1.config", "fs1.img");
  // Disk* d2 = initialize_disk_ffile("fs2.config", "fs2.img");
  // Disk* d = (Disk*) malloc(sizeof(Disk));
  // FILE* f = fopen("fs1.img", "r");
  // fread(d, sizeof(Disk), 1, f);
  print_disk_info(d);
  // print_disk_info(d2);
  return 0;
}
