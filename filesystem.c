#include "filesystem.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FILE_MARKER '#'

static bool file_exists(FILE* fs, const char* filename) {
    fseek(fs, 0, SEEK_SET);
    char line[MAX_FILENAME + 2];
    
    while (fgets(line, sizeof(line), fs)) {
        line[strcspn(line, "\n")] = '\0';
        if (line[0] == FILE_MARKER && strcmp(line + 1, filename) == 0) {
            return true;
        }
    }
    return false;
}

int view_file(FILE* fs, const char* filename) {
    fseek(fs, 0, SEEK_SET);
    char line[MAX_CONTENT];
    bool found = false;
    
    while (fgets(line, sizeof(line), fs)) {
        line[strcspn(line, "\n")] = '\0';
        
        if (!found && line[0] == FILE_MARKER && strcmp(line + 1, filename) == 0) {
            found = true;
            printf("Contents of '%s':\n", filename);
            continue;
        }
        
        if (found) {
            if (line[0] == FILE_MARKER) break;
            printf("%s\n", line);
        }
    }
    
    if (!found) {
        printf("File '%s' not found\n", filename);
        return -1;
    }
    return 0;
}

int delete_file(FILE** fs_ptr, const char* filename) {
    FILE* fs = *fs_ptr;
    
    if (!file_exists(fs, filename)) {
        printf("File '%s' not found\n", filename);
        return -1;
    }

    FILE* temp = fopen(TEMP_FILE, "w");
    if (!temp) {
        perror("Failed to create temp file");
        return -1;
    }

    fseek(fs, 0, SEEK_SET);
    char line[MAX_CONTENT];
    bool skip = false;
    
    while (fgets(line, sizeof(line), fs)) {
        line[strcspn(line, "\n")] = '\0';
        
        if (line[0] == FILE_MARKER) {
            if (strcmp(line + 1, filename) == 0) {
                skip = true;
                continue;
            } else {
                skip = false;
            }
        }
        
        if (!skip) {
            fprintf(temp, "%s\n", line);
        }
    }

    fclose(temp);
    fclose(fs);
    
    if (remove(FS_CONTAINER) != 0) {
        perror("Failed to remove old container");
        return -1;
    }
    
    if (rename(TEMP_FILE, FS_CONTAINER) != 0) {
        perror("Failed to rename temp file");
        return -1;
    }
    
    // Переоткрываем основной файл
    *fs_ptr = fopen(FS_CONTAINER, "r+");
    if (!*fs_ptr) {
        perror("Failed to reopen filesystem");
        return -1;
    }
    
    printf("File '%s' deleted successfully\n", filename);
    return 0;
}

FILE* init_filesystem() {
    FILE* fs = fopen(FS_CONTAINER, "a+");
    if (!fs) {
        perror("Failed to initialize filesystem");
        exit(EXIT_FAILURE);
    }
    return fs;
}

int create_file(FILE** fs, const char* filename, const char* content) {
    if (file_exists(*fs, filename)) {
        printf("File '%s' already exists\n", filename);
        return -1;
    }

    fseek(*fs, 0, SEEK_END);
    fprintf(*fs, "%c%s\n", FILE_MARKER, filename);
    fprintf(*fs, "%s\n", content ? content : "Default content");
    fflush(*fs);
    
    printf("File '%s' created successfully\n", filename);
    return 0;
}

int modify_file(FILE** fs_ptr, const char* filename, const char* new_content) {
    FILE* fs = *fs_ptr;
    
    if (!file_exists(fs, filename)) {
        printf("File '%s' not found\n", filename);
        return -1;
    }

    FILE* temp = fopen(TEMP_FILE, "w");
    if (!temp) {
        perror("Failed to create temp file");
        return -1;
    }

    fseek(fs, 0, SEEK_SET);
    char line[MAX_CONTENT];
    bool in_target = false;
    
    while (fgets(line, sizeof(line), fs)) {
        line[strcspn(line, "\n")] = '\0';
        
        if (line[0] == FILE_MARKER) {
            if (strcmp(line + 1, filename) == 0) {
                in_target = true;
                fprintf(temp, "%c%s\n", FILE_MARKER, filename);
                fprintf(temp, "%s\n", new_content ? new_content : "Modified content");
                // Пропускаем старое содержимое
                while (fgets(line, sizeof(line), fs) && line[0] != FILE_MARKER);
                in_target = false;
                continue;
            }
        }
        
        if (!in_target) {
            fprintf(temp, "%s\n", line);
        }
    }

    fclose(temp);
    fclose(fs);
    
    if (remove(FS_CONTAINER) != 0) {
        perror("Failed to remove old container");
        return -1;
    }
    
    if (rename(TEMP_FILE, FS_CONTAINER) != 0) {
        perror("Failed to rename temp file");
        return -1;
    }
    
    // Переоткрываем основной файл
    *fs_ptr = fopen(FS_CONTAINER, "r+");
    if (!*fs_ptr) {
        perror("Failed to reopen filesystem");
        return -1;
    }
    
    printf("File '%s' modified successfully\n", filename);
    return 0;
}
