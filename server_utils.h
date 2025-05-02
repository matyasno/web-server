//
// Created by matyas on 4/20/25.
//

#ifndef SERVUTIL_H
#define SERVUTIL_H

#include "server.h"

int handle_response(int client_fd,const char* request, const char* root_dir);

int handle_get(int client_fd, const char* request, const char* root_dir);
int handle_head(const char* request, const char* root_dir);
int handle_post(const char* request, const char* root_dir);
int handle_put(const char* request, const char* root_dir);
int handle_delete(const char* request, const char* root_dir);
int handle_options(const char* request, const char* root_dir);
int handle_patch(const char* request, const char* root_dir);
int handle_connect(const char* request, const char* root_dir);
int handle_trace(const char* request, const char* root_dir);

#endif //SERVUTIL_H