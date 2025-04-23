//
// Created by matyas on 4/20/25.
//

#ifndef SERVUTIL_H
#define SERVUTIL_H

#endif //SERVUTIL_H

#include "Server.h"

// net shit
int get_client_handle(struct Server *server);
int get_client_request(int clientHandle, char* buff, size_t buffSize);

// parsing requests/building responses
int get_HTML_content(const char* request, const char* rootDir, char* buff, const size_t buffSize);
int get_requested_file(const char* request, char* buff, const size_t buffSize);
int get_file_path(const char* request, const char* currentWorkingDir, char* buff, const size_t buffSize);
int build_response(const char* request, const char* rootDir, char* buff, const size_t buffSize);
