/* vehicle.h
 *
 * Este arquivo representa as declarações de funções, macros e tipos do vehicle.c.
 *
 *********************************************************************/

#ifndef _VEHICLE_H_INCLUDED_
#define _VEHICLE_H_INCLUDED_

// disponível, alugado e em manutenção
typedef enum
{
    VEHICLE_STATUS_AVAILABLE = 0,
    VEHICLE_STATUS_LEASED = 1,
    VEHICLE_STATUS_MAINTENANCE = 2,
} VehicleStatus;

typedef struct
{
    int cod;
    char descricao[100];
    char modelo[30];
    char cor[30];
    char placa[15];
    double valorDiaria;
    int qntOcupantes;
    VehicleStatus status;
} Vehicle;

int vehicle_register(char *desc, char *model, char *color, char *registration_plate,
                     double charge_per_day, int passenger_capacity);
void vehicle_list(void);
int vehicle_find_one_by_capacity(int cap, Vehicle *v);
int vehicle_update_status(int cod, VehicleStatus new_status);
int vehicle_get_by_cod(int cod, Vehicle *v);

extern char *vehicle_db_file;

#endif