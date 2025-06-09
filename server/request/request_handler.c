//
// Created by matyas on 4/20/25.
//

#include "request_handler.h"
#include "request_parser.h"
#include "../utils/file_utils.h"
#include "../response/response_handler.h"
#include "../defines.h"
#include "../logger/logger.h"

#include <stdlib.h>
#include <string.h>

typedef int handler_address(const int, const char *, const char *);

typedef struct{
    char method[METHOD_SIZE];
    handler_address *handler;
}http_handlers;

http_handlers handlers[] = {
    {"GET",handle_get},
    {"HEAD", handle_head}
};
int handle_request(const int client_fd, const char* request, const char* root_dir) {
    char method[METHOD_SIZE];
    get_request_method(request, method, sizeof(method));

    for (size_t i = 0; i < sizeof(handlers) / sizeof(handlers[0]); ++i) {
        if (strcmp(method, handlers[i].method) == 0) {
            return handlers[i].handler(client_fd, request, root_dir);
        }
    }

    log_error("Unknown method: %s\n", method);
    return ERROR_GENERIC;
}
int handle_get(const int client_fd, const char* request, const char* root_dir) {
    size_t content_length;
    char* file_content = get_file_content(request, root_dir, &content_length);

    if (!file_content) {
        return send_404_response(client_fd);
    }
    if (send_response(client_fd, request, root_dir, file_content, content_length) < 0) {
        free(file_content);
        return ERROR_GENERIC;
    }
    free(file_content);
    return OK;
}
int handle_head(const int client_fd, const char* request, const char* root_dir) {
    size_t content_length;
    char* file_content = get_file_content(request, root_dir, &content_length);

    if (!file_content) {
        return send_404_response(client_fd);
    }
    if (send_response(client_fd, request, root_dir, 0, content_length) < 0) {
        free(file_content);
        return ERROR_GENERIC;
    }
    free(file_content);
    return OK;
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
