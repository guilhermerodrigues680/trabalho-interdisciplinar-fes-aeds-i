#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vehicle.h"
#include "utils.h"

// alugado, disponível e em manutenção
#define VEHICLE_STATUS_LEASED 0
#define VEHICLE_STATUS_AVAILABLE 1
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

// global
const char *vehicleDbFile = "vehicle_db.dat";

// prototypes
void registerVehicle();
void listVehicles();
int getLastVehicleId();
char *getVehicleStatusText(Vehicle *v);

void registerVehicle()
{
    const size_t maxStrLength = 100;

    Vehicle v;
    v.cod = getLastVehicleId() + 1;
    v.status = 0;

    printf("Por favor, informe a descricao do veiculo: ");
    fgets(v.descricao, maxStrLength, stdin);
    printf("Por favor, informe a modelo do veiculo: ");
    fgets(v.modelo, maxStrLength, stdin);
    printf("Por favor, informe a cor do veiculo: ");
    fgets(v.cor, maxStrLength, stdin);
    printf("Por favor, informe a placa do veiculo: ");
    fgets(v.placa, maxStrLength, stdin);
    printf("Por favor, informe o valor da diaria do veiculo: ");
    scanf("%lf", &v.valorDiaria);
    printf("Por favor, informe a quantidade de ocupantes do veiculo: ");
    scanf("%d", &v.qntOcupantes);

    removeTrailingNewline(v.descricao);
    removeTrailingNewline(v.modelo);
    removeTrailingNewline(v.cor);
    removeTrailingNewline(v.placa);

    FILE *fVehiclePtr = fopen(vehicleDbFile, "a+");
    fwrite(&v, sizeof(Vehicle), 1, fVehiclePtr);
    if (fwrite == 0)
        printf("erro interno ao cadastrar veiculo.\n");
    else
        printf("Veiculo %s cadastrado. Cod do cliente: %d\n", v.descricao, v.cod);
    fclose(fVehiclePtr);
}

void listVehicles()
{
    FILE *fVehiclePtr = fopen(vehicleDbFile, "r");
    if (fVehiclePtr == NULL) // Arquivo não existe
    {
        printf("* Nenhum veiculo cadastrado.\n");
        return;
    }

    printf("* COD - DESCRICAO : MODELO : COR : PLACA : VALOR DIARIA : QNT. OCUPANTES : STATUS\n");
    Vehicle v;
    while (fread(&v, sizeof(Vehicle), 1, fVehiclePtr))
        printf("* %d - %s : %s : %s : %s : R$%.2f : %d : %s\n",
               v.cod,
               v.descricao,
               v.modelo,
               v.cor,
               v.placa,
               v.valorDiaria,
               v.qntOcupantes,
               v.status ? "Indisponivel" : "Disponivel");

    fclose(fVehiclePtr);
}

int getLastVehicleId()
{
    int lastId = -1;
    FILE *fVehiclePtr = fopen(vehicleDbFile, "r");
    if (fVehiclePtr == NULL) // Arquivo não existe
        return lastId;

    // lê todos os veiculos
    Vehicle v;
    while (fread(&v, sizeof(Vehicle), 1, fVehiclePtr))
        lastId = v.cod;

    fclose(fVehiclePtr);
    return lastId;
}

char *getVehicleStatusText(Vehicle *v)
{
    if (v == NULL)
        return "Erro interno. Veiculo NULL";

    switch (v->status)
    {
    case VEHICLE_STATUS_AVAILABLE:
        return "Alugado";
    case VEHICLE_STATUS_LEASED:
        return "Disponível";
    case VEHICLE_STATUS_MAINTENANCE:
        return "Em manutenção";
    default:
        return "Erro interno. Status inesperado";
    }
}

const VehicleRepo vehicleRepo = {
    .listVehicles = &listVehicles,
    .registerVehicle = &registerVehicle};