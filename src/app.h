/* app.h
 *
 * Este arquivo representa as declarações de funções, macros e tipos do app.c.
 *
 *********************************************************************/

#ifndef _APP_H_INCLUDED_
#define _APP_H_INCLUDED_

void app_register_client(void);
void app_register_vehicle(void);
void app_register_lease(void);
void app_list_clients(void);
void app_list_vehicles(void);
void app_list_leases(void);
void app_finalize_lease(void);

#endif