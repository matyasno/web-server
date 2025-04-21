//
// Created by matyas on 4/20/25.
//

#ifndef SERVUTIL_H
#define SERVUTIL_H

#endif //SERVUTIL_H

#include "Server.h"

// net shit
int getClientHandle(struct Server *server);
int getClientRequest(int clientHandle, char* buff, size_t buffSize);

// parsing/building client requests
char* getHTMLContent(const char* path);
char* getRequestedFile(const char* request);
char* getFilePath(const char* request, const char* currentWorkingDir);
char* buildResponse(const char* HTMLContent);
char* buildNotFoundResponse();
