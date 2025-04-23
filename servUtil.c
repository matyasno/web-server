//
// Created by matyas on 4/20/25.
//

#include "servUtil.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int get_client_handle(struct Server *server) {
    socklen_t address_length = sizeof(server->address);
    const int clientSocket = accept(server->socket, (struct sockaddr *)&server->address, &address_length);

    if (clientSocket < 0) {
        perror("Failed to accept a new connection");
        return 0;
    }

    printf("Client connected successfully\n");
    return clientSocket;
}

int get_client_request(const int clientHandle, char* buff, const size_t buffSize) {
    const ssize_t bytesRead = read(clientHandle, buff, buffSize);

    if (bytesRead < 0) {
        perror("Failed to read request");
        close(clientHandle);
        return 1;
    } else if (bytesRead == 0) {
        printf("Client closed the connection before sending data.\n");
        close(clientHandle);
        return 0;
    }

    buff[bytesRead] = '\0';
    return 0;
}

int get_requested_file(const char* request, char* buff, const size_t buffSize) {
    char method[8], path[256];

    if (sscanf(request, "%7s %255s", method, path) != 2) {
        fprintf(stderr, "Invalid request format.\n");
        return -1;
    }

    const char* requested = (path[0] == '/') ? path + 1 : path;
    if (strlen(requested) == 0) {
        requested = "index.html";
    }

    size_t neededSize = strlen(requested) + 1;
    if (neededSize > buffSize) {
        return -2;
    }

    strcpy(buff, requested);
    return 0;
}

int get_file_path(const char* request, const char* currentWorkingDir, char* buff, const size_t buffSize) {
    char reqFile[1024] = {0};

    if (get_requested_file(request, reqFile, sizeof(reqFile)) != 0) {
        return -1;
    }

    if (snprintf(buff, buffSize, "%s%s", currentWorkingDir, reqFile) >= (int)buffSize) {
        fprintf(stderr, "Full path is too long.\n");
        return -2;
    }

    return 0;
}

int get_HTML_content(const char* request, const char* rootDir, char* buff, const size_t buffSize) {
    char path[1024] = {0};
    if (get_file_path(request, rootDir, path, sizeof(path)) != 0) {
        return 1;
    }

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("File not found: %s\n", path);
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    if (size <= 0 || size >= (long)buffSize) {
        fclose(file);
        return 1;
    }

    size_t bytesRead = fread(buff, 1, size, file);
    buff[bytesRead] = '\0';
    fclose(file);

    return 0;
}

int build_response(const char* reqeust, const char* rootDir, char* buff, const size_t buffSize) {
    char htmlbuffer[4000];
    int written=0;
    if (get_HTML_content(reqeust, rootDir, htmlbuffer, sizeof(htmlbuffer)) < 0) {
        const char* body = "<html><body><h1>404 Not Found</h1></body></html>";
        written = snprintf(buff, buffSize,
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Length: %zu\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n%s", strlen(body), body);

        return (written >= 0 && (size_t)written < buffSize) ? 0 : -1;
    } else {
        const size_t contentLength = strlen(htmlbuffer);
        written = snprintf(buff, buffSize,
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: %zu\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n%s", contentLength, htmlbuffer);
    }

    return (written >= 0 && (size_t)written < buffSize) ? 0 : -1;
}