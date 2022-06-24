#ifndef HEADER_VEHICLE_H
#define HEADER_VEHICLE_H

#include "models.h"

typedef struct
{
    int (*registerVehicle)(char *descricao, char *modelo, char *cor,
                           char *placa, double valorDiaria, int qntOcupantes);
    void (*listVehicles)(void);
    int (*findVehicleWithCapacity)(int cap, Vehicle *v);
    int (*updateVehicleStatus)(int cod, int vehicleStatus);
    int (*getVehicle)(int cod, Vehicle *v);
} VehicleRepo;

extern char *vehicleDbFile;
extern const VehicleRepo vehicleRepo;

#endif