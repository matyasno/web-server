//
// Created by matyas on 5/2/25.
//

#include "net_utils.h"
#include "defines.h"

int get_client_handle(struct Server *server) {
    socklen_t address_length = sizeof(server->address);
    const int client_fd = accept(server->socket, (struct sockaddr *)&server->address, &address_length);


    if (client_fd < 0) {
        perror("Failed to accept a new connection");
        return ERROR_GENERIC;
    }

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(server->address.sin_addr), client_ip, INET_ADDRSTRLEN);
    const int client_port = ntohs(server->address.sin_port);

    printf("Client connected: %s:%d\n", client_ip, client_port);

    return client_fd;
}

int send_client_response(const int client_fd, const char* buff, const size_t buff_size, const int flags) {
    if (send(client_fd, buff, buff_size, flags) < 0) {
        return ERROR_GENERIC;
    }
    return OK;
}