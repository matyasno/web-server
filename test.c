#include <unistd.h>

#include "request_handler.h"
#include "net_utils.h"
#include "request_parser.h"
#include "defines.h"

#ifdef _WIN32
    #define SHUT_RDWR SD_BOTH
#endif

void launch(struct Server *server) {
    printf("=== SERVER LAUNCHED ===\n");

    while (1) {
        char request[REQUEST_SIZE] = {0};
        const char* root_dir = "../testWeb/";

        printf("Waiting for connection...\n");
        const int client_fd = get_client_handle(server);

        if (client_fd <= 0) {
            continue;
        }

        if (get_client_request(client_fd, request, sizeof(request) - 1) < 0) {
            continue;
        }

        if (handle_response(client_fd, request, root_dir) < 0) {
            continue;
        }

        printf("Response sent, closing connection.\n\n");

        shutdown(client_fd, SHUT_RDWR);
        close(client_fd);
    }
}

int main(const int args, char **argv) {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error: %d\n", WSAGetLastError());
        exit(1);
    }
#endif

    struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 8080, 5, launch);
    server.launch(&server);
    return 0;
}
