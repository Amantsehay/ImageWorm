#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>

int main() {
    // The functionality of this server is to listen for incoming connections 
    int sock, client_socket;
    char buffer[1024];
    char response[18384];
    struct sockaddr_in server_address, client_address;

    int i = 0;
    int optval = 1;
    socklen_t client_length;

    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Error creating socket\n");
        return 1;
    }

    // Set socket options
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        printf("Error setting TCP socket options\n");
        return 1;
    }

    // Configure server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("10.6.157.138");
    server_address.sin_port = htons(40000);

    // Bind the socket to the address and port
    if (bind(sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        printf("Error binding socket\n");
        return 1;
    }

    // Listen for incoming connections
    if (listen(sock, 5) < 0) {
        printf("Error listening on socket\n");
        return 1;
    }

    printf("Server listening on port 40000\n");

    // Accept a connection from a client
    client_length = sizeof(client_address);
    client_socket = accept(sock, (struct sockaddr *) &client_address, &client_length);
    if (client_socket < 0) {
        printf("Error accepting connection\n");
        return 1;
    }

    printf("Connection accepted from %s:%d\n",
           inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

    // Main communication loop
    while (1) {
        bzero(buffer, sizeof(buffer));
        bzero(response, sizeof(response));
        printf("* Shell#%s~$", inet_ntoa(client_address.sin_addr));
        fgets(buffer, sizeof(buffer), stdin);
        strtok(buffer, "\n");
        write(client_socket, buffer, strlen(buffer) + 1);
        if (strcmp(buffer, "q") == 0) {
            break;
        } else {
            recv(client_socket, response, sizeof(response), 0);
            printf("%s", response);
        }
    }

    // Close the sockets
    close(client_socket);
    close(sock);

    return 0;
}
