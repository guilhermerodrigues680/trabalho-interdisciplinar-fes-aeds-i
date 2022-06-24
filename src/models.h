#ifndef HEADER_MODELS_H
#define HEADER_MODELS_H

#include <time.h>

// alugado, disponível e em manutenção
#define VEHICLE_STATUS_AVAILABLE 0
#define VEHICLE_STATUS_LEASED 1
#define VEHICLE_STATUS_MAINTENANCE 2

typedef struct
{
    int cod;
    char name[100];
    char address[100];
} Client;

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
    int cod;
    int clientCod;
    int vehicleCod;
    time_t withdrawalDate;
    time_t returnDate;
    unsigned char hasInsurance;
    unsigned char finished;
    // TODO: Armazenar dados da locação para relatório
} Location;

#endif