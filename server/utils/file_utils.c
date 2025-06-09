//
// Created by matyas on 5/2/25.
//

#include "file_utils.h"
#include "../request/request_parser.h"
#include "../defines.h"
#include "../logger/logger.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* get_file_content(const char* request, const char* root_dir, size_t* outSize) {
    char path[PATH_SIZE];
    if (get_file_path(request, root_dir, path, sizeof(path)) != 0) {
        return NULL;
    }

    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        log_warning("File not found: %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    const long size = ftell(file);
    rewind(file);
    *outSize = size;

    char* content = malloc(size+1);
    if (content == NULL) {
        log_error("Failed to allocate memory");
        fclose(file);
        return NULL;
    }

    const size_t bytesRead = fread(content, 1, size, file);

    if (bytesRead != size) {
        log_error("Failed to read file");
        fclose(file);
        free(content);
        return NULL;
    }

    content[size] = '\0';
    fclose(file);
    return content;
}
