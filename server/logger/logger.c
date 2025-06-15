//
// Created by matyas on 09.06.2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include <stdarg.h>
#else
#include <__stdarg_va_arg.h>
#endif

#include "../defines.h"
#include "../utils/file_utils.h"
#include "logger.h"
#include "log_defines.h"

int INFO_ENABLED = 1;
int DEBUG_ENABLED = 0;
int WARNING_ENABLED = 1;
int ERROR_ENABLED = 1;

int FILE_ENABLED = 1;

void log_error(const char *format, ...) {
    char msg[MAX_MSG_SIZE];
    const char* prefix = ERROR_PREFIX;

    if (ERROR_ENABLED == 1) {
        va_list args;
        va_start(args, format);
        vsnprintf(msg, MAX_MSG_SIZE, format, args);
        va_end(args);
        fprintf(stderr, RED "%s"RESET, prefix);
        fprintf(stderr, "%s\n", msg);
    }
    if (ERROR_ENABLED == 1 && FILE_ENABLED == 1) {
        write_to_log_file(prefix, msg);
    }
}

void log_debug(const char *format, ...) {
    char msg[MAX_MSG_SIZE];
    const char* prefix = DEBUG_PREFIX;

    if (DEBUG_ENABLED == 1) {
        va_list args;
        va_start(args, format);
        vsnprintf(msg, MAX_MSG_SIZE, format, args);
        va_end(args);
        fprintf(stdout, BLU "%s" RESET, prefix);
        fprintf(stdout, "%s\n", msg);
    }
    if (DEBUG_ENABLED == 1 && FILE_ENABLED == 1) {
        write_to_log_file(prefix, msg);
    }
}

void log_info(const char *format, ...) {
    char msg[MAX_MSG_SIZE];
    const char* prefix = INFO_PREFIX;

    if (INFO_ENABLED == 1) {
        va_list args;
        va_start(args, format);
        vsnprintf(msg, MAX_MSG_SIZE, format, args);
        va_end(args);
        fprintf(stdout, GRN "%s" RESET, prefix);
        fprintf(stdout, "%s\n", msg);
    }
    if (INFO_ENABLED == 1 && FILE_ENABLED == 1) {
        write_to_log_file(prefix, msg);
    }
}

void log_warning(const char *format, ...) {
    char msg[MAX_MSG_SIZE];
    const char* prefix = WARN_PREFIX;

    if (WARNING_ENABLED == 1) {
        va_list args;
        va_start(args, format);
        vsnprintf(msg, MAX_MSG_SIZE, format, args);
        va_end(args);
        fprintf(stdout, YEL WARN_PREFIX RESET);
        fprintf(stdout, "%s\n", msg);
    }
    if (WARNING_ENABLED == 1 && FILE_ENABLED == 1) {
        write_to_log_file(prefix, msg);
    }
}

int write_to_log_file(const char* prefix, const char* msg) {
    FILE *file_fd = fopen(LOG_PATH, "a");
    write_to_file(file_fd, prefix, strlen(prefix));
    write_to_file(file_fd, msg, strlen(msg));
    write_to_file(file_fd, "\n", 1);
    fclose(file_fd);
    return OK;
}

void error_disable() {
    ERROR_ENABLED = 0;
}
void error_enable() {
    ERROR_ENABLED = 1;
}

void warning_disable() {
    WARNING_ENABLED = 0;
}
void warning_enable() {
    WARNING_ENABLED = 1;
}

void info_disable() {
    INFO_ENABLED = 0;
}
void info_enable() {
    INFO_ENABLED = 1;
}

void debug_disable() {
    DEBUG_ENABLED = 0;
}
void debug_enable() {
    DEBUG_ENABLED = 1;
}
