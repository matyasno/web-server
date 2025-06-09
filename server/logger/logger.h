//
// Created by matyas on 09.06.2025.
//

#ifndef LOGGER_H
#define LOGGER_H


void log_error(const char *format, ...);
void log_debug(const char *format, ...);
void log_warning(const char *format, ...);
void log_info(const char *format, ...);

#endif //LOGGER_H