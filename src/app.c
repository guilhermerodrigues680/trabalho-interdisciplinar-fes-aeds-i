/* app.c
 *
 * Este arquivo representa a interface com o usuário via linha de comando (CLI).
 * O objetivo do app.c é fornecer uma forma de coletar dados do usuário pela linha de comando.
 *
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "app.h"
#include "core.h"
#include "utils.h"
#include "client.h"

void app_register_client(void)
{
    const size_t maxStrLength = 100;
    char name[maxStrLength];
    char address[maxStrLength];

    printf("Por favor, informe o nome do cliente: ");
    fgets(name, maxStrLength, stdin);
    printf("Por favor, informe o endereço do cliente: ");
    fgets(address, maxStrLength, stdin);

    utils_remove_trailing_newline(name);
    utils_remove_trailing_newline(address);

    if (core_register_client(name, address))
        fprintf(stderr, "erro interno ao cadastrar cliente.\n");

    printf("Cliente cadastrado com sucesso!\n");
}

void app_register_vehicle(void)
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

    utils_remove_trailing_newline(descricao);
    utils_remove_trailing_newline(modelo);
    utils_remove_trailing_newline(cor);
    utils_remove_trailing_newline(placa);

    if (core_register_vehicle(descricao, modelo, cor, placa, valorDiaria, qntOcupantes))
        fprintf(stderr, "erro interno ao cadastrar veiculo.\n");

    printf("Veiculo cadastrado com sucesso!\n");
}

void app_register_lease(void)
{
    int clientCod;
    int vehicleCap;

    printf("Por favor, informe o cod do cliente: ");
    scanf("%d", &clientCod);
    getchar();
    Client c;
    if (core_get_client(clientCod, &c))
    {
        printf("Não foi encontrado o cliente com o código informado\n");
        return;
    }
    printf("Cliente %s\n", c.name);

    printf("Por favor, informe a data da retirada no formato \"dd/mm/aaaa hh:mm\": ");
    struct tm withdrawalDate;
    if (!utils_read_date_from_stdin(&withdrawalDate))
    {
        printf("A data inserida é inválida\n");
        return;
    }

    printf("Por favor, informe a data de devolução no formato \"dd/mm/aaaa hh:mm\": ");
    struct tm returnDate;
    if (!utils_read_date_from_stdin(&returnDate))
    {
        printf("A data inserida é inválida\n");
        return;
    }

    char bufFmtWithdrawalDate[20];
    char bufFmtReturnDate[20];
    utils_format_date(&withdrawalDate, bufFmtWithdrawalDate, sizeof(bufFmtWithdrawalDate));
    utils_format_date(&returnDate, bufFmtReturnDate, sizeof(bufFmtReturnDate));
    printf("Periodo de locação: %s -> %s\n", bufFmtWithdrawalDate, bufFmtReturnDate);

    time_t epochWithdrawalDate = mktime(&withdrawalDate);
    time_t epochReturnDate = mktime(&returnDate);

    // Compara as datas, definido o horário para 00:00
    const int secsInDay = (60 * 60 * 24);
    const time_t epochWithdrawalDate0000 = epochWithdrawalDate - (epochWithdrawalDate % secsInDay);
    const time_t epochReturnDate0000 = epochReturnDate - (epochReturnDate % secsInDay);
    if (epochWithdrawalDate0000 > epochReturnDate0000)
    {
        printf("Erro: A data de retorno deve ser após a data de retirada\n");
        return;
    }
    else if (epochWithdrawalDate == epochReturnDate)
    {
        printf("Retirada e retorno no mesmo dia\n");
    }

    printf("Por favor, informe a quantidade de ocupantes desejada: ");
    // https://faq.cprogramming.com/cgi-bin/smartfaq.cgi?answer=1352443831&id=1043284392#:~:text=For%20a%20simple%20solution%2C%20you,with%20the%20other%20format%20specifiers.
    scanf("%d", &vehicleCap);
    getchar();
    Vehicle v;
    if (!core_find_vehicle_with_capacity(vehicleCap, &v))
    {
        printf("Não há nenhum veiculo disponivel que atenda essa capacidade\n");
        return;
    }
    printf("Veiculo %s, %s, %s , %d lugares\n", v.descricao, v.modelo, v.cor, v.qntOcupantes);

    char resDesejaSeguro;
    printf("Por favor, informe se o cliente deseja seguro (s/n): ");
    // https://faq.cprogramming.com/cgi-bin/smartfaq.cgi?answer=1352443831&id=1043284392#:~:text=For%20a%20simple%20solution%2C%20you,with%20the%20other%20format%20specifiers.
    scanf(" %c", &resDesejaSeguro);
    getchar();

    if (resDesejaSeguro != 's' && resDesejaSeguro != 'n')
    {
        printf("Use s ou n\n");
        return;
    }

    int hasInsurance = resDesejaSeguro == 's' ? 1 : 0;

    if (core_register_lease(epochWithdrawalDate, epochReturnDate, hasInsurance, clientCod, v.cod))
        fprintf(stderr, "erro interno ao cadastrar locação.\n");

    printf("Locação cadastrado com sucesso!\n");
}

void app_list_clients(void)
{
    core_list_clients();
}

void app_list_vehicles(void)
{
    core_list_vehicles();
}

void app_list_leases(void)
{
    core_list_leases();
}

void app_finalize_lease(void)
{
    core_finalize_lease();
}
