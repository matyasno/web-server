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


void parse_args(const int argc, char **argv, int *port, char* server_interface, int *protocol , int *backlog, char* web_directory) {
    if (argc == 2) {
        parse_options(argc, argv);
        exit(-1);
    }
    if (argc == 5) {
        parse_options(argc, argv);
        *port = atoi(argv[3]);
        *protocol = 0;
        *backlog = 5;
        strcpy(server_interface, argv[2]);
        strcpy(web_directory, argv[4]);
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

int parse_options(const int argc, char **argv) {
    if (strcmp(argv[1], "--help") == 0) {
        printf("%s\n", man);
    }  else if (strcmp(argv[1], "--silent") == 0) {
        error_disable();
        warning_disable();
        info_disable();
        debug_disable();
    } else if (strcmp(argv[1], "--debug") == 0) {
        debug_disable();
    }
    return ERROR_GENERIC;
}
