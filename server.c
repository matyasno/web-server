#include "server.h"

struct Server server_constructor(int domain, int service, int protocol, u_long server_interface, int port, int backlog, void (*launch)(struct Server *server)) {
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

    server.launch = launch;
    return server;
}
