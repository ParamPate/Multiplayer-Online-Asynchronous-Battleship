#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "network.h"

int make_listen_socket(int port) {
int fd = socket(AF_INET, SOCK_STREAM, 0); //creates a socket for listening, IPv4, TCP connection 

if(fd < 0) {
    perror("socket");
    exit(1);
}   

int opt = 1; 
setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); //preventing "Address already in use" error 
struct sockaddr_in addr;
memset(&addr, 0, sizeof(addr)); //cleans memory for addr
addr.sin_family = AF_INET;
addr.sin_addr.s_addr = INADDR_ANY;
addr.sin_port = htons(port);

if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    perror("bind");
    exit(1);
}
if(listen(fd, 16) < 0) {
    perror("listen");
    exit(1);
}
return fd;
}