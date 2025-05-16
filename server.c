#include "server.h"

#include "defines.h"
#include "net_utils.h"
#include "request_handler.h"
#include "request_parser.h"
#include "args_parser.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <memory.h>

struct Server server_constructor(int service, const int protocol, const char* server_interface, const char* port, int backlog) {
    struct Server server;
    server.service = service;
    server.protocol = protocol;
    server.server_interface = server_interface;
    server.port = port;
    server.backlog = backlog;

    if (get_address_family(&server) != OK) {
        perror("Failed to get address family");
        exit(EXIT_FAILURE);
    }

    if (init_socket_address(&server) != OK) {
        perror("Failed to initialize socket address");
        exit(EXIT_FAILURE);
    }

    server.socket = socket(server.family, server.service, server.protocol);
    if (server.socket < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    if (bind(server.socket, (struct sockaddr*)&server.address, server.address_len) < OK) {
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }

    if (listen(server.socket, backlog) < OK) {
        perror("Failed to start listening");
        exit(EXIT_FAILURE);
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

int init_socket_address(struct Server *server) {
    if (server->family == AF_INET) {
        struct sockaddr_in *addr = (struct sockaddr_in*)&server->address;
        addr->sin_family = AF_INET;
        addr->sin_port = htons(parse_port(server->port));
        inet_pton(AF_INET, server->server_interface, &addr->sin_addr);
        server->address_len = sizeof(struct sockaddr_in);
    } else if (server->family == AF_INET6) {
        struct sockaddr_in6 *addr6 = (struct sockaddr_in6*)&server->address;
        addr6->sin6_family = AF_INET6;
        addr6->sin6_port = htons(parse_port(server->port));
        inet_pton(AF_INET6, server->server_interface, &addr6->sin6_addr);
        server->address_len = sizeof(struct sockaddr_in6);
    } else {
        fprintf(stderr, "Unsupported domain\n");
        return ERROR_GENERIC;
    }
    return OK;
}

int get_address_family(struct Server *server) {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int err = getaddrinfo(server->server_interface, server->port, &hints, &res);
    if (err != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
        return ERROR_GENERIC;
    }
    server->family = res->ai_family;
    freeaddrinfo(res);
    return OK;
}