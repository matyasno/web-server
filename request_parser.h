//
// Created by matyas on 5/2/25.
//

#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include <stddef.h>

int get_client_request(int client_fd, char* buff, size_t buff_size);
int get_request_method(const char* request, char* buff, size_t buffSize);
int get_requested_file(const char* request, char* buff, size_t buff_size);
int get_file_path(const char* request, const char* currentWorkingDir, char* buff, size_t buffSize);

#endif //REQUEST_PARSER_H
