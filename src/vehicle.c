#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "vehicle.h"
#include "utils.h"

// global
const char *vehicleDbFile = "vehicle_db.dat";

// prototypes
void registerVehicle();
void listVehicles();
int getLastVehicleId();
char *getVehicleStatusText(Vehicle *v);
int findVehicleWithCapacity(int cap, Vehicle *v);

void registerVehicle()
{
    const size_t maxStrLength = 100;

    Vehicle v;
    v.cod = getLastVehicleId() + 1;
    v.status = VEHICLE_STATUS_AVAILABLE;

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
               getVehicleStatusText(&v));

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
        return "Disponível";
    case VEHICLE_STATUS_LEASED:
        return "Alugado";
    case VEHICLE_STATUS_MAINTENANCE:
        return "Em manutenção";
    default:
        return "Erro interno. Status inesperado";
    }
}

int findVehicleWithCapacity(int cap, Vehicle *v)
{
    FILE *fPtr = fopen(vehicleDbFile, "r");
    if (fPtr == NULL) // Arquivo não existe
        return 0;

    int vCod = -1;
    int vCap = INT_MAX;
    Vehicle vehicle;
    while (fread(&vehicle, sizeof(Vehicle), 1, fPtr))
    {
        // Procuro por um veiculo que tenha a capacidade que o cliente procura.
        // Se não encontrar, vai buscando o mais próximo da capacidade.
        if (vehicle.qntOcupantes >= cap && vehicle.status == VEHICLE_STATUS_AVAILABLE && vehicle.qntOcupantes < vCap)
        {
            vCod = vehicle.cod;
            vCap = vehicle.qntOcupantes;
            if (vehicle.qntOcupantes == cap)
                break;
        }
    }

    int found = 0;
    if (vCod != -1)
    {
        found = 1;
        fseek(fPtr, sizeof(Vehicle) * vCod, SEEK_SET);
        fread(v, sizeof(Vehicle), 1, fPtr);
    }

    fclose(fPtr);
    return found;
}

int updateVehicleStatus(int cod, int vehicleStatus)
{
    FILE *fPtr = fopen(vehicleDbFile, "r+");
    if (fPtr == NULL) // Arquivo não existe, logo veiculo não existe
        return 0;

    // https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
    fseek(fPtr, 0L, SEEK_END);
    long size = ftell(fPtr);
    rewind(fPtr);

    // como o id do cliente é incremental
    long offset = sizeof(Vehicle) * cod;
    if (offset >= size) // se o offset é maior que o tamanho do arquivo, ou seja o veiculo não existe
        return 0;

    Vehicle v;
    fseek(fPtr, offset, SEEK_SET);
    int exits = fread(&v, sizeof(Vehicle), 1, fPtr) == 1;
    if (!exits)
    {
        printf("erro interno ao ler veiculo.\n");
        return 0;
    }

    fseek(fPtr, offset, SEEK_SET);

    v.status = vehicleStatus;
    if (fwrite(&v, sizeof(Vehicle), 1, fPtr) == 0)
    {
        printf("erro interno ao atualizar veiculo.\n");
        return 0;
    }
    else
    {
        printf("Veiculo %s atualizado. Cod do Veiculo: %d\n", v.descricao, v.cod);
    }

    fclose(fPtr);
    return exits;
}

const VehicleRepo vehicleRepo = {
    .listVehicles = &listVehicles,
    .registerVehicle = &registerVehicle,
    .findVehicleWithCapacity = &findVehicleWithCapacity,
    .updateVehicleStatus = &updateVehicleStatus,
};