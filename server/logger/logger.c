//
// Created by matyas on 09.06.2025.
//
#include <stdio.h>
#include <__stdarg_va_arg.h>
#include <stdlib.h>
#include <string.h>

#include "../defines.h"

#include "logger.h"
#include "log_defines.h"

void log_error(const char *format, ...) {
    if (ERROR_ENABLED == 1) {
        fprintf(stderr, RED"[ERROR] " RESET);
        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
        fputs("\n", stderr);
    }
}

void log_debug(const char *format, ...) {
    if (DEBUG_ENABLED == 1) {
        fprintf(stdout, BLU"[DEBUG] " RESET);
        va_list args;
        va_start(args, format);
        vfprintf(stdout, format, args);
        va_end(args);
        fputs("\n", stdout);
    }
}

void log_info(const char *format, ...) {
    if (INFO_ENABLED == 1) {
        fprintf(stdout, GRN"[INFO] " RESET);
        va_list args;
        va_start(args, format);
        vfprintf(stdout, format, args);
        va_end(args);
        fputs("\n", stdout);
    }
}

void log_warning(const char *format, ...) {
   if (WARNING_ENABLED == 1) {
       fprintf(stderr, YEL"[WARNING] " RESET);
       va_list args;
       va_start(args, format);
       vfprintf(stderr, format, args);
       va_end(args);
       fputs("\n", stderr);
   }
}
