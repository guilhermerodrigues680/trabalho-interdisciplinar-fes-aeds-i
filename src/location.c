#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "location.h"
#include "utils.h"

typedef struct
{
    int cod;
    time_t withdrawalDate;
    time_t returnDate;
    int hasInsurance;
    int clientCod;
    int vehicleCod;
} Location;

// global
const char *locationDbFile = "location_db.dat";

// prototypes
void registerLocation(time_t withdrawalDate, time_t returnDate, int hasInsurance, int clientCod, int vehicleCod);
// void listLocations();
int getLastLocationId();

void registerLocation(time_t withdrawalDate, time_t returnDate, int hasInsurance, int clientCod, int vehicleCod)
{
    Location l;
    l.cod = getLastLocationId() + 1;
    l.clientCod = clientCod;
    l.vehicleCod = vehicleCod;
    l.hasInsurance = hasInsurance;
    l.withdrawalDate = withdrawalDate;
    l.returnDate = returnDate;

    FILE *fPtr = fopen(locationDbFile, "a+");
    fwrite(&l, sizeof(Location), 1, fPtr);
    if (fwrite == 0)
        printf("erro interno ao cadastrar locação.\n");
    else
        printf("Locação cadastrada. Cod: %d\n", l.cod);
    fclose(fPtr);
}

// void listLocations()
// {
//     FILE *fPtr = fopen(locationDbFile, "r");
//     if (fPtr == NULL) // Arquivo não existe
//     {
//         printf("* Nenhuma locação cadastrado.\n");
//         return;
//     }

//     printf("* COD - DESCRICAO : MODELO : COR : PLACA : VALOR DIARIA : QNT. OCUPANTES : STATUS\n");
//     Location l;
//     while (fread(&l, sizeof(Location), 1, fPtr))
//         printf("* %d - %s : %s : %s : %s : R$%.2f : %d : %s\n",
//                v.cod,
//                v.descricao,
//                v.modelo,
//                v.cor,
//                v.placa,
//                v.valorDiaria,
//                v.qntOcupantes,
//                v.status ? "Indisponivel" : "Disponivel");

//     fclose(fPtr);
// }

int getLastLocationId()
{
    int lastId = -1;
    FILE *fPtr = fopen(locationDbFile, "r");
    if (fPtr == NULL) // Arquivo não existe
        return lastId;

    // lê todos as locacoes
    Location l;
    while (fread(&l, sizeof(Location), 1, fPtr))
        lastId = l.cod;

    fclose(fPtr);
    return lastId;
}

const LocationRepo locationRepo = {
    // .listLocations = &listLocations,
    .registerLocation = &registerLocation};