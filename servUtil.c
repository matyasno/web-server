//
// Created by matyas on 4/20/25.
//

#include "servUtil.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OK 0
#define ERROR_GENERIC -1
#define ERROR_NOT_FOUND -2
#define ERROR_OVERFLOW -3

#define METHOD_SIZE 8
#define CONTENT_SIZE 5000
#define PATH_SIZE 1024

typedef int handler_address(const char *, const char *, char *, const size_t);

typedef struct{
    char method[METHOD_SIZE];
    handler_address *handler;
}html_handlers;

html_handlers handlers[] = {
    {"HEAD",handle_head},
    {"GET",handle_get},
    {"POST",handle_post},
    {"DELETE",handle_delete},
    {"PATCH",handle_patch},
    {"PUT",handle_put},
    {"TRACE",handle_trace},
    {"CONNECT",handle_connect},
    {"OPTIONS",handle_options}
};

int get_client_handle(struct Server *server) {
    socklen_t address_length = sizeof(server->address);
    const int clientSocket = accept(server->socket, (struct sockaddr *)&server->address, &address_length);

    if (clientSocket < 0) {
        perror("Failed to accept a new connection");
        return ERROR_GENERIC;
    }

    printf("Client connected successfully\n");
    return clientSocket;
}
int get_client_request(const int clientHandle, char* buff, const size_t buffSize) {
    const ssize_t bytesRead = read(clientHandle, buff, buffSize);

    if (bytesRead < 0) {
        perror("Failed to read request");
        close(clientHandle);
        return ERROR_GENERIC;
    } else if (bytesRead == 0) {
        printf("Client closed the connection before sending data.\n");
        close(clientHandle);
        return OK;
    }

    buff[bytesRead] = '\0';
    return OK;
}

int get_requested_file(const char* request, char* buff, const size_t buffSize) {
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
    if (neededSize > buffSize) {
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
        return -1;
    }

    if (snprintf(buff, buffSize, "%s%s", currentWorkingDir, reqFile) >= (int)buffSize) {
        fprintf(stderr, "Full path is too long.\n");
        return ERROR_OVERFLOW;
    }

    return OK;
}
char* get_file_content(const char* request, const char* rootDir) {
    char path[PATH_SIZE];
    if (get_file_path(request, rootDir, path, sizeof(path)) != 0) {
        return NULL;
    }

    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        printf("File not found: %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* content = malloc(size+1);
    if (content == NULL) {
        perror("Failed to allocate memory");
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(content, 1, size, file);

    if (bytesRead != size) {
        perror("Failed to read file");
        fclose(file);
        free(content);
        return NULL;
    }

    content[size] = '\0';

    fclose(file);
    return content;
}
int build_response(const char* request, const char* rootDir, char* buff, const size_t buffSize) {
    char method[METHOD_SIZE];
    get_request_method(request, method, sizeof(method));

    for (size_t i = 0; i < sizeof(handlers) / sizeof(handlers[0]); ++i) {
        if (strcmp(method, handlers[i].method) == 0) {
            return handlers[i].handler(request, rootDir, buff, buffSize);
        }
    }

    fprintf(stderr, "Unknown method: %s\n", method);
    return ERROR_GENERIC;
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

int handle_get(const char* request, const char* rootDir, char* buff, const size_t buffSize) {
    char* file_content = get_file_content(request, rootDir);
    int header_length;
    if (file_content == NULL) {
        const char* body = "<html><body><h1>404 Not Found</h1><a href='index.html'>Go home</a></body></html>";
        header_length = snprintf(buff, buffSize,
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Length: %zu\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n%s", strlen(body), body);
        return (header_length >= 0 && (size_t)header_length  < buffSize) ? OK : ERROR_OVERFLOW;
    } else {
        size_t contentLength = strlen(file_content);
        const char* mime_type = get_mime_type(request, rootDir);

        header_length = snprintf(buff, buffSize,
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: %zu\r\n"
            "Content-Type: %s\r\n"
            "Connection: close\r\n"
            "\r\n", contentLength, mime_type);

        if (header_length < 0 || (size_t)header_length >= buffSize) {
            free(file_content);
            return ERROR_OVERFLOW;
        }

        if ((size_t)header_length + contentLength >= buffSize) {
            free(file_content);
            return ERROR_OVERFLOW;
        }

        memcpy(buff + header_length, file_content, contentLength);

        free(file_content);
        return OK;

    }
    return ERROR_GENERIC;
}
int handle_head(const char* request, const char* rootDir, char* buff, const size_t buffSize) {
    return ERROR_GENERIC;
}
int handle_post(const char* request, const char* rootDir, char* buff, const size_t buffSize) {
    return ERROR_GENERIC;
}
int handle_put(const char* request, const char* rootDir, char* buff, const size_t buffSize) {
    return ERROR_GENERIC;
}
int handle_delete(const char* request, const char* rootDir, char* buff, const size_t buffSize) {
    return ERROR_GENERIC;
}
int handle_options(const char* request, const char* rootDir, char* buff, const size_t buffSize) {
    return ERROR_GENERIC;
}
int handle_patch(const char* request, const char* rootDir, char* buff, const size_t buffSize) {
    return ERROR_GENERIC;
}
int handle_connect(const char* request, const char* rootDir, char* buff, const size_t buffSize) {
    return ERROR_GENERIC;
}
int handle_trace(const char* request, const char* rootDir, char* buff, const size_t buffSize) {
    return ERROR_GENERIC;
}
