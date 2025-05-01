//
// Created by matyas on 4/20/25.
//

#include "servUtil.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
}html_handlers;

html_handlers handlers[] = {
    {"GET",handle_get}
};

int get_client_handle(struct Server *server) {
    socklen_t address_length = sizeof(server->address);
    const int client_fd = accept(server->socket, (struct sockaddr *)&server->address, &address_length);


    if (client_fd < 0) {
        perror("Failed to accept a new connection");
        return ERROR_GENERIC;
    }

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(server->address.sin_addr), client_ip, INET_ADDRSTRLEN);
    const int client_port = ntohs(server->address.sin_port);

    printf("Client connected: %s:%d\n", client_ip, client_port);

    return client_fd;
}
int get_client_request(const int client_fd, char* buff, const size_t buff_size) {
    const ssize_t bytesRead = read(client_fd, buff, buff_size);

    if (bytesRead < 0) {
        perror("Failed to read request");
        close(client_fd);
        return ERROR_GENERIC;
    } else if (bytesRead == 0) {
        printf("Client closed the connection before sending data.\n");
        close(client_fd);
        return OK;
    }

    buff[bytesRead] = '\0';
    return OK;
}

int send_client_response(const int client_fd, const char* buff, const size_t buff_size, const int flags) {
    if (send(client_fd, buff, buff_size, flags) < 0) {
        return ERROR_GENERIC;
    }
    return OK;
}

int get_requested_file(const char* request, char* buff, const size_t buff_size) {
    char method[METHOD_SIZE], path[PATH_SIZE];

    if (sscanf(request, "%7s %255s", method, path) != 2) {
        fprintf(stderr, "Invalid request format.\n");
        return ERROR_GENERIC;
    }

    const char* requested = (path[0] == '/') ? path + 1 : path;
    if (strlen(requested) == 0) {
        requested = "index.html";
    }

    if (strstr(requested, "..")) {
        fprintf(stderr, "Blocked suspicious path: %s\n", requested);
        return ERROR_GENERIC;
    }

    size_t neededSize = strlen(requested) + 1;
    if (neededSize > buff_size) {
        return ERROR_OVERFLOW;
    }

    strcpy(buff, requested);
    return OK;
}
int get_request_method(const char* request, char* buff, const size_t buffSize) {
    char method[METHOD_SIZE], path[PATH_SIZE];

    if (sscanf(request, "%7s %255s", method, path) != 2) {
        fprintf(stderr, "Invalid request format.\n");
        return ERROR_GENERIC;
    }
    if (buffSize < strlen(method) + 1) {
        return ERROR_OVERFLOW;
    }
    strcpy(buff, method);
    return OK;
}
int get_file_path(const char* request, const char* currentWorkingDir, char* buff, const size_t buffSize) {
    char reqFile[PATH_SIZE];

    if (get_requested_file(request, reqFile, sizeof(reqFile)) != 0) {
        return ERROR_GENERIC;
    }

    if (snprintf(buff, buffSize, "%s%s", currentWorkingDir, reqFile) >= (int)buffSize) {
        fprintf(stderr, "Full path is too long.\n");
        return ERROR_OVERFLOW;
    }

    return OK;
}
char* get_file_content(const char* request, const char* root_dir, size_t* outSize) {
    char path[PATH_SIZE];
    if (get_file_path(request, root_dir, path, sizeof(path)) != 0) {
        return nullptr;
    }

    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        printf("File not found: %s\n", path);
        return nullptr;
    }

    fseek(file, 0, SEEK_END);
    const long size = ftell(file);
    rewind(file);
    *outSize = size;

    char* content = malloc(size+1);
    if (content == NULL) {
        perror("Failed to allocate memory");
        fclose(file);
        return nullptr;
    }

    const size_t bytesRead = fread(content, 1, size, file);

    if (bytesRead != size) {
        perror("Failed to read file");
        fclose(file);
        free(content);
        return nullptr;
    }

    content[size] = '\0'; //nemuzu pouzit u bin souboru

    fclose(file);
    return content;
}

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
    int header_length = snprintf(header, sizeof(header),
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Length: %zu\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n%s", strlen(body), body);

    if (header_length >= sizeof(header)) {
        return ERROR_OVERFLOW;
    }

    if (send_header(client_fd, header, strlen(header), 0) < 0) {
        return ERROR_GENERIC;
    }

    return OK;
}
int send_file_response(const int client_fd, const char* request, const char* root_dir, const char* content, const size_t content_length) {
    const size_t contentLength = strlen(content);
    char header[HEADER_SIZE];
    const char* mime_type = get_mime_type(request, root_dir);

    const int header_length = snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: %zu\r\n"
        "Content-Type: %s\r\n"
        "Connection: close\r\n"
        "\r\n", contentLength, mime_type);

    if (header_length >= sizeof(header)) {
        return ERROR_OVERFLOW;
    }

    if (header_length + contentLength >= sizeof(header)) {
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
    } else {
        if (send_file_response(client_fd, request, root_dir, file_content, content_length) < 0) {
            free(file_content);
            return ERROR_GENERIC;
        }
        free(file_content);
        return OK;
    }
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
