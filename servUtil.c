//
// Created by matyas on 4/20/25.
//

#include "servUtil.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int getClientHandle(struct Server *server) {
    int address_length = sizeof(server->address);
    int clientSocket = accept(server->socket, (struct sockaddr *)&server->address, (socklen_t *)&address_length);

    if (clientSocket < 0) {
        perror("Failed to accept a new connection");
        return 0;
    }

    return clientSocket;
}

int getClientRequest(int clientHandle, char* buff, size_t buffSize) {
    ssize_t bytesRead = read(clientHandle, buff, buffSize);

    if (bytesRead < 0) {
        perror("Failed to read request");
        close(clientHandle);
        return 1;
    } else if (bytesRead == 0) {
        printf("Client closed the connection before sending data.\n");
        close(clientHandle);
        return 0;
    } else {
        buff[bytesRead] = '\0';
        printf("Request received:\n%s\n", buff);
    }
    return 0;
}

char* getHTMLContent(const char* path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("File not found: %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    const long size = ftell(file);
    rewind(file);

    if (size == 0) {
        fclose(file);
        return NULL;
    }

    char* buffer = (char*) malloc(size + 1);
    if (buffer == NULL) {
        printf("Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(buffer, 1, size, file);
    if (bytesRead != size) {
        printf("Error reading the file\n");
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[size] = '\0';
    fclose(file);

    return buffer;
}

char* getRequestedFile(const char* request) {
    char method[8], path[256];

    sscanf(request, "%7s %255s", method, path);

    char *filename = malloc(strlen(path) + 1);
    if (filename == NULL) {
        printf("Memory allocation failed for filename.\n");
        return NULL;
    }

    strcpy(filename, path[0] == '/' ? path + 1 : path);

    if (strlen(filename) == 0) {
        free(filename);
        filename = strdup("index.html");
        if (filename == NULL) {
            printf("Memory allocation failed for default filename.\n");
            return NULL;
        }
    }

    return filename;
}

char* getFilePath(const char* request, const char* currentWorkingDir) {
    char* reqFile = getRequestedFile(request);
    char* fullPath = malloc(strlen(currentWorkingDir) + strlen(reqFile) + 1);
    if (reqFile == NULL) {
        printf("File not found: %s\n", reqFile);
        reqFile = "";
    }
    sprintf(fullPath,"%s%s",currentWorkingDir, reqFile);
    return fullPath;
}

char* buildResponse(const char* HTMLContent) {
    size_t contentLength = strlen(HTMLContent);
    size_t headerLength = 100;
    size_t totalLength = headerLength + contentLength;

    char* response = malloc(totalLength);
    if (response == NULL) {
        return NULL;
    }

    snprintf(response, totalLength,
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: %lu\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n%s", contentLength, HTMLContent);

    return response;
}


