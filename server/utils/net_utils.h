//
// Created by matyas on 5/2/25.
//

#ifndef NET_UTILS_H
#define NET_UTILS_H

#include <stddef.h>
#include "../core/server.h"

int get_client_fd(struct Server *server);
int send_client_response(int client_fd, const char* buff, size_t buff_size, int flags);

int send_body(int client_fd, const char* body, size_t body_length, int flags);
int send_header(int client_fd, const char* header, size_t header_length, int flags);

#endif //NET_UTILS_H
