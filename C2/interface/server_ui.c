#include "server_ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "database.h"

/*
 * start_server_ui():
 * This function runs a simple text-based menu. It loops indefinitely,
 * prompting the user for actions:
 *   1) List credentials
 *   2) Search by username
 *   3) Delete by ID
 *   4) Quit
 */


void *start_server_ui(void *arg) {
    (void)arg; 

    while (1) {
        printf("\n=== [ C2 Management Menu ] ===\n");
        printf("[1] List all credentials\n");
        printf("[2] Search credentials by username\n");
        printf("[3] Delete credentials by ID\n");
        printf("[4] Quit C2\n");
        printf("Enter your choice: ");
        fflush(stdout);

        char choice[16];
        if (!fgets(choice, sizeof(choice), stdin)) { //in case of error or EOF
            continue;
        }

        switch (choice[0]) {
            case '1': {
                printf("\n[List all credentials]\n");
                load_credentials();
                break;
            }
            case '2': {
                printf("\n[Search by username]\n");
                printf("Enter username: ");
                char user[256];
                if (fgets(user, sizeof(user), stdin)) { //remove any trailing newline
                    user[strcspn(user, "\n")] = '\0';
                    search_by_username(user);
                }
                break;
            }
            case '3': {
                printf("\n[Delete by ID]\n");
                printf("Enter the ID to delete: ");
                char id_str[64];
                if (fgets(id_str, sizeof(id_str), stdin)) {
                    int id = atoi(id_str);
                    delete_by_id(id);
                }
                break;
            }
            case '4': {
                printf("[!] Exiting C2 now...\n");
                exit(0);
            }
            default:
                printf("[!] Invalid choice.\n");
                break;
        }
    }

    return NULL;
}
