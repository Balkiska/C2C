#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//generate a unique ID based on file entries
int generate_id() {
    FILE *file = fopen(DB_FILE, "rb");
    int id = 0;
    if (file) {
        fseek(file, 0, SEEK_END);
        id = ftell(file) / sizeof(Credentials);
        fclose(file);
    }
    return id + 1;
}

//save credentials to the binary file
void save_credentials(Credentials *cred) {
    FILE *file = fopen(DB_FILE, "ab");
    if (file) {
        fwrite(cred, sizeof(Credentials), 1, file);
        fclose(file);
        printf("ID: %d\n", cred->id);
    } else {
        perror("Error opening the database file");
    }
}

// load and print all the credentials
void load_credentials() {
    FILE *file = fopen(DB_FILE, "rb");
    Credentials cred;
    if (file) {
        while (fread(&cred, sizeof(Credentials), 1, file)) {
            printf("ID: %d, Username: %s, Hostname: %s\n", cred.id, cred.username, cred.hostname);
        }
        fclose(file);
    } else {
        printf("No credential has been found.\n");
    }
}

// search credential by username
void search_by_username(const char *username) {
    FILE *file = fopen(DB_FILE, "rb");
    Credentials cred;
    if (file) {
        while (fread(&cred, sizeof(Credentials), 1, file)) {
            if (strcmp(cred.username, username) == 0) {
                printf("Found ID: %d, Hostname: %s\n", cred.id, cred.hostname);
            }
        }
        fclose(file);
    } else {
        printf("Database not found.\n");
    }
}

// elete credentials by ID
void delete_by_id(int id) {
    FILE *file = fopen(DB_FILE, "rb");
    FILE *temp = fopen("temp.bin", "wb");
    Credentials cred;
    if (file && temp) {
        while (fread(&cred, sizeof(Credentials), 1, file)) {
            if (cred.id != id) {
                fwrite(&cred, sizeof(Credentials), 1, temp);
            }
        }
        fclose(file);
        fclose(temp);
        remove(DB_FILE);
        rename("temp.bin", DB_FILE);
        printf("Credential with ID %d deleted.\n", id);
    } else {
        printf("Error processing the database.\n");
    }
}
