//
// Created by matyas on 5/2/25.
//

#include "args_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "../defines.h"
#include "../logger/logger.h"
#include "../logger/log_defines.h"

char* man = "MANUAL\n"
           BOLD "--silent \n\t" RESET
           "Disables all logger messages\n"
           BOLD "--debug \n\t" RESET
           "Enables debug messages\n\t"
           "Only INFO, WARNING and ERROR are enabled by default\n"
           BOLD ""
;


void parse_args(const int argc, char **argv, server_config *config) {
    if (argc == 2) {
        handle_flag(argc, argv);
        exit(-1);
    }
    if (argc == 5) {
        handle_flag(argc, argv);
        strncpy(config->interface, argv[2], MAX_ADDR_LEN - 1);
        config->interface[MAX_ADDR_LEN - 1] = '\0';
        strncpy(config->port, argv[3], MAX_PORT_LEN - 1);
        config->port[MAX_PORT_LEN - 1] = '\0';
        strncpy(config->web_root, argv[4], PATH_SIZE - 1);
        config->web_root[PATH_SIZE - 1] = '\0';
        config->protocol=0;
        config->backlog=5;
    }
    if(argc == 4) {
        handle_flag(argc, argv);
        strncpy(config->interface, argv[1], MAX_ADDR_LEN - 1);
        config->interface[MAX_ADDR_LEN - 1] = '\0';
        strncpy(config->port, argv[2], MAX_PORT_LEN - 1);
        config->port[MAX_PORT_LEN - 1] = '\0';
        strncpy(config->web_root, argv[3], PATH_SIZE - 1);
        config->web_root[PATH_SIZE - 1] = '\0';
        config->protocol=0;
        config->backlog=5;
    }
    if (argc == 1) {
        printf("Usage: <options (optional)> <ip> <port> <dir>\n");
        exit(-1);
    }
}

u_long get_host_order_ip(const char *host_ip) {
    return ntohl(inet_addr(host_ip));
}

int parse_port(const char *host_port) {
    int port;
    char *endptr;
    long port_long = strtol(host_port, &endptr, 10);

    if (*endptr != '\0' || port_long < 1 || port_long > 65535) {
        fprintf(stderr, "Invalid port number: %s\n", host_port);
        return ERROR_GENERIC;
    } else {
        port = (int)port_long;
    }
    return port;
}

int handle_flag(const int argc, char **argv) {
    if (strcmp(argv[1], "--help") == 0) {
        printf("%s\n", man);
    }  else if (strcmp(argv[1], "--silent") == 0) {
        error_disable();
        warning_disable();
        info_disable();
        debug_disable();
    } else if (strcmp(argv[1], "--debug") == 0) {
        debug_enable();
    }
    return ERROR_GENERIC;
}
