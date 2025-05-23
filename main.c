#include "filesystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main() {
    FILE* fs = init_filesystem();
    int choice;
    char filename[MAX_FILENAME];
    char content[MAX_CONTENT];
    
    while (1) {
        printf("\nFile System Menu:\n");
        printf("1. Create file\n");
        printf("2. Modify file\n");
        printf("3. View file\n");
        printf("4. Delete file\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');
        
        if (choice == 5) break;
        
        printf("Enter filename: ");
        fgets(filename, sizeof(filename), stdin);
        filename[strcspn(filename, "\n")] = '\0';
        
        switch (choice) {
            case 1:
                printf("Enter content: ");
                fgets(content, sizeof(content), stdin);
                content[strcspn(content, "\n")] = '\0';
                create_file(&fs, filename, content);
                break;
                
            case 2:
                printf("Enter new content: ");
                fgets(content, sizeof(content), stdin);
                content[strcspn(content, "\n")] = '\0';
                modify_file(&fs, filename, content);
                break;
                
            case 3:
                view_file(fs, filename);
                break;
                
            case 4:
                delete_file(&fs, filename);
                break;
                
            default:
                printf("Invalid choice!\n");
        }
    }
    
    fclose(fs);
    return 0;
}
