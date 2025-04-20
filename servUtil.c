//
// Created by matyas on 4/20/25.
//

#include "servUtil.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* getHTMLContent(const char* path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("File not found: %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
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

