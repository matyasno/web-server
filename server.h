#ifndef SERVER_H
#define SERVER_H

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    typedef int socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

struct Server {
    int domain;
    int service;
    int protocol;
    const char* server_interface;
    int port;
    int backlog;
    int socket;
    struct sockaddr_storage address;
    socklen_t address_len;
};

struct Server server_constructor(int domain, int service, int protocol, const char* server_interface, int port, int backlog);
void start_http_server(struct Server *server, const char* root_dir);

#endif // SERVER_H
