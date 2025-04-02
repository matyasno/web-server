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
    u_long servinterface;
    int port;
    int backlog;
    int socket;
    struct sockaddr_in address;
    void (*launch)(struct Server *server);
};

struct Server server_constructor(int domain, int service, int protocol, u_long servInterface, int port, int backlog, void (*launch)(struct Server *server));

#endif // SERVER_H
