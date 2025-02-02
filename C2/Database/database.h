#ifndef DATABASE_H
#define DATABASE_H

#define MAX_LENGTH 256
#define MAX_KEY_LENGTH 4096
#define DB_FILE "credentials.bin"

// store SSH credentials
typedef struct {
    int id;
    char ssh_key[MAX_KEY_LENGTH];
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
    char hostname[MAX_LENGTH];
} Credentials;

//declarations of the functions
int generate_id();
void save_credentials(Credentials *cred);
void load_credentials();
void search_by_username(const char *username);
void delete_by_id(int id);

#endif 
