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

static double calc_days_between_dates(time_t date_a, time_t date_b);
static time_t get_date_at_midnight(time_t date);
static double calc_late_fee_amount(int days_of_delay, double lease_value);

static const int SECS_IN_DAY = (60 * 60 * 24);
// O valor do seguro é de R$ 50,00
static const double INSURANCE_PRICE = 50.0;
// multa: ... + R$ 30,00 por dia de atraso
static const double FINE_AMOUNT_PER_DAY = 30.0;

static double calc_days_between_dates(time_t date_a, time_t date_b)
{
    return (date_a - date_b) / (double)SECS_IN_DAY;
}

static time_t get_date_at_midnight(time_t date)
{
    // o calculo é simples, basta remover os segundos além da meia noite
    return date - (date % SECS_IN_DAY);
}

static double calc_late_fee_amount(int days_of_delay, double lease_value)
{
    if (days_of_delay == 0)
        return 0.0;

    // multa: valor de 5% do valor total da locação + R$ 30,00 por dia de atraso
    double lease_value_percent = lease_value * 0.05;
    double value_days_late = days_of_delay * FINE_AMOUNT_PER_DAY;
    return lease_value_percent + value_days_late;
}

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

int core_finalize_lease(int location_cod, time_t final_return_date)
{
    // Implemente uma função que dê baixa em uma determinada locação, calcule e mostre o
    // valor total a ser pago por um determinado cliente. Lembre-se de alterar o status do
    // veículo para disponível. Além disso, será necessário solicitar a data de entrega do veículo
    // para validar com a data de devolução prevista. Também será preciso calcular multa por
    // atraso, caso o cliente não entregue o veículo no dia combinado (considere como multa o
    // valor de 5% do valor total da locação + R$ 30,00 por dia de atraso)

    Lease l;
    if (lease_get_by_cod(location_cod, &l))
    {
        fprintf(stderr, "Não foi encontrado a locação\n");
        return EXIT_FAILURE;
    }

    if (l.finished)
    {
        fprintf(stderr, "A locação já foi finalizada\n");
        return EXIT_FAILURE;
    }

    // Busca o cliente e veiculo

    Client c;
    if (client_get_by_cod(l.clientCod, &c))
    {
        fprintf(stderr, "Erro interno ao buscar cliente\n");
        return EXIT_FAILURE;
    }

    Vehicle v;
    if (vehicle_get_by_cod(l.vehicleCod, &v))
    {
        fprintf(stderr, "Erro interno ao busca veiculo\n");
        return EXIT_FAILURE;
    }

    // calculo de valor pago pelo cliente
    const double leaseValue = core_calc_lease_value(l.withdrawalDate, l.returnDate, final_return_date, v.valorDiaria, l.hasInsurance);
    printf("Valor final locação: R$ %.2f\n", leaseValue);

    // Altera status da locação para finalizada
    if (lease_finalize(l.cod))
    {
        fprintf(stderr, "Erro interno ao finalizar a locação\n");
        return EXIT_FAILURE;
    }

    // Altera status do veiculo para disponivel
    if (vehicle_update_status(v.cod, VEHICLE_STATUS_AVAILABLE))
    {
        fprintf(stderr, "Erro interno ao alterar status do veiculo para disponivel\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

double core_calc_lease_value(time_t withdrawal_date, time_t return_date, time_t final_return_date, double charge_per_day, int has_insurance)
{
    // Compara as datas definido o horário para 00:00 e salva em int para truncar o valor
    int dailys = calc_days_between_dates(get_date_at_midnight(return_date), get_date_at_midnight(withdrawal_date));
    const int daysOfDelay = calc_days_between_dates(get_date_at_midnight(final_return_date), get_date_at_midnight(return_date));
    // no minimo uma diária é cobrada
    dailys = dailys == 0 ? 1 : dailys;

    const double leaseValueWithoutAdditions = dailys * charge_per_day;
    const double leaseValueWithInsurance = leaseValueWithoutAdditions + (has_insurance ? INSURANCE_PRICE : 0.0);
    const double lateFeeAmount = calc_late_fee_amount(daysOfDelay, leaseValueWithoutAdditions);
    const double leaseValue = leaseValueWithInsurance + lateFeeAmount;

    printf("CALCULO VALOR LOCAÇÃO:\n");
    printf("Valor da locação sem SEGURO: R$ %.2f\n", leaseValueWithoutAdditions);
    printf("Valor da locação com SEGURO: R$ %.2f\n", leaseValueWithInsurance);
    printf("Valor da multa por atraso: R$ %.2f\n", lateFeeAmount);
    printf("Valor final locação: R$ %.2f\n", leaseValue);
    return leaseValue;
}