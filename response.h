//
// Created by matyas on 5/2/25.
//

#ifndef RESPONSE_H
#define RESPONSE_H

#include <stddef.h>

int send_404_response(int client_fd);
int send_response(const int client_fd, const char* request, const char* root_dir, const char* content, const size_t content_length);

int build_head( const char* protocol, const char* version, const char* status, const char* mime_type, const size_t content_length, const char* connection_type, char* header, size_t header_length );

#endif //RESPONSE_H
