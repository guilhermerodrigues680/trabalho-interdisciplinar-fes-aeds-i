#ifndef HEADER_SERVICE_H
#define HEADER_SERVICE_H

#include "models.h"

// https://stackoverflow.com/questions/4396140/why-doesnt-ansi-c-have-namespaces
typedef struct
{
    // const int some_value;
    // void (*method1)(void);
    // void (*method2)(int);
    int (*registerClient)(const char *name, const char *address);
    int (*registerVehicle)(char *descricao, char *modelo, char *cor,
                           char *placa, double valorDiaria, int qntOcupantes);
    int (*registerLocation)(time_t withdrawalDate, time_t returnDate, int hasInsurance, int clientCod, int vehicleCod);
    // void serviceFinishLocation();
    void (*listClients)(void);
    void (*listVehicles)(void);
    void (*listLocations)(void);
    void (*endLocation)(void);
    int (*getClient)(int cod, Client *client);
    int (*findVehicleWithCapacity)(int cap, Vehicle *v);
    // void serviceFindByClient(int cod);
    // void serviceFindByVehicle(int cod);
    // void serviceFindLocationsByClient(int cod);
    // void serviceCalcRewardPointsByClient(int cod);
} Service;

extern const Service service;

#endif