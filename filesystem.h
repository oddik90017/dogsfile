#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>

#define MAX_CONTENT_SIZE 1024
#define TEMP_FS "temp.fs"
#define FS_NAME "filesystem.fs"

FILE* open_filesystem(const char* filename) {
    FILE* file = fopen(filename, "r+");
    if (file == NULL) {
        file = fopen(filename, "w+");
        if (file == NULL) {
            perror("Failed to open or create filesystem");
            exit(EXIT_FAILURE);
        }
    }
    return file;
}

int create_new_file(FILE* fs_file, const char* filename, const char* content) {
    fseek(fs_file, 0, SEEK_END);
    fprintf(fs_file, "/%s\n", filename);
    
    if (content != NULL) {
        fprintf(fs_file, "%s\n", content);
    } else {
        fprintf(fs_file, "Default content for new file '%s'\n", filename);
    }
    
    fflush(fs_file);
    printf("File '%s' created successfully at the end of filesystem.\n", filename);
    return 1;
}
int modify_file(FILE* fs_file, const char* filename, const char* new_content) {
    FILE* temp_file = fopen(TEMP_FS, "w");
    if (temp_file == NULL) {
        perror("Failed to create temp file");
        return -1;
    }

    fseek(fs_file, 0, SEEK_SET);
    char line[256];
    bool found = false;
    bool in_target_file = false;
    
    while (fgets(line, sizeof(line), fs_file)) {
        line[strcspn(line, "\n")] = '\0';
        
        if (in_target_file) {
            if (line[0] == '/') {
                in_target_file = false;
            } else {
                continue; // Пропускаем старое содержимое
            }
        }
        
        if (line[0] == '/' && strcmp(line + 1, filename) == 0) {
            found = true;
            in_target_file = true;
            fprintf(temp_file, "/%s\n", filename); // Сохраняем заголовок файла
            if (new_content != NULL) {
                fprintf(temp_file, "%s\n", new_content); // Записываем новое содержимое
            }
            continue;
        }
        
        fprintf(temp_file, "%s\n", line);
    }
    
    fclose(temp_file);
    fclose(fs_file);
    
    if (!found) {
        remove(TEMP_FS);
        printf("File '%s' not found in the filesystem.\n", filename);
        return -1;
    }
    
    remove(FS_NAME);
    rename(TEMP_FS, FS_NAME);
    
    printf("File '%s' modified successfully.\n", filename);
    return 0;
}

void view_file(FILE* fs_file, const char* filename) {
    fseek(fs_file, 0, SEEK_SET);
    char line[256];
    bool found = false;
    bool print = false;
    
    while (fgets(line, sizeof(line), fs_file)) {
        line[strcspn(line, "\n")] = '\0';
        
        if (print) {
            if (line[0] == '/') break;
            printf("%s\n", line);
            continue;
        }
        
        if (line[0] == '/' && strcmp(line + 1, filename) == 0) {
            found = true;
            print = true;
        }
    }
    
    if (!found) {
        printf("File '%s' not found.\n", filename);
    }
}
int delete_file(FILE* fs_file, const char* filename) {
    FILE* temp_file = fopen(TEMP_FS, "w");
    if (!temp_file) {
        perror("Failed to create temp file");
        return -1;
    }

    fseek(fs_file, 0, SEEK_SET);
    char line[256];
    bool found = false;
    bool skip = false;
    
    while (fgets(line, sizeof(line), fs_file)) {
        line[strcspn(line, "\n")] = '\0';
        
        if (skip) {
            if (line[0] == '/') skip = false;
            else continue;
        }
        
        if (line[0] == '/' && strcmp(line + 1, filename) == 0) {
            found = true;
            skip = true;
            continue;
        }
        
        fprintf(temp_file, "%s\n", line);
    }
    
    fclose(temp_file);
    fclose(fs_file);
    
    if (!found) {
        remove(TEMP_FS);
        printf("File '%s' not found.\n", filename);
        return -1;
    }
    
    remove(FS_NAME);
    rename(TEMP_FS, FS_NAME);
    printf("File '%s' deleted successfully.\n", filename);
    return 0;
}

#endif /* FILESYSTEM_H */
