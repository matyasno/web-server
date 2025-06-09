//
// Created by matyas on 5/2/25.
//

#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H
#include <sys/types.h>

#endif //ARGS_PARSER_H

void parse_args(int argc, char *argv[], int *port, char* server_interface, int *protocol , int *backlog, char* web_directory);
u_long get_host_order_ip(const char *host_ip);
int parse_port(const char *host_port);
int parse_options(const int argc, char **argv);
