#ifndef HEADER_VEHICLE_H
#define HEADER_VEHICLE_H

typedef struct
{
    int cod;
    char descricao[100];
    char modelo[30];
    char cor[30];
    char placa[15];
    double valorDiaria;
    int qntOcupantes;
    int status;
} Vehicle;

typedef struct
{
    void (*registerVehicle)(void);
    void (*listVehicles)(void);
    int (*findVehicleWithCapacity)(int cap, Vehicle *v);
} VehicleRepo;

extern const VehicleRepo vehicleRepo;

#endif