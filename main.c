#include <"stdio.h">
#include <"stdlib.h">
#include <"NETWORK.h">
#include <"NETWORK.c">

int main(){
    int port = 8080;
    int server_fd = make_listening_socket(port);
    printf("Server is listening on port %d\n", port);
    printf("Test command: nc localhost %d\n", port);
    
    int client_fd = accept(server_fd, NULL, NULL);
    if(client_fd < 0){
        perror("accept");
        exit(1);
    }
    printf("Client connected!\n");
    char buffer[1024];
    ssize_t bytes; 
    while((bytes = read(client_fd, buffer, sizeof(buffer) - 1)) > 0){
       write(client_fd, buffer, bytes);
    }

    printf("Client disconnected\n");
    close(client_fd);
    close(server_fd);
    return 0; 

}
