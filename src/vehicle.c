#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.c"

typedef struct
{
    int cod;
    char *descricao;
    char *modelo;
    char *cor;
    char *placa;
    double valorDiaria;
    int qntOcupantes;
    int status; // 0 -> livre, 1 -> ocupado
} Vehicle;

// prototypes
void registerVehicle();
void listVehicles();
int getLastVehicleId();

void registerVehicle()
{
    const size_t maxStrLength = 100;

    Vehicle v;
    v.cod = getLastVehicleId() + 1;
    v.descricao = malloc(maxStrLength);
    v.modelo = malloc(maxStrLength);
    v.cor = malloc(maxStrLength);
    v.placa = malloc(maxStrLength);
    v.valorDiaria = 0.0;
    v.qntOcupantes = 0;
    v.status = 0; // 0 -> livre, 1 -> ocupado

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

    FILE *fClientsPtr = fopen("vehicle_db.txt", "a+");
    fprintf(fClientsPtr, "%d|%s|%s|%s|%s|%lf|%d|%d\n",
            v.cod,
            v.descricao,
            v.modelo,
            v.cor,
            v.placa,
            v.valorDiaria,
            v.qntOcupantes,
            v.status);
    fclose(fClientsPtr);

    printf("Veiculo %s cadastrado. Cod do cliente: %d\n", v.descricao, v.cod);

    free(v.descricao);
    free(v.modelo);
    free(v.cor);
    free(v.placa);
}

void listVehicles()
{
    FILE *fClientsPtr = fopen("vehicle_db.txt", "a+");
    char *line = NULL;
    size_t len = 0;
    printf("* COD - DESCRICAO : MODELO : COR : PLACA : VALOR DIARIA : QNT. OCUPANTES : STATUS\n");
    while (getline(&line, &len, fClientsPtr) != -1)
    {
        const size_t maxStrLength = 100;
        Vehicle v;
        v.cod = getLastVehicleId() + 1;
        v.descricao = malloc(maxStrLength);
        v.modelo = malloc(maxStrLength);
        v.cor = malloc(maxStrLength);
        v.placa = malloc(maxStrLength);
        v.valorDiaria = 0.0;
        v.qntOcupantes = 0;
        v.status = 0; // 0 -> livre, 1 -> ocupado

        // https://stackoverflow.com/questions/2854488/reading-a-string-with-spaces-with-sscanf
        sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%lf|%d|%d",
               &v.cod,
               v.descricao,
               v.modelo,
               v.cor,
               v.placa,
               &v.valorDiaria,
               &v.qntOcupantes,
               &v.status);

        printf("* %d - %s : %s : %s : %s : R$%.2f : %d : %s\n",
               v.cod,
               v.descricao,
               v.modelo,
               v.cor,
               v.placa,
               v.valorDiaria,
               v.qntOcupantes,
               v.status ? "Indisponivel" : "Disponivel");

        free(v.descricao);
        free(v.modelo);
        free(v.cor);
        free(v.placa);
    }
    fclose(fClientsPtr);
}

int getLastVehicleId()
{
    FILE *fClientsPtr = fopen("vehicle_db.txt", "a+");
    char *line = NULL;
    size_t len = 0;
    int lastId = -1;
    while (getline(&line, &len, fClientsPtr) != -1)
    {
        sscanf(line, "%d", &lastId);
    }
    fclose(fClientsPtr);

    return lastId;
}