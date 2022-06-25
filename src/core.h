/* core.h
 *
 * Este arquivo representa as declarações de funções, macros e tipos do core.c.
 *
 *********************************************************************/

#ifndef _CORE_H_INCLUDED_
#define _CORE_H_INCLUDED_

#include <time.h>
#include "client.h"
#include "vehicle.h"

int core_register_client(const char *name, const char *address);
int core_register_vehicle(char *desc, char *model, char *color, char *registration_plate,
                          double charge_per_day, int passenger_capacity);
int core_register_lease(time_t withdrawal_date, time_t return_date, int has_insurance,
                        int client_cod, int vehicle_cod);
void core_list_clients(void);
void core_list_vehicles(void);
void core_list_leases(void);
void core_finalize_lease(void);
int core_get_client(int cod, Client *client);
int core_find_vehicle_with_capacity(int cap, Vehicle *v);

#endif