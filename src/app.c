/* app.c
 *
 * Este arquivo representa a interface com o usuário via linha de comando (CLI).
 *
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "app.h"
#include "service.h"
#include "utils.h"

// Tipos

// Prototipos privados

// Variaveis globais

// Implementações

void appRegisterClient(void)
{
    const size_t maxStrLength = 100;
    char name[maxStrLength];
    char address[maxStrLength];

    printf("Por favor, informe o nome do cliente: ");
    fgets(name, maxStrLength, stdin);
    printf("Por favor, informe o endereço do cliente: ");
    fgets(address, maxStrLength, stdin);

    removeTrailingNewline(name);
    removeTrailingNewline(address);

    if (service.registerClient(name, address))
        fprintf(stderr, "erro interno ao cadastrar cliente.\n");

    printf("Cliente cadastrado com sucesso!\n");
}

void appRegisterVehicle(void)
{
    char descricao[100];
    char modelo[30];
    char cor[30];
    char placa[15];
    double valorDiaria;
    int qntOcupantes;

    printf("Por favor, informe a descricao do veiculo: ");
    fgets(descricao, sizeof(descricao), stdin);
    printf("Por favor, informe a modelo do veiculo: ");
    fgets(modelo, sizeof(modelo), stdin);
    printf("Por favor, informe a cor do veiculo: ");
    fgets(cor, sizeof(cor), stdin);
    printf("Por favor, informe a placa do veiculo: ");
    fgets(placa, sizeof(placa), stdin);
    printf("Por favor, informe o valor da diaria do veiculo: ");
    scanf("%lf", &valorDiaria);
    printf("Por favor, informe a quantidade de ocupantes do veiculo: ");
    scanf("%d", &qntOcupantes);
    getchar();

    removeTrailingNewline(descricao);
    removeTrailingNewline(modelo);
    removeTrailingNewline(cor);
    removeTrailingNewline(placa);

    if (service.registerVehicle(descricao, modelo, cor, placa, valorDiaria, qntOcupantes))
        fprintf(stderr, "erro interno ao cadastrar veiculo.\n");

    printf("Veiculo cadastrado com sucesso!\n");
}

void appRegisterLocation(void)
{
    service.registerLocation();
}

void appListClients(void)
{
    service.listClients();
}

void appListVehicles(void)
{
    service.listVehicles();
}

void appListLocations(void)
{
    service.listLocations();
}

void appEndLocation(void)
{
    service.endLocation();
}
