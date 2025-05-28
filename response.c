//
// Created by matyas on 5/2/25.
//

#include "response.h"
#include "defines.h"
#include "net_utils.h"
#include "request_parser.h"

#include <string.h>
#include <stdio.h>

const char* status_types[] = {
    "200 OK",
    "404 Not Found"
};

const char* connection_types[] = {
    "close",
    "keep-alive"
};

int send_response(const int client_fd, const char* request, const char* root_dir, const char* content, const size_t content_length) {
    char header[HEADER_SIZE];
    const char* mime_type = get_mime_type(request, root_dir);

    if (build_head("HTTP", "1.1", "200 OK", mime_type, content_length, "close", header, HEADER_SIZE) != OK) {
        return ERROR_GENERIC;
    }
    if (send_header(client_fd, header, strlen(header), 0) != OK) {
        return ERROR_GENERIC;
    }
    if (send_body(client_fd, content, content_length, 0) != OK) {
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

int build_head( const char* protocol, const char* version, const char* status, const char* mime_type, const size_t content_length, const char* connection_type, char* header, size_t header_length ) {
    if (snprintf(header, header_length,
        "%s/%s %s\r\n"
        "Content-Length: %zu\r\n"
        "Content-Type: %s\r\n"
        "Connection: %s\r\n"
        "\r\n", protocol, version, status, content_length, mime_type, connection_type) >= (int)header_length) {
        return ERROR_OVERFLOW;
    }

    return OK;
}