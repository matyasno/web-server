//
// Created by matyas on 09.06.2025.
//

#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include "../defines.h"

typedef struct {
    char interface[MAX_ADDR_LEN];
    char port[MAX_PORT_LEN];
    char web_root[PATH_SIZE];
    int protocol;
    int backlog;
} server_config;

#endif //CONFIG_PARSER_H
