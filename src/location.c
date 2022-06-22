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
void listLocations();
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

void listLocations()
{
    FILE *fPtr = fopen(locationDbFile, "r");
    if (fPtr == NULL) // Arquivo não existe
    {
        printf("* Nenhuma locação cadastrado.\n");
        return;
    }

    printf("* COD - DESCRICAO : MODELO : COR : PLACA : VALOR DIARIA : QNT. OCUPANTES : STATUS\n");
    Location l;
    while (fread(&l, sizeof(Location), 1, fPtr))
    {
        struct tm withdrawalDate = *localtime(&l.withdrawalDate);
        struct tm returnDate = *localtime(&l.returnDate);
        // struct tm tm2 = *gmtime(&t);

        char bufFmtWithdrawalDate[20];
        char bufFmtReturnDate[20];
        formatDate(&withdrawalDate, bufFmtWithdrawalDate, sizeof(bufFmtWithdrawalDate));
        formatDate(&returnDate, bufFmtReturnDate, sizeof(bufFmtReturnDate));

        printf("* %d - %d : %d : %s -> %s : %s\n",
               l.cod,
               l.clientCod,
               l.vehicleCod,
               bufFmtWithdrawalDate,
               bufFmtReturnDate,
               l.hasInsurance ? "Possui Seguro" : "Não Possui Seguro");
    }

    fclose(fPtr);
}

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
    .listLocations = &listLocations,
    .registerLocation = &registerLocation};