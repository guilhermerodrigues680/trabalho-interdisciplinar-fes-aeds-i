/* client.h
 *
 * Este arquivo representa as declarações de funções, macros e tipos do client.c.
 *
 *********************************************************************/

#ifndef _CLIENT_H_INCLUDED_
#define _CLIENT_H_INCLUDED_

typedef struct
{
    int cod;
    char name[100];
    char address[100];
} Client;

int client_register(const char *name, const char *address);
void client_list(void);
int client_get_by_cod(int cod, Client *client);

extern char *clients_db_file;

#endif