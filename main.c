#include "filesystem.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int choice;
    char filename[256];
    char content[MAX_CONTENT_SIZE];
    
    while(1) {
        printf("\nFile System Manager\n");
        printf("1. Create new file\n");
        printf("2. Modify file\n");
        printf("3. View file\n");
        printf("4. Delete file\n");
        printf("5. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        while(getchar() != '\n'); // Очистка буфера ввода
        
        if (choice == 5) break;
        
        switch(choice) {
            case 1: {
                printf("Enter filename: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0';
                
                printf("Enter content (or press enter for default): ");
                fgets(content, sizeof(content), stdin);
                content[strcspn(content, "\n")] = '\0';
                
                create_file(filename, content[0] ? content : NULL);
                break;
            }
                
            case 2: {
                printf("Enter filename to modify: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0';
                
                printf("Enter new content: ");
                fgets(content, sizeof(content), stdin);
                content[strcspn(content, "\n")] = '\0';
                
                modify_file(filename, content);
                break;
            }
                
            case 3: {
                printf("Enter filename to view: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0';
                
                view_file(filename);
                break;
            }
                
            case 4: {
                printf("Enter filename to delete: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0';
                
                delete_file(filename);
                break;
            }
                
            default:
                printf("Invalid choice!\n");
        }
    }
    
    return 0;
}
