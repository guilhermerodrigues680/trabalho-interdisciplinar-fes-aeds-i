#ifndef HEADER_CLIENT_H
#define HEADER_CLIENT_H

typedef struct
{
    void (*registerClient)(void);
    void (*listClients)(void);
} ClientsRepo;

extern const ClientsRepo clientsRepo;

#endif