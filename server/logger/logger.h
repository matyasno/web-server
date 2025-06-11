//
// Created by matyas on 09.06.2025.
//

#ifndef LOGGER_H
#define LOGGER_H

#include "log_defines.h"

void log_error(const char *format, ...);
void log_debug(const char *format, ...);
void log_warning(const char *format, ...);
void log_info(const char *format, ...);

int write_to_log_file(const char* prefix, const char* msg);

void error_disable();
void error_enable();

void warning_disable();
void warning_enable();

void info_disable();
void info_enable();

void debug_disable();
void debug_enable();

#endif //LOGGER_H