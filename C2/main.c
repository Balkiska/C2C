#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

    //DB
    Credentials new_cred;
    new_cred.id = generate_id();
    strcpy(new_cred.ssh_key, "ssh-rsa AAAAB3...");
    strcpy(new_cred.username, "victim_user");
    strcpy(new_cred.password, "securepassword");
    strcpy(new_cred.hostname, "victim.com");

    save_credentials(&new_cred);

    printf("Stored credentials:\n");
    load_credentials();

    printf("\nSearching for user 'victim_user':\n");
    search_by_username("victim_user");

    printf("\nDeleting ID 1:\n");
    delete_by_id(1);

    printf("\nCredentials after deletion:\n");
    load_credentials();

    return 0;
}
