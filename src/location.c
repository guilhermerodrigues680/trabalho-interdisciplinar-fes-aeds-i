#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "location.h"
#include "utils.h"

// prototypes
int registerLocation(time_t withdrawalDate, time_t returnDate, int hasInsurance, int clientCod, int vehicleCod);
void listLocations(void);
int getLocation(int cod, Location *location);
int finalizeLease(int cod);

int getLastLocationId();

// global
char *locationDbFile = "_db_leases.dat";

const LocationRepo locationRepo = {
    .listLocations = &listLocations,
    .registerLocation = &registerLocation,
    .getLocation = &getLocation,
    .finalizeLease = &finalizeLease};

int registerLocation(time_t withdrawalDate, time_t returnDate, int hasInsurance, int clientCod, int vehicleCod)
{
    Location l = {
        .cod = getLastLocationId() + 1,
        .clientCod = clientCod,
        .vehicleCod = vehicleCod,
        .hasInsurance = hasInsurance,
        .withdrawalDate = withdrawalDate,
        .returnDate = returnDate,
        .finished = 0,
    };

    FILE *fPtr = fopen(locationDbFile, "a+");
    if (fwrite(&l, sizeof(Location), 1, fPtr) != 1)
    {
        printf("erro interno ao cadastrar locação.\n");
        fclose(fPtr);
        return EXIT_FAILURE;
    }

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

    printf("* COD - (STATUS) - CLIENTE : VEICULO : INICIO -> FIM : SEGURO\n");
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

        printf("* %d - %s - %d : %d : %s -> %s : %s\n",
               l.cod,
               l.finished ? "(FINALIZADA)" : "()",
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

int getLocation(int cod, Location *l)
{
    FILE *fPtr = fopen(locationDbFile, "r");
    if (fPtr == NULL) // Arquivo não existe, logo locação não existe
        return 0;

    // https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
    fseek(fPtr, 0L, SEEK_END);
    long size = ftell(fPtr);
    rewind(fPtr);

    // como o id do cliente é incremental
    long offset = sizeof(Location) * cod;
    if (offset >= size) // se o offset é maior que o tamanho do arquivo, ou seja a locação não existe
        return 0;

    fseek(fPtr, offset, SEEK_SET);
    int exits = fread(l, sizeof(Location), 1, fPtr) == 1;
    fclose(fPtr);
    return exits;
}

int finalizeLease(int cod)
{
    FILE *fPtr = fopen(locationDbFile, "r+");
    if (fPtr == NULL) // Arquivo não existe, logo locacao não existe
        return 0;

    // https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
    fseek(fPtr, 0L, SEEK_END);
    long size = ftell(fPtr);
    rewind(fPtr);

    // como o id do cliente é incremental
    long offset = sizeof(Location) * cod;
    if (offset >= size) // se o offset é maior que o tamanho do arquivo, ou seja o veiculo não existe
        return 0;

    Location l;
    fseek(fPtr, offset, SEEK_SET);
    int exits = fread(&l, sizeof(Location), 1, fPtr) == 1;
    if (!exits)
    {
        printf("erro interno ao ler locacao.\n");
        return 0;
    }

    l.finished = 1;

    fseek(fPtr, offset, SEEK_SET);
    if (fwrite(&l, sizeof(Location), 1, fPtr) == 0)
    {
        printf("erro interno ao atualizar locação.\n");
        return 0;
    }
    else
    {
        printf("Locação atualizada. Cod: %d\n", l.cod);
    }

    fclose(fPtr);
    return 1;
}
