//
// Created by matyas on 5/2/25.
//

#ifndef RESPONSE_H
#define RESPONSE_H

#include <stddef.h>

int send_404_response(int client_fd);
int send_file_response(int client_fd, const char* request, const char* root_dir, const char* content, size_t content_length);

#endif //RESPONSE_H
