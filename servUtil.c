//
// Created by matyas on 4/20/25.
//

#include "servUtil.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* loadHTMLContent(const char* path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("File not found: %s\n", path);
        return NULL;  // Vracím NULL v případě chyby
    }

    // Zjištění velikosti souboru v bajtech
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    // Kontrola, jestli soubor není prázdný
    if (size == 0) {
        fclose(file);
        return NULL;
    }

    // Alokace paměti pro obsah souboru
    char* buffer = (char*) malloc(size + 1);  // +1 pro '\0'
    if (buffer == NULL) {  // Kontrola, jestli alokace probíhla správně
        printf("Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    // Přečteme obsah souboru do bufferu
    size_t bytesRead = fread(buffer, 1, size, file);
    if (bytesRead != size) {
        printf("Error reading the file\n");
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[size] = '\0';
    fclose(file);

    return buffer;
}

char* getRequestedFile(const char* request) {
    char method[8], path[256];

    // Parsuj první řádek requestu: "GET /cesta HTTP/1.1"
    sscanf(request, "%7s %255s", method, path);

    // Odstraň počáteční lomítko
    char *filename = malloc(strlen(path) + 1);  // Alokace pro string + '\0'
    if (filename == NULL) {
        printf("Memory allocation failed for filename.\n");
        return NULL;  // Pokud alokace selže, vrátíme NULL
    }

    // Zkopíruj cestu, odstraníme první lomítko, pokud je tam
    strcpy(filename, path[0] == '/' ? path + 1 : path);

    // Pokud nenajde vrátí index.html
    if (strlen(filename) == 0) {
        free(filename);
        filename = strdup("index.html");
        if (filename == NULL) {
            printf("Memory allocation failed for default filename.\n");
            return NULL;  // Pokud se nepodaří alokovat "index.html"
        }
    }

    return filename;
}

