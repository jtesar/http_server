#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define RESPONSE_SIZE 4096
#define ROOT_DIR "/var/www/html"

void handle_request(int new_socket) {
    char buffer[BUFFER_SIZE] = {0};
    char response[RESPONSE_SIZE] = {0};

    // Read the request from the client
    read(new_socket, buffer, BUFFER_SIZE);
    printf("Received request:\n%s\n", buffer);

    // Extract the requested file name
    char *filename = strtok(buffer, " ");
    filename = strtok(NULL, " ");
    char filepath[100];
    sprintf(filepath, "%s%s", ROOT_DIR, filename);

    // Open the requested file
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        // File not found, send a 404 response
        sprintf(response, "HTTP/1.1 404 Not Found\nContent-Type: text/plain\n\nFile not found");
    } else {
        // File found, read its contents
        char file_contents[RESPONSE_SIZE];
        memset(file_contents, 0, sizeof(file_contents));
        fread(file_contents, sizeof(char), sizeof(file_contents), file);
        fclose(file);

        // Send the file contents as the response
        sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %zu\n\n%s",
                strlen(file_contents), file_contents);
    }

    // Send the response to the client
    send(new_socket, response, strlen(response), 0);
    printf("Response sent\n");
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        // Accept incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        // Handle the client request
        handle_request(new_socket);

        // Close the connection
        close(new_socket);
    }

    close(server_fd);

    return 0;
}

