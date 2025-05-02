//
// Created by matyas on 4/20/25.
//

#include "request_handler.h"
#include "request_parser.h"
#include "file_utils.h"
#include "response.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "net_utils.h"

#define OK 0
#define ERROR_GENERIC (-1)
#define ERROR_NOT_FOUND (-2)
#define ERROR_OVERFLOW (-3)

#define METHOD_SIZE 8
#define PATH_SIZE 1024
#define HEADER_SIZE 1024

typedef int handler_address(const int, const char *, const char *);

typedef struct{
    char method[METHOD_SIZE];
    handler_address *handler;
}http_handlers;

http_handlers handlers[] = {
    {"GET",handle_get}
};
int handle_response(const int client_fd, const char* request, const char* root_dir) {
    char method[METHOD_SIZE];
    get_request_method(request, method, sizeof(method));

    for (size_t i = 0; i < sizeof(handlers) / sizeof(handlers[0]); ++i) {
        if (strcmp(method, handlers[i].method) == 0) {
            return handlers[i].handler(client_fd, request, root_dir);
        }
    }

    fprintf(stderr, "Unknown method: %s\n", method);
    return ERROR_GENERIC;
}
int handle_get(const int client_fd, const char* request, const char* root_dir) {
    size_t content_length;
    char* file_content = get_file_content(request, root_dir, &content_length);

    if (!file_content) {
        return send_404_response(client_fd);
    }
    if (send_file_response(client_fd, request, root_dir, file_content, content_length) < 0) {
        free(file_content);
        return ERROR_GENERIC;
    }
    free(file_content);
    return OK;
}
int handle_head(const char* request, const char* root_dir) {
    return ERROR_GENERIC;
}
int handle_post(const char* request, const char* root_dir) {
    return ERROR_GENERIC;
}
int handle_put(const char* request, const char* root_dir) {
    return ERROR_GENERIC;
}
int handle_delete(const char* request, const char* root_dir) {
    return ERROR_GENERIC;
}
int handle_options(const char* request, const char* root_dir) {
    return ERROR_GENERIC;
}
int handle_patch(const char* request, const char* root_dir) {
    return ERROR_GENERIC;
}
int handle_connect(const char* request, const char* root_dir) {
    return ERROR_GENERIC;
}
int handle_trace(const char* request, const char* root_dir) {
    return ERROR_GENERIC;
}
