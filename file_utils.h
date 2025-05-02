//
// Created by matyas on 5/2/25.
//

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stddef.h>

char* get_file_content(const char* request, const char* root_dir, size_t* outSize);

#endif //FILE_UTILS_H
