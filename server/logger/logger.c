//
// Created by matyas on 09.06.2025.
//
#include <stdio.h>
#include <__stdarg_va_arg.h>
#include <stdlib.h>
#include <string.h>

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
    if (ERROR_ENABLED == 1) {
        fprintf(stderr, RED ERROR_PREFIX RESET);
        va_list args;
        va_start(args, format);
        vsnprintf(msg, MAX_MSG_SIZE, format, args);
        fprintf(stderr, "%s\n", msg);
        va_end(args);
    }
    if (ERROR_ENABLED == 1 && FILE_ENABLED == 1) {
        FILE *file_fd = fopen(LOG_PATH, "a");
        write_to_file(file_fd, ERROR_PREFIX, strlen(ERROR_PREFIX));
        write_to_file(file_fd, msg, strlen(msg));
        write_to_file(file_fd, "\n", 1);
        fclose(file_fd);
    }
}

void log_debug(const char *format, ...) {
    char msg[MAX_MSG_SIZE];
    if (DEBUG_ENABLED == 1) {
        fprintf(stdout, BLU DEBUG_PREFIX RESET);
        va_list args;
        va_start(args, format);
        vsnprintf(msg, MAX_MSG_SIZE, format, args);
        fprintf(stdout, "%s\n", msg);
        va_end(args);
    }
    if (DEBUG_ENABLED == 1 && FILE_ENABLED == 1) {
        FILE *file_fd = fopen(LOG_PATH, "a");
        write_to_file(file_fd, DEBUG_PREFIX, strlen(DEBUG_PREFIX));
        write_to_file(file_fd, msg, strlen(msg));
        write_to_file(file_fd, "\n", 1);
        fclose(file_fd);
    }
}

void log_info(const char *format, ...) {
    char msg[MAX_MSG_SIZE];
    if (INFO_ENABLED == 1) {
        fprintf(stdout, GRN INFO_PREFIX RESET);
        va_list args;
        va_start(args, format);
        vsnprintf(msg, MAX_MSG_SIZE, format, args);
        fprintf(stdout, "%s\n", msg);
        va_end(args);
    }
    if (INFO_ENABLED == 1 && FILE_ENABLED == 1) {
        FILE *file_fd = fopen(LOG_PATH, "a");
        write_to_file(file_fd, INFO_PREFIX, strlen(INFO_PREFIX));
        write_to_file(file_fd, msg, strlen(msg));
        write_to_file(file_fd, "\n", 1);
        fclose(file_fd);
    }
}

void log_warning(const char *format, ...) {
    char msg[MAX_MSG_SIZE];
    if (WARNING_ENABLED == 1) {
        fprintf(stdout, YEL WARN_PREFIX RESET);
        va_list args;
        va_start(args, format);
        vsnprintf(msg, MAX_MSG_SIZE, format, args);
        fprintf(stdout, "%s\n", msg);
        va_end(args);
    }
    if (WARNING_ENABLED == 1 && FILE_ENABLED == 1) {
        FILE *file_fd = fopen(LOG_PATH, "a");
        write_to_file(file_fd, WARN_PREFIX, strlen(WARN_PREFIX));
        write_to_file(file_fd, msg, strlen(msg));
        write_to_file(file_fd, "\n", 1);
        fclose(file_fd);
    }
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
