#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "filesystem.h"

int main() {
    FILE* fs_file = NULL;
    int choice;
    char filename[256];
    char content[MAX_CONTENT_SIZE];
    
    while(1) {
        printf("\nFile System Manager\n");
        printf("1. Create new or Open file\n");
        printf("2. Modify existing file\n");
        printf("3. View file\n");
        printf("4. Delete file\n");
        printf("5. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        while(getchar() != '\n');
        
        switch(choice) {
            case 1: {
                printf("Enter filename: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0';
                
                printf("Enter content (or press enter for default): ");
                fgets(content, sizeof(content), stdin);
                content[strcspn(content, "\n")] = '\0';
                
                fs_file = open_filesystem(FS_NAME);
                create_new_file(fs_file, filename, 
                              content[0] ? content : NULL);
                fclose(fs_file);
                break;
            }
                
            case 2: {
                printf("Enter filename to modify: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0';
                
                printf("Enter new content: ");
                fgets(content, sizeof(content), stdin);
                content[strcspn(content, "\n")] = '\0';
                
                fs_file = open_filesystem(FS_NAME);
                modify_file(fs_file, filename, content);
                break;
            }
                
            case 3: {
                printf("Enter filename to view: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0';
                
                fs_file = open_filesystem(FS_NAME);
                view_file(fs_file, filename);
                fclose(fs_file);
                break;
            }
                
            case 4: {
                printf("Enter filename to delete: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0';
                
                fs_file = open_filesystem(FS_NAME);
                delete_file(fs_file, filename);
                break;
            }
                
            case 5:
                return 0;
                
            default:
                printf("Invalid choice!\n");
        }
    }
}
