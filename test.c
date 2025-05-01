#include <unistd.h>
#include <string.h>

#include "servUtil.h"

#ifdef _WIN32
    #define SHUT_RDWR SD_BOTH
#endif

#define RESPONSE_SIZE 5000000
#define REQUEST_SIZE 1000

void launch(struct Server *server) {
    printf("=== SERVER LAUNCHED ===\n");

    while (1) {
        char request[REQUEST_SIZE] = {0};
        char *response = calloc(RESPONSE_SIZE, sizeof(char));
        const char* rootDir = "../testWeb/";

        printf("Waiting for connection...\n");
        const int clientHandle = get_client_handle(server);

        if (clientHandle <= 0) {
            continue;
        }

        if (get_client_request(clientHandle, request, sizeof(request) - 1) < 0) {
            continue;
        }

        if (handle_response(clientHandle, request, rootDir, response, RESPONSE_SIZE) < 0) {
            continue;
        }

        printf("\nResponse sent\n, closing connection.\n");

        free(response);

        shutdown(clientHandle, SHUT_RDWR);
        close(clientHandle);
    }
}

int main(int args, char **argv) {
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