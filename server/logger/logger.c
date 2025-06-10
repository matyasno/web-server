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
    if (ERROR_ENABLED == 1) {
        fprintf(stderr, RED"[ERROR] " RESET);
        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
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
        char msg[MAX_MSG_SIZE];
        fprintf(stdout, GRN"[INFO] " RESET);
        va_list args;
        va_start(args, format);
        vsnprintf(msg, MAX_MSG_SIZE, format, args);
        fprintf(stdout, "%s\n", msg);
        va_end(args);
    }
    if (WARNING_ENABLED == 1 && FILE_ENABLED == 1) {

    }
}

void log_warning(const char *format, ...) {
   if (WARNING_ENABLED == 1) {
       char msg[MAX_MSG_SIZE];
       fprintf(stderr, YEL"[WARNING] " RESET);
       va_list args;
       va_start(args, format);
       vsnprintf(msg, MAX_MSG_SIZE, format, args);
       fprintf(stderr, "%s", msg);
       va_end(args);
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
