#include "server.h"
#include <unistd.h>
#include <stdio.h>

#ifdef _WIN32
    #define SHUT_RDWR SD_BOTH
#endif

void launch(struct Server *server) {
    printf("=== SERVER STARTED ===\n");

    while (1) {
        char buffer[30000] = {0};
        printf("Waiting for connection...\n");

        int address_length = sizeof(server->address);
        int new_socket = accept(server->socket, (struct sockaddr *)&server->address, (socklen_t *)&address_length);
        if (new_socket < 0) {
            perror("Failed to accept a new connection");
            continue;
        }

        printf("New connection accepted\n");
        read(new_socket, buffer, sizeof(buffer) - 1);
        printf("Received request: %s\n", buffer);

        char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 38\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<html><body><h1>Hello</h1></body></html>";

        send(new_socket, response, strlen(response), 0);
        printf("Response sent, closing connection.\n");
        shutdown(new_socket, SHUT_RDWR);
        close(new_socket);
    }
}

int main() {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error: %d\n", WSAGetLastError());
        exit(1);
    }
#endif

    struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 8080, 10, launch);
    server.launch(&server);
    return 0;
}
