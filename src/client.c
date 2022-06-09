#include <stdio.h>
#include <string.h>

typedef struct
{
    int cod;
    char name[100];
    char address[100];

    int (*mediaValores)(int, int, int);
} Client;

// prototypes
void NewClient(Client *c, int cod, char *name, char *address);
int Client_MediaValores(int v1, int v2, int v3);

void NewClient(Client *c, int cod, char *name, char *address)
{
    c->cod = cod;
    strcpy(c->name, name);
    strcpy(c->address, address);

    c->mediaValores = Client_MediaValores;
}

int Client_MediaValores(int v1, int v2, int v3)
{
    return ((v1 + v2 + v3) / 3);
}
