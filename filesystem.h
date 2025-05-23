#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>

#define MAX_CONTENT_SIZE 1024

int create_file(const char* filename, const char* content);
int modify_file(const char* filename, const char* new_content);
void view_file(const char* filename);
int delete_file(const char* filename);

#endif
