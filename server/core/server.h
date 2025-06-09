#ifndef SERVER_H
#define SERVER_H

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    typedef int socklen_t;
#else
#include <sys/socket.h>
#endif

struct Server {
    int family;
    int service;
    int protocol;
    const char* server_interface;
    const char* port;
    int backlog;
    int socket;
    struct sockaddr_storage address;
    socklen_t address_len;
};

struct Server create_server(int service, int protocol, const char* server_interface, const char* port, int backlog);
void start_http_server(struct Server *server, const char* root_dir);

int init_address(struct Server *server);
int get_address_family(struct Server *server);
int create_socket(struct Server *server);

#endif // SERVER_H
