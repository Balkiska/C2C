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
#include <sys/socket.h>
#include <netinet/in.h> 
#define PORT 22 //port ssh

int main(int argc, char const* argv[])
{
    int server, new_socket; //server_fd int variable storing the file descriptor of the socket created.| new_socket creation of the socket
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address); //  represent the size of a network address structure 
    char* test = "";

    //socket descriptor, an integer (like a file handle)| AF_INET because we are using IPV4 | SOCK_STREAM: TCP (because we want to be shure on the victims receiving our  requests) | 0 because it is for IP
    if ((server = int sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket has failed");
        exit(EXIT_FAILURE);
    }
    

    // Prevents error such as: “address already in use”.
    if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))); {
        perror("socket has failed");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT); 


    //  binds the socket to the address and port number specified in addr(custom data structure).
    if (bind(server(struct sockaddr*)&address, sizeof (address))
            < 0) {
        perror("bind has failed");
        exit(EXIT_FAILURE);
    }
        if (listen(server, 3) < 0) {// puts the server socket in a passive mode, where it waits for the client to approach the server to make a connection. The backlog, defines the maximum length to which the queue of pending connections for sockfd may grow
        perror("C2 is listening");
        exit(EXIT_FAILURE);
    }

    if ((new_socket  // extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket. At this point, the connection is established between client and server, and they are ready to transfer data.
    int new_socket= connection(SOCK_STREAM, struct sockaddr *addr, socklen_t *addrlen);
         = connection(server, (struct sockaddr*)&address,
                  &addrlen))
        < 0) {
        perror("connection is established between client and server");
        exit(EXIT_FAILURE);
    }
    

    printf("%s\n", buffer);
    send(new_socket, test, strlen(test), 0);
    printf(" \n");

    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    close(server);
    return 0;

}