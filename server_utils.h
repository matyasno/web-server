//
// Created by matyas on 4/20/25.
//

#ifndef SERVUTIL_H
#define SERVUTIL_H

#endif //SERVUTIL_H

#include "server.h"

// net shit
int get_client_request(int client_fd, char* buff, size_t buff_size);

// parsing requests/building responses
char* get_file_content(const char* request, const char* root_dir, size_t* outSize);
int get_requested_file(const char* request, char* buff, size_t buff_size);
int get_file_path(const char* request, const char* currentWorkingDir, char* buff, size_t buffSize);
int get_request_method(const char* request, char* buff, size_t buffSize);
int handle_response(int client_fd,const char* request, const char* root_dir);
const char* get_mime_type(const char* request, const char* rootDir);
int send_404_response(int client_fd);
int send_file_response(int client_fd, const char* request, const char* root_dir, const char* content, size_t content_length);
int send_body(int client_fd, const char* body, size_t body_length, int flags);
int send_header(int client_fd, const char* header, size_t header_length, int flags);

// method handling
int handle_get(int client_fd, const char* request, const char* root_dir);
int handle_head(const char* request, const char* root_dir);
int handle_post(const char* request, const char* root_dir);
int handle_put(const char* request, const char* root_dir);
int handle_delete(const char* request, const char* root_dir);
int handle_options(const char* request, const char* root_dir);
int handle_patch(const char* request, const char* root_dir);
int handle_connect(const char* request, const char* root_dir);
int handle_trace(const char* request, const char* root_dir);

