//
// Created by matyas on 5/2/25.
//

#ifndef NET_UTILS_H
#define NET_UTILS_H

#endif //NET_UTILS_H

#include <stddef.h>
#include "server.h"

int get_client_handle(struct Server *server);
int send_client_response(int client_fd, const char* buff, size_t buff_size, int flags);
