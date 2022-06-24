#ifndef HEADER_CLIENT_H
#define HEADER_CLIENT_H

typedef struct
{
    int cod;
    char name[100];
    char address[100];
} Client;

typedef struct
{
    int (*registerClient)(const char *name, const char *address);
    void (*listClients)(void);
    int (*clientExists)(int cod, Client *client);
} ClientsRepo;

extern char *clientsDbFile;
extern const ClientsRepo clientsRepo;

#endif