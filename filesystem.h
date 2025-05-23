#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>
#include <stdbool.h>

#define FS_CONTAINER "fs_container.dat"
#define TEMP_FILE "temp_fs.dat"
#define MAX_FILENAME 256
#define MAX_CONTENT 1024

FILE* init_filesystem();
int create_file(FILE** fs, const char* filename, const char* content);
int modify_file(FILE** fs, const char* filename, const char* new_content);
int view_file(FILE* fs, const char* filename);
int delete_file(FILE** fs, const char* filename);

#endif
