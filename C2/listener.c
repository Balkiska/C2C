/*Test the code :

gcc listener.c -o server -pthread
sudo ./server
On another terminal -> nc 127.0.0.1 22
*should not work : error message "Connection refused for 127.0.0.1 (port not unlocked)."

                ->  nc 127.0.0.1 1111
                    nc 127.0.0.1 11
                    nc 127.0.0.1 2222
                    nc 127.0.0.1 222
                    nc 127.0.0.1 22
*notifiction "New client connected."
You can now write anything you want from one to the other terminal. 

On a third terminal->   nc 127.0.0.1 1111
                        nc 127.0.0.1 11
                        nc 127.0.0.1 2222
                        nc 127.0.0.1 222
                        nc 127.0.0.1 22

*notifiction "New client connected."
You can now write anything you want from one to the third terminal.


*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <pthread.h>
#include <arpa/inet.h>
#include <time.h>
#define PORT 22 //port ssh


/*-----------
Port knocking
-----------*/

#define MAX_CLIENTS 666
#define SEQUENCE_LEN 5

int port_knocking_sequence[SEQUENCE_LEN] = {1111, 11, 2222, 222, 22}; 

struct PortKnockingState {
    char ip[INET_ADDRSTRLEN];
    int sequence;
    time_t last_knocking;
    int unlocked;
};

struct PortKnockingState clients[MAX_CLIENTS];
int client_count = 0;

//manage the port knocking sequence
void process_knocking(const char *ip, int port) {
    time_t now = time(NULL);

    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].ip, ip) == 0) {
            if (clients[i].unlocked) {
                printf("Port already unlocked for %s\n", ip);
                return;
            }
            if (port == port_knocking_sequence[clients[i].sequence]) {
                if (now - clients[i].last_knocking > 5) {
                    clients[i].sequence = 0;  // Reset if needed
                }
                clients[i].sequence++;

                if (clients[i].sequence == SEQUENCE_LEN) {
                    printf("SCorrect sequence received from %s\n", ip);
                    printf("SSH port is unlocked for %s\n", ip);
                    clients[i].unlocked = 1; // port open for this ip
                    clients[i].sequence = 0;  // Reinitialize
                }
                clients[i].last_knocking = now;
            } else {
                clients[i].sequence = 0;  // Reset if wrong port
            }
            return;
        }
    }
    //for a new client
    strcpy(clients[client_count].ip, ip);
    clients[client_count].sequence = (port == port_knocking_sequence[0]) ? 1 : 0;
    clients[client_count].last_knocking = now;
    clients[client_count].unlocked = 0;
    client_count++;
}

/*--------------------------
Thread containing the socket
--------------------------*/

void* handle_client(void* arg) {
    int new_socket = *((int*)arg);
    free(arg);

    char buffer[1024] = {0};
    char* response = "\n";

    read(new_socket, buffer, sizeof(buffer));
    printf(" %s\n", buffer);
    send(new_socket, response, strlen(response), 0);

    close(new_socket);
    return NULL;
}

int main(int argc, char const* argv[])
{
    int server, new_socket; //server_fd int variable storing the file descriptor of the socket created.| new_socket creation of the socket
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address); //  represent the size of a network address structure 


    //socket descriptor, an integer (like a file handle)| AF_INET because we are using IPV4 | SOCK_STREAM: TCP (because we want to be shure on the victims receiving our  requests) | 0 because it is for IP
        if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation has failed");
        exit(EXIT_FAILURE);
    }
    

    // Prevents error such as: “address already in use”.
    if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("socket has failed");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT); 


    //  binds the socket to the address and port number specified in addr(custom data structure).
    if (bind(server, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind has failed");
        exit(EXIT_FAILURE);
    }
        if (listen(server, 3) < 0) {// puts the server socket in a passive mode, where it waits for the client to approach the server to make a connection. The backlog, defines the maximum length to which the queue of pending connections for sockfd may grow
        perror("C2 failed listening");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket. At this point, the connection is established between client and server, and they are ready to transfer data.
    while (1) {
        int *new_socket = (int *)malloc(sizeof(int));
        if ((*new_socket = accept(server, (struct sockaddr*)&address, &addrlen)) < 0) {
            perror("Accept failed");
            free(new_socket);
            continue;
        }

        //retrieve IP address and check knocking
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &address.sin_addr, client_ip, INET_ADDRSTRLEN);
        int client_port = ntohs(address.sin_port);

        process_knocking(client_ip, client_port);

        // checks if the IP is unlocked before doing the connection
        int is_unlocked = 0;
        for (int i = 0; i < client_count; i++) {
            if (strcmp(clients[i].ip, client_ip) == 0 && clients[i].unlocked) {
                is_unlocked = 1;
                break;
            }
        }

        if (!is_unlocked) {
            printf("Connection refused for %s (port not unlocked).\n", client_ip);
            close(*new_socket);
            free(new_socket);
            continue; 
        }
        
        printf("New client connected.\n");

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void*)new_socket) != 0) {
            perror("pthread_create failed");
            free(new_socket);
        }

        pthread_detach(thread_id);  //allow thread to clean up after finishing
    }

    close(server);
    return 0;
}