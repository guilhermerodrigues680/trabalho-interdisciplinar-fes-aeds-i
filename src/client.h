#ifndef HEADER_CLIENT_H
#define HEADER_CLIENT_H

#include "models.h"

typedef struct
{
    int (*registerClient)(const char *name, const char *address);
    void (*listClients)(void);
    int (*getClient)(int cod, Client *client);
} ClientsRepo;

extern char *clientsDbFile;
extern const ClientsRepo clientsRepo;

#endif