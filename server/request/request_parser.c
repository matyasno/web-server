//
// Created by matyas on 5/2/25.
//

#include "request_parser.h"
#include "../defines.h"
#include "../logger/logger.h"

#include <unistd.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#include <winsock2.h>
#endif

int get_client_request(const int client_fd, char* buff, const size_t buff_size) {
#ifdef WIN32
    const ssize_t bytesRead = recv(client_fd, buff, buff_size, 0);
#else
    const ssize_t bytesRead = read(client_fd, buff, buff_size);
#endif
    if (bytesRead < 0) {
        log_warning("Failed to read request");
        close(client_fd);
        return ERROR_GENERIC;
    } else if (bytesRead == 0) {
        log_info("Client closed the connection before sending data.");
        close(client_fd);
        return OK;
    }

    buff[bytesRead] = '\0';
    return OK;
}
int get_request_method(const char* request, char* buff, const size_t buffSize) {
    char method[METHOD_SIZE], path[PATH_SIZE];

    if (sscanf(request, "%7s %255s", method, path) != 2) {
        log_warning("Invalid request format.");
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
        log_warning("Invalid request format.\n");
        return ERROR_GENERIC;
    }

    const char* requested = (path[0] == '/') ? path + 1 : path;
    if (strlen(requested) == 0) {
        requested = "index.html";
    }

    if (strstr(requested, "..")) {
        log_warning("Blocked suspicious path: %s\n", requested);
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
        log_warning("Full path is too long.\n");
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
    if (strcmp(extension, ".txt") == 0) return "text/plain";
    return "application/octet-stream";
}