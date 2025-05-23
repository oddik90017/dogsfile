#include "filesystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int create_file(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to create file");
        return -1;
    }
    
    if (content != NULL && content[0] != '\0') {
        fprintf(file, "%s\n", content);
    } else {
        fprintf(file, "Default content for file '%s'\n", filename);
    }
    
    fclose(file);
    printf("File '%s' created successfully.\n", filename);
    return 0;
}

int modify_file(const char* filename, const char* new_content) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file for modification");
        return -1;
    }
    
    if (new_content != NULL && new_content[0] != '\0') {
        fprintf(file, "%s\n", new_content);
    } else {
        fprintf(file, "Modified content for file '%s'\n", filename);
    }
    
    fclose(file);
    printf("File '%s' modified successfully.\n", filename);
    return 0;
}

void view_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("File not found");
        return;
    }
    
    printf("Contents of '%s':\n", filename);
    char line[MAX_CONTENT_SIZE];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    
    fclose(file);
}

int delete_file(const char* filename) {
    if (remove(filename) == -1) {
        perror("Failed to delete file");
        return -1;
    }
    
    printf("File '%s' deleted successfully.\n", filename);
    return 0;
}
