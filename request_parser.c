//
// Created by matyas on 5/2/25.
//

#include "request_parser.h"
#include "defines.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>

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