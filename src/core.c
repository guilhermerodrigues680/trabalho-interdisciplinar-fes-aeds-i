/* core.c
 *
 * Este arquivo representa os serviços que o sistema realiza.
 * Aqui não há interface com o usuário, somente funções para realizar as regras de negócio.
 * A separação das camadas de aplicação e serviço é importante para realizar testes com
 * entradas arbitrárias. Uma vez que a camada de aplicação usa funções que os testes
 * não conseguem executar, como o `scanf`.
 *
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "core.h"
#include "client.h"
#include "lease.h"
#include "vehicle.h"
#include "utils.h"

int core_register_client(const char *name, const char *address)
{
    return client_register(name, address);
}

int core_register_vehicle(char *desc, char *model, char *color, char *registration_plate,
                          double charge_per_day, int passenger_capacity)
{
    return vehicle_register(desc, model, color, registration_plate, charge_per_day, passenger_capacity);
}

int core_get_client(int cod, Client *client)
{
    return client_get_by_cod(cod, client);
}

int core_find_vehicle_with_capacity(int cap, Vehicle *v)
{
    return vehicle_find_one_by_capacity(cap, v);
}

int core_register_lease(time_t withdrawal_date, time_t return_date, int has_insurance,
                        int client_cod, int vehicle_cod)
{
    // struct tm withdrawalDate = *localtime(&l.withdrawalDate);
    // struct tm returnDate = *localtime(&l.returnDate);

    // valida os dados fornecidos

    Client c;
    if (core_get_client(client_cod, &c))
    {
        fprintf(stderr, "Erro: cliente não econtrado\n");
        return EXIT_FAILURE;
    }

    Vehicle v;
    if (vehicle_get_by_cod(vehicle_cod, &v))
    {
        fprintf(stderr, "Erro: Veiculo cliente não econtrado\n");
        return EXIT_FAILURE;
    }

    // Validar quantidade de ocupantes
    // v.qntOcupantes >=

    const int secsInDay = (60 * 60 * 24);
    const time_t withdrawalDate0000 = withdrawal_date - (withdrawal_date % secsInDay);
    const time_t returnDate0000 = return_date - (return_date % secsInDay);
    if (withdrawalDate0000 > returnDate0000)
    {
        fprintf(stderr, "Erro: A data de retorno deve ser após a data de retirada\n");
        return EXIT_FAILURE;
    }

    if (lease_register(withdrawal_date, return_date, has_insurance, client_cod, vehicle_cod))
    {
        fprintf(stderr, "erro interno ao registrar locação.\n");
        return EXIT_FAILURE;
    }

    if (vehicle_update_status(vehicle_cod, VEHICLE_STATUS_LEASED))
    {
        fprintf(stderr, "erro interno ao atualizar status do veiculo.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void core_list_clients(void)
{
    client_list();
}

void core_list_vehicles(void)
{
    vehicle_list();
}

void core_list_leases(void)
{
    lease_list();
}

void core_finalize_lease(void)
{
    // Implemente uma função que dê baixa em uma determinada locação, calcule e mostre o
    // valor total a ser pago por um determinado cliente. Lembre-se de alterar o status do
    // veículo para disponível. Além disso, será necessário solicitar a data de entrega do veículo
    // para validar com a data de devolução prevista. Também será preciso calcular multa por
    // atraso, caso o cliente não entregue o veículo no dia combinado (considere como multa o
    // valor de 5% do valor total da locação + R$ 30,00 por dia de atraso)

    int locationCod;

    printf("serviceEndLocation\n");

    printf("Por favor, informe o cod da locação: ");
    scanf("%d", &locationCod);
    getchar();
    Lease l;
    if (!lease_get_by_cod(locationCod, &l))
    {
        fprintf(stderr, "Não foi encontrado a locação com o código informado\n");
        return;
    }

    if (l.finished)
    {
        fprintf(stderr, "A locação já foi finalizada\n");
        return;
    }

    printf("Locação encontrada\n");

    // calculo de valor pago pelo cliente

    // Busca o cliente e veiculo

    Client c;
    if (client_get_by_cod(l.clientCod, &c))
    {
        printf("Erro interno ao buscar cliente\n");
        return;
    }

    Vehicle v;
    if (vehicle_get_by_cod(l.vehicleCod, &v))
    {
        printf("Erro interno ao busca veiculo\n");
        return;
    }

    printf("Por favor, informe a data que o veiculo foi devolvido no formato \"dd/mm/aaaa hh:mm\": ");
    struct tm finalReturnDate;
    if (!utils_read_date_from_stdin(&finalReturnDate))
    {
        printf("A data inserida é inválida\n");
        return;
    }

    struct tm withdrawalDate = *localtime(&l.withdrawalDate);
    struct tm returnDate = *localtime(&l.returnDate);

    char bufFmtWithdrawalDate[20];
    char bufFmtReturnDate[20];
    utils_format_date(&withdrawalDate, bufFmtWithdrawalDate, sizeof(bufFmtWithdrawalDate));
    utils_format_date(&returnDate, bufFmtReturnDate, sizeof(bufFmtReturnDate));

    time_t epochWithdrawalDate = mktime(&withdrawalDate);
    time_t epochReturnDate = mktime(&returnDate);

    // Compara as datas, definido o horário para 00:00
    const int secsInDay = (60 * 60 * 24);
    const time_t epochWithdrawalDate0000 = epochWithdrawalDate - (epochWithdrawalDate % secsInDay);
    const time_t epochReturnDate0000 = epochReturnDate - (epochReturnDate % secsInDay);

    // int trunca o valor, e no minimo uma diária é cobrada.
    int dailys = (epochReturnDate0000 - epochWithdrawalDate0000) / secsInDay;
    dailys = dailys == 0 ? 1 : dailys;

    // Compara as datas, definido o horário para 00:00
    time_t epochFinalReturnDate = mktime(&finalReturnDate);
    const time_t epochFinalReturnDate0000 = epochFinalReturnDate - (epochFinalReturnDate % secsInDay);

    int daysOfDelay = 0;
    if (epochFinalReturnDate0000 > epochReturnDate0000)
    {
        printf("DEBUG: Veiculo devolvido após a data prevista\n");
        // int trunca o valor, e no minimo uma diária é cobrada.
        // TODO: checar, calculo de dias de atraso
        daysOfDelay = (epochFinalReturnDate0000 - epochReturnDate0000) / secsInDay;
    }

    // TODO: metodo para calcular valor da multa
    double valueWithoutAdditions = dailys * v.valorDiaria;
    // multa: valor de 5% do valor total da locação + R$ 30,00 por dia de atraso
    const double fineAmountPerDay = 30.0;
    const double INSURANCE_PRICE = 50.0;
    double lateFeePercentage = valueWithoutAdditions * (5.0 / 100.0);
    double lateFeeDaysOfDelay = daysOfDelay * fineAmountPerDay;
    double lateFee = lateFeePercentage + lateFeeDaysOfDelay;
    double totalValue = valueWithoutAdditions + lateFee;
    double insurancePrice = l.hasInsurance ? INSURANCE_PRICE : 0;

    char bufFmtFinalReturnDate[20];
    utils_format_date(&finalReturnDate, bufFmtFinalReturnDate, sizeof(bufFmtFinalReturnDate));

    printf("Cliente %s\n", c.name);
    printf("Veiculo %s, %s, %s\n", v.descricao, v.modelo, v.cor);
    printf("Periodo de locação: %s -> %s, devolução: %s\n", bufFmtWithdrawalDate, bufFmtReturnDate, bufFmtFinalReturnDate);
    if (epochWithdrawalDate == epochReturnDate)
        printf("Retirada e retorno no mesmo dia\n");
    printf("Total de %d diárias\n", dailys);
    printf("Total diaria: R$%.2f\n", v.valorDiaria);
    printf("Valor seguro: R$%.2f\n", insurancePrice);
    printf("Valor sem acrescimos: R$%.2f\n", valueWithoutAdditions);
    if (daysOfDelay == 0)
    {
        printf("Veiculo devolvido na data prevista\n");
        printf("Valor total: R$%.2f\n", valueWithoutAdditions + insurancePrice);
    }
    else
    {
        printf("Veiculo devolvido após a data prevista\n");
        printf("Dias atrasos: %d\n", daysOfDelay);
        printf("Valor 5%%: R$%.2f\n", lateFeePercentage);
        printf("Valor Dias atrasos: R$%.2f\n", lateFeeDaysOfDelay);
        printf("Valor da multa: R$%.2f\n", lateFee);
        printf("Valor com acrescimos: R$%.2f\n", totalValue);
        printf("Valor total: R$%.2f\n", totalValue + insurancePrice);
    }

    // TODO: calcular valor do aluguel e multa em uma funcao

    // Altera status da locação para finalizada
    if (!lease_finalize(l.cod))
    {
        fprintf(stderr, "Erro interno ao finalizar a locação\n");
        return;
    }

    // Altera status do veiculo para disponivel
    if (!vehicle_update_status(v.cod, VEHICLE_STATUS_AVAILABLE))
    {
        fprintf(stderr, "Erro interno ao alterar status do veiculo para disponivel\n");
        return;
    }
}
