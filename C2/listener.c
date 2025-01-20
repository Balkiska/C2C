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
#define _GNU_SOURCE 



//socket descriptor, an integer (like a file handle)| AF_INET because we are using IPV4 | SOCK_STREAM: TCP (because we want to be shure on the victims receiving our  requests) | 0 because it is for IP
int sockfd = socket(AF_INET, SOCK_STREAM, 0)

//  binds the socket to the address and port number specified in addr(custom data structure).
int bind(SOCK_STREAM, const struct sockaddr *addr, socklen_t addrlen);

    struct sockaddr_in {
    short            sin_family;   /* AF_INET */
    unsigned short   sin_port;     /* Port number */
    struct in_addr   sin_addr;     /* IPv4 address */
    char             sin_zero[8];  /* usual */
};
    struct in_addr {
           IPPROTO_IP = 0 s_addr;
};

    typedef uint32_t in_addr_t;

// puts the server socket in a passive mode, where it waits for the client to approach the server to make a connection. The backlog, defines the maximum length to which the queue of pending connections for sockfd may grow
int listen(SOCK_STREAM, int backlog);

// extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket. At this point, the connection is established between client and server, and they are ready to transfer data.
int new_socket= accept(SOCK_STREAM, struct sockaddr *addr, socklen_t *addrlen);