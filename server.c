#include "server.h"

#include "defines.h"
#include "net_utils.h"
#include "request_handler.h"
#include "request_parser.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

struct Server server_constructor(int domain, int service, int protocol, u_long server_interface, int port, int backlog) {
    struct Server server;
    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.server_interface = server_interface;
    server.port = port;
    server.backlog = backlog;

    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = htonl(server_interface);

    server.socket = socket(domain, service, protocol);
    if (server.socket == 0) {
        perror("Failed to create socket");
        exit(1);
    }

    if (bind(server.socket, (struct sockaddr *)&server.address, sizeof(server.address)) < 0) {
        perror("Failed to bind socket");
        exit(1);
    }

    if (listen(server.socket, server.backlog) < 0) {
        perror("Failed to start listening");
        exit(1);
    }

    return server;
}

void start_http_server(struct Server *server, const char* root_dir) {
    printf("=== SERVER LAUNCHED ===\n");
    printf("Web directory: %s\n", root_dir);

    while (1) {
        char request[REQUEST_SIZE] = {0};

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
