//
// Created by matyas on 5/2/25.
//

#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include "../config/config_parser.h"

#ifdef WIN32
#include <_bsd_types.h>
#else
#include <sys/types.h>
#endif

void parse_args(int argc, char *argv[], server_config *config);
u_long get_host_order_ip(const char *host_ip);
int parse_port(const char *host_port);
int handle_flag(const int argc, char **argv);

#endif //ARGS_PARSER_H
