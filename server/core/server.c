#include "server.h"

#include "../defines.h"
#include "../utils/net_utils.h"
#include "../request/request_handler.h"
#include "../request/request_parser.h"
#include "../arguments/args.h"
#include "../logger/logger.h"

#include <unistd.h>
#include <stdlib.h>
#include <memory.h>

#ifndef _WIN32
#include <arpa/inet.h>
#include <netdb.h>
#endif

struct Server create_server(const int service, const int protocol, const char* server_interface, const char* port, const int backlog) {
    struct Server server;
    server.service = service;
    server.protocol = protocol;
    server.server_interface = server_interface;
    server.port = port;
    server.backlog = backlog;

    if (get_address_family(&server) != OK) {
        log_error("Failed to get address family");
        exit(EXIT_FAILURE);
    }

    if (init_address(&server) != OK) {
        log_error("Failed to initialize address");
        exit(EXIT_FAILURE);
    }

    if (create_socket(&server) != OK) {
        log_error("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server.socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(server.socket, (struct sockaddr*)&server.address, server.address_len) < OK) {
        log_error("Failed to bind socket");
        exit(EXIT_FAILURE);
    }

    if (listen(server.socket, server.backlog) < OK) {
        log_error("Failed to start listening");
        exit(EXIT_FAILURE);
    }

    log_info("SERVER CREATED");
    return server;
}


void start_http_server(struct Server *server, const char* root_dir) {
    log_info("SERVER STARTED");

    while (1) {
        char request[REQUEST_SIZE] = {0};
        const int client_fd = get_client_handle(server);

        if (client_fd <= 0) {
            continue;
        }

        if (get_client_request(client_fd, request, sizeof(request) - 1) < 0) {
            continue;
        }

        log_debug("REQUEST RECEIVED");

        if (handle_request(client_fd, request, root_dir) < 0) {
            continue;
        }

        log_debug("RESPONSE SENT");
        log_debug("CLOSING CONNECTION");
        shutdown(client_fd, SHUT_RDWR);
        close(client_fd);
    }
}

int init_address(struct Server *server) {
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
        log_error("Unsupported address family");
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
        log_error("getaddrinfo: %s", gai_strerror(err));
        return ERROR_GENERIC;
    }
    server->family = res->ai_family;
    freeaddrinfo(res);
    return OK;
}

int create_socket(struct Server *server) {
    server->socket = socket(server->family, server->service, server->protocol);
    if (server->socket < 0) {
        return ERROR_GENERIC;
    }
    return OK;
}