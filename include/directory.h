/*
* Copyright - Talha
* DISK.h (Structure for boot_sector)
*/
#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME_LENGTH 64
#define FILE_EXTENSION_LENGTH 3

/*
  * Attributes: 8-bits (1 Byte) 0 0 0 0 ( Last 4 Bits - right)
  *                                   | isDirectory
  *                                 | isHidden
  *                               | isSystemFile
  *                             | isReadOnly
  *                             0 0 0 0 ( First 4 Bits - left)
  *                                   | isFreeToWriteANewEntry
  *                                 | isEncrypted
  *                               | NotinUse
  *                             | NotinUse
*/


#define ATTRIBUTE_IS_DIRECTORY     (1 << 0)  // 0x01
#define ATTRIBUTE_IS_HIDDEN        (1 << 1)  // 0x02
#define ATTRIBUTE_IS_SYSTEMFILE    (1 << 2)  // 0x04
#define ATTRIBUTE_IS_READONLY      (1 << 3)  // 0x08
#define ATTRIBUTE_FREE_TO_WRITE    (1 << 4)  // 0x01
#define ATTRIBUTE_IS_ENCRYPTED        (1 << 5)  // 0x20
#define ATTRIBUTE_NOTINUSE2        (1 << 6)  // 0x40
#define ATTRIBUTE_NOTINUSE3        (1 << 7)  // 0x80

// checking each attribute
#define IS_READONLY(attributes)     ((attributes) & ATTRIBUTE_IS_READONLY)
#define IS_HIDDEN(attributes)       ((attributes) & ATTRIBUTE_IS_HIDDEN)
#define IS_SYSTEMFILE(attributes)   ((attributes) & ATTRIBUTE_IS_SYSTEMFILE)
#define IS_DIRECTORY(attributes)    ((attributes) & ATTRIBUTE_IS_DIRECTORY)
#define IS_FREE_TO_WRITE(attributes)((attributes) & ATTRIBUTE_FREE_TO_WRITE)
#define IS_ENCRYPTED(attributes)    ((attributes) & ATTRIBUTE_IS_ENCRYPTED)
#define IS_NOTINUSE2(attributes)    ((attributes) & ATTRIBUTE_NOTINUSE2)
#define IS_NOTINUSE3(attributes)    ((attributes) & ATTRIBUTE_NOTINUSE3)

// setting each attribute
#define SET_READONLY(attributes)     ((attributes) |= ATTRIBUTE_IS_READONLY)
#define SET_HIDDEN(attributes)       ((attributes) |= ATTRIBUTE_IS_HIDDEN)
#define SET_SYSTEMFILE(attributes)   ((attributes) |= ATTRIBUTE_IS_SYSTEMFILE)
#define SET_DIRECTORY(attributes)    ((attributes) |= ATTRIBUTE_IS_DIRECTORY)
#define SET_FREE_TO_WRITE(attributes)((attributes) |= ATTRIBUTE_FREE_TO_WRITE)
#define SET_ENCRYPTED(attributes)    ((attributes) |= ATTRIBUTE_IS_ENCRYPTED)
#define SET_NOTINUSE2(attributes)    ((attributes) |= ATTRIBUTE_NOTINUSE2)
#define SET_NOTINUSE3(attributes)    ((attributes) |= ATTRIBUTE_NOTINUSE3)

// clearing each attribute
#define CLEAR_READONLY(attributes)     ((attributes) &= ~ATTRIBUTE_IS_READONLY)
#define CLEAR_HIDDEN(attributes)       ((attributes) &= ~ATTRIBUTE_IS_HIDDEN)
#define CLEAR_SYSTEMFILE(attributes)   ((attributes) &= ~ATTRIBUTE_IS_SYSTEMFILE)
#define CLEAR_DIRECTORY(attributes)    ((attributes) &= ~ATTRIBUTE_IS_DIRECTORY)
#define CLEAR_FREE_TO_WRITE(attributes)((attributes) &= ~ATTRIBUTE_FREE_TO_WRITE)
#define CLEAR_ENCRYPTED(attributes)    ((attributes) &= ~ATTRIBUTE_IS_ENCRYPTED)
#define CLEAR_NOTINUSE2(attributes)    ((attributes) &= ~ATTRIBUTE_NOTINUSE2)
#define CLEAR_NOTINUSE3(attributes)    ((attributes) &= ~ATTRIBUTE_NOTINUSE3)

typedef struct {
    uint8_t attributes;  // file attributes (read-only, hidden, system, directory, etc.)
    char name[FILE_NAME_LENGTH];  // 64-bytes in name
    char ext[FILE_EXTENSION_LENGTH];   // 3-byte file extension
    uint32_t file_size;  // Size of the file in bytes
    uint32_t first_cluster;  // First cluster of the file's data
} DirectoryEntry; // 76 bytes padding for ext +3 added

DirectoryEntry *create_file(const char *filename, uint32_t size);
void delete_file(DirectoryEntry *entry);
void list_files();

#endif // DIRECTORY_H
