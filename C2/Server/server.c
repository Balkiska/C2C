#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

void *start_server_ui(void *arg) {
    (void)arg; // unused

    while (1) {
        printf("\n====== [ C2 Database Menu ] ======\n");
        printf("[1] List all credentials\n");
        printf("[2] Search by username\n");
        printf("[3] Delete by ID\n");
        printf("[4] Quit\n");
        printf("Choice: ");
        fflush(stdout);

        char line[32];
        if (!fgets(line, sizeof(line), stdin)) {
            continue;
        }

        switch (line[0]) {
            case '1': {
                load_credentials();
                break;
            }
            case '2': {
                printf("Enter username to search: ");
                char user[256];
                if (fgets(user, sizeof(user), stdin)) {
                    // remove trailing newline
                    user[strcspn(user, "\n")] = 0;
                    search_by_username(user);
                }
                break;
            }
            case '3': {
                printf("Enter ID to delete: ");
                char id_str[64];
                if (fgets(id_str, sizeof(id_str), stdin)) {
                    int id = atoi(id_str);
                    delete_by_id(id);
                }
                break;
            }
            case '4':
                printf("Exiting UI.\n");
                exit(0);
                break;
            default:
                printf("Invalid choice.\n");
                break;
        }
    }
    return NULL;
}
