//
// Created by matyas on 4/20/25.
//

#include "server_utils.h"
#include "request_parser.h"
#include "file_utils.h"

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
const char* get_mime_type(const char* request, const char* rootDir) {
    char path[PATH_SIZE];
    get_file_path(request, rootDir, path, sizeof(path));
    const char* extension = strrchr(path, '.');
    if (!extension) return "application/octet-stream";
    if (strcmp(extension, ".html") == 0) return "text/html";
    if (strcmp(extension, ".css") == 0) return "text/css";
    if (strcmp(extension, ".js") == 0) return "application/javascript";
    if (strcmp(extension, ".png") == 0) return "image/png";
    if (strcmp(extension, ".gif") == 0) return "image/gif";
    if (strcmp(extension, ".jpg") == 0) return "image/jpeg";
    if (strcmp(extension, ".jpeg") == 0) return "image/jpeg";
    return "application/octet-stream";
}
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
int send_body(const int client_fd, const char* body, const size_t body_length,const int flags) {
    if (send_client_response(client_fd, body, body_length, flags) < 0) {
        return ERROR_GENERIC;
    }
    return OK;
}
int send_header(const int client_fd, const char* header, const size_t header_length,const int flags) {
    if (send_client_response(client_fd, header, header_length, flags) < 0) {
        return ERROR_GENERIC;
    }
    return OK;
}
int send_404_response(const int client_fd) {
    const char* body = "<html><body><h1>404 Not Found</h1><a href='index.html'>Go home</a></body></html>";
    char header[HEADER_SIZE];
    const int header_length = snprintf(header, sizeof(header),
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Length: %zu\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n", strlen(body));

    if (header_length >= sizeof(header)) {
        return ERROR_OVERFLOW;
    }

    if (send_header(client_fd, header, strlen(header), 0) < 0) {
        return ERROR_GENERIC;
    }

    if (send_body(client_fd, body, strlen(body), 0) < 0) {
        return ERROR_GENERIC;
    }

    return OK;
}
int send_file_response(const int client_fd, const char* request, const char* root_dir, const char* content, const size_t content_length) {
    char header[HEADER_SIZE];
    const char* mime_type = get_mime_type(request, root_dir);

    const int header_length = snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: %zu\r\n"
        "Content-Type: %s\r\n"
        "Connection: close\r\n"
        "\r\n", content_length, mime_type);

    if (header_length >= sizeof(header)) {
        return ERROR_OVERFLOW;
    }

    if (send_header(client_fd, header, strlen(header), 0) < 0) {
        return ERROR_GENERIC;
    }

    if (send_body(client_fd, content, content_length, 0) < 0) {
        return ERROR_GENERIC;
    }

    return OK;
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
