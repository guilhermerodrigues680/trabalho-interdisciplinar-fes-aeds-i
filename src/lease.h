/* lease.h
 *
 * Este arquivo representa as declarações de funções, macros e tipos do lease.c.
 *
 *********************************************************************/

#ifndef _LEASE_H_INCLUDED_
#define _LEASE_H_INCLUDED_

#include <time.h>

typedef struct
{
    int cod;
    int clientCod;
    int vehicleCod;
    time_t withdrawalDate;
    time_t returnDate;
    unsigned char hasInsurance;
    unsigned char finished;
    // TODO: Armazenar dados da locação para relatório
} Lease;

int lease_register(time_t withdrawal_date, time_t return_date, int has_insurance,
                   int client_cod, int vehicle_cod);
void lease_list(void);
int lease_get_by_cod(int cod, Lease *lease);
int lease_finalize(int cod);

extern char *lease_db_file;

#endif