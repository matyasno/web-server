//
// Created by matyas on 5/2/25.
//

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stddef.h>
#include <stdio.h>

char* get_file_content(const char* request, const char* root_dir, size_t* outSize);
int write_to_file(FILE *file_fd, const char* content, size_t content_size);

#endif //FILE_UTILS_H