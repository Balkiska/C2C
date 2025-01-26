/* References:
https://www.geeksforgeeks.org/socket-programming-cc/
https://man7.org/linux/man-pages/man7/socket.7.html
https://man7.org/linux/man-pages/man2/socket.2.html
https://man7.org/linux/man-pages/man2/bind.2.html
https://tala-informatique.fr/index.php?title=C_socket

Help:
Socket programming is a way of connecting two nodes on a network to communicate with each other. */ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <pthread.h>
#define PORT 22 //port ssh


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