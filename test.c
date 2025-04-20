#include "Server.h"
#include <unistd.h>
#include <string.h>

#include "servUtil.h"

#ifdef _WIN32
    #define SHUT_RDWR SD_BOTH
#endif

void launch(struct Server *server) {
    printf("=== SERVER LAUNCHED ===\n");

    while (1) {
        char request[1000] = {0};
        printf("Waiting for connection...\n");

        char* currentWorkingDirectory="../testWeb/\0";
        char currentFilePath[256];

        int address_length = sizeof(server->address);
        int clientSocket = accept(server->socket, (struct sockaddr *)&server->address, (socklen_t *)&address_length);

        if (clientSocket < 0) {
            perror("Failed to accept a new connection");
            continue;
        }

        printf("New connection accepted\n");
        read(clientSocket, request, sizeof(request) - 1);
        printf("\n\nReceived request: %s \n\n", request);

        char *requestedFile = getRequestedFile(request);
        strcpy(currentFilePath, currentWorkingDirectory);
        strcat(currentFilePath, requestedFile);
        free(requestedFile);

        char* htmlContent = getHTMLContent(currentFilePath);

        char response[1000] = {0};
        strcpy(response, "HTTP/1.1 200 OK\r\n"
        "Content-Length: 100\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n");

        strcat(response, htmlContent);
        printf("%s", response);

        send(clientSocket, response, strlen(response), 0);
        printf("Response sent, closing connection.\n");
        shutdown(clientSocket, SHUT_RDWR);
        close(clientSocket);
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


