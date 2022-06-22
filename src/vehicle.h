#ifndef HEADER_VEHICLE_H
#define HEADER_VEHICLE_H

// alugado, disponível e em manutenção
#define VEHICLE_STATUS_AVAILABLE 0
#define VEHICLE_STATUS_LEASED 1
#define VEHICLE_STATUS_MAINTENANCE 2

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
    int (*updateVehicleStatus)(int cod, int vehicleStatus);
} VehicleRepo;

extern const VehicleRepo vehicleRepo;

#endif