/* vehicle.c
 *
 * Este arquivo representa a implementação de funções para manipulação
 * de veiculos no sistema.
 *
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "vehicle.h"
#include "utils.h"

static int get_last_vehicle_id(void);
static char *get_vehicle_status_text(Vehicle *v);

char *vehicle_db_file = "_db_vehicle.dat";

static int get_last_vehicle_id(void)
{
    int lastId = -1;
    FILE *fVehiclePtr = fopen(vehicle_db_file, "r");
    if (fVehiclePtr == NULL) // Arquivo não existe
        return lastId;

    // lê todos os veiculos
    Vehicle v;
    while (fread(&v, sizeof(Vehicle), 1, fVehiclePtr))
        lastId = v.cod;

    fclose(fVehiclePtr);
    return lastId;
}

static char *get_vehicle_status_text(Vehicle *v)
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

int vehicle_register(char *desc, char *model, char *color, char *registration_plate,
                     double charge_per_day, int passenger_capacity)
{
    Vehicle v = {
        .cod = get_last_vehicle_id() + 1,
        .valorDiaria = charge_per_day,
        .qntOcupantes = passenger_capacity,
        .status = VEHICLE_STATUS_AVAILABLE};

    strcpy(v.descricao, desc);
    strcpy(v.modelo, model);
    strcpy(v.cor, color);
    strcpy(v.placa, registration_plate);

    FILE *f_ptr = fopen(vehicle_db_file, "a+");
    if (fwrite(&v, sizeof(Vehicle), 1, f_ptr) != 1)
    {
        printf("erro interno ao cadastrar veiculo.\n");
        fclose(f_ptr);
        return EXIT_FAILURE;
    }

    printf("Veiculo %s cadastrado. Cod do veiculo: %d\n", v.descricao, v.cod);
    fclose(f_ptr);
    return EXIT_SUCCESS;
}

void vehicle_list(void)
{
    FILE *fVehiclePtr = fopen(vehicle_db_file, "r");
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
               get_vehicle_status_text(&v));

    fclose(fVehiclePtr);
}

int vehicle_find_one_by_capacity(int cap, Vehicle *v)
{
    FILE *f_ptr = fopen(vehicle_db_file, "r");
    if (f_ptr == NULL) // Arquivo não existe
        return 0;

    int vCod = -1;
    int vCap = INT_MAX;
    Vehicle vehicle;
    while (fread(&vehicle, sizeof(Vehicle), 1, f_ptr))
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
        fseek(f_ptr, sizeof(Vehicle) * vCod, SEEK_SET);
        fread(v, sizeof(Vehicle), 1, f_ptr);
    }

    fclose(f_ptr);
    return found;
}

int vehicle_update_status(int cod, int new_status)
{
    FILE *f_ptr = fopen(vehicle_db_file, "r+");
    if (f_ptr == NULL) // Arquivo não existe, logo veiculo não existe
        return 0;

    // https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
    fseek(f_ptr, 0L, SEEK_END);
    long size = ftell(f_ptr);
    rewind(f_ptr);

    // como o id do cliente é incremental
    long offset = sizeof(Vehicle) * cod;
    if (offset >= size) // se o offset é maior que o tamanho do arquivo, ou seja o veiculo não existe
        return 0;

    Vehicle v;
    fseek(f_ptr, offset, SEEK_SET);
    int exits = fread(&v, sizeof(Vehicle), 1, f_ptr) == 1;
    if (!exits)
    {
        printf("erro interno ao ler veiculo.\n");
        return 0;
    }

    fseek(f_ptr, offset, SEEK_SET);

    v.status = new_status;
    if (fwrite(&v, sizeof(Vehicle), 1, f_ptr) == 0)
    {
        printf("erro interno ao atualizar veiculo.\n");
        return 0;
    }
    else
    {
        printf("Veiculo %s atualizado. Cod do Veiculo: %d\n", v.descricao, v.cod);
    }

    fclose(f_ptr);
    return exits;
}

int vehicle_get_by_cod(int cod, Vehicle *v)
{
    FILE *f_ptr = fopen(vehicle_db_file, "r");
    if (f_ptr == NULL) // Arquivo não existe, logo veiculo não existe
        return EXIT_FAILURE;

    // https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
    fseek(f_ptr, 0L, SEEK_END);
    long size = ftell(f_ptr);
    rewind(f_ptr);

    // como o id do cliente é incremental
    long offset = sizeof(Vehicle) * cod;
    if (offset >= size) // se o offset é maior que o tamanho do arquivo, ou seja a veiculo não existe
    {
        fclose(f_ptr);
        return EXIT_FAILURE;
    }

    fseek(f_ptr, offset, SEEK_SET);
    if (fread(v, sizeof(Vehicle), 1, f_ptr) != 1)
    {
        fclose(f_ptr);
        return EXIT_FAILURE;
    }

    fclose(f_ptr);
    return EXIT_SUCCESS;
}
