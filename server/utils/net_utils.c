//
// Created by matyas on 5/2/25.
//

#include "net_utils.h"
#include "../defines.h"

#include <stdio.h>

int get_client_handle(struct Server *server) {
    socklen_t address_length = sizeof(server->address);
    const int client_fd = accept(server->socket, (struct sockaddr *)&server->address, &address_length);

    if (client_fd < 0) {
        perror("Failed to accept a new connection");
        return ERROR_GENERIC;
    }

    return client_fd;
}

int send_client_response(const int client_fd, const char* buff, const size_t buff_size, const int flags) {
    if (send(client_fd, buff, buff_size, flags) < 0) {
        return ERROR_GENERIC;
    }
    return OK;
}

int send_body(const int client_fd, const char* body, const size_t body_length,const int flags) {
    if (send_client_response(client_fd, body, body_length, flags) < 0) {
        return ERROR_GENERIC;
    }
    return OK;
}

int send_header(const int client_fd, const char* header, const size_t header_length,const int flags) {
    if (send_client_response(client_fd, header, header_length, flags) < 0) {
        return ERROR_GENERIC;
    }
    return OK;
}