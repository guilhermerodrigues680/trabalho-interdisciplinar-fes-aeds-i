#include <stdio.h>

typedef struct
{
    int cod;
    char *name;
    char *address;

    int (*mediaValores)(int, int, int);
} Client;

// prototypes
void NewClient(Client *c, int cod, char *name, char *address);
int Client_MediaValores(int v1, int v2, int v3);

void NewClient(Client *c, int cod, char *name, char *address)
{
    c->cod = cod;
    // TODO: strcpy fica dando warning
    // strcpy(c->name, name);
    // strcpy(c->address, address);

    c->name = name;
    c->address = address;
    c->mediaValores = Client_MediaValores;
}

int Client_MediaValores(int v1, int v2, int v3)
{
    return ((v1 + v2 + v3) / 3);
}
