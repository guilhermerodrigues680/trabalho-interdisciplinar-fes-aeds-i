#ifndef HEADER_SERVICE_H
#define HEADER_SERVICE_H

// https://stackoverflow.com/questions/4396140/why-doesnt-ansi-c-have-namespaces
typedef struct
{
    // const int some_value;
    // void (*method1)(void);
    // void (*method2)(int);
    int (*registerClient)(const char *name, const char *address);
    int (*registerVehicle)(char *descricao, char *modelo, char *cor,
                           char *placa, double valorDiaria, int qntOcupantes);
    void (*registerLocation)(void);
    // void serviceFinishLocation();
    void (*listClients)(void);
    void (*listVehicles)(void);
    void (*listLocations)(void);
    void (*endLocation)(void);
    // void serviceFindByClient(int cod);
    // void serviceFindByVehicle(int cod);
    // void serviceFindLocationsByClient(int cod);
    // void serviceCalcRewardPointsByClient(int cod);
} Service;

extern const Service service;

#endif