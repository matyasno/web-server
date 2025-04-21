//
// Created by matyas on 4/20/25.
//

#ifndef SERVUTIL_H
#define SERVUTIL_H

#endif //SERVUTIL_H

char* getHTMLContent(const char* path);

char* getRequestedFile(const char* request);

void HTMLparser(char* content);

char* getFilePath(const char* request, const char* currentWorkingDir);

char* buildResponse(const char* HTMLContent);