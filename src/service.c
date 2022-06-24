#include <stdio.h>
#include <time.h>
#include <string.h>
#include "service.h"
#include "client.h"
#include "location.h"
#include "vehicle.h"
#include "utils.h"

// Tipos privados

// Prototipos privados

int serviceRegisterClient(const char *name, const char *address);
int serviceRegisterVehicle(char *descricao, char *modelo, char *cor,
                           char *placa, double valorDiaria, int qntOcupantes);
void serviceRegisterLocation(void);
void serviceListClients(void);
void serviceListVehicles(void);
void serviceListLocations(void);
void serviceEndLocation(void);

// Variaveis globais

const Service service = {
    .registerClient = &serviceRegisterClient,
    .registerVehicle = &serviceRegisterVehicle,
    .registerLocation = &serviceRegisterLocation,
    .listClients = &serviceListClients,
    .listVehicles = &serviceListVehicles,
    .listLocations = &serviceListLocations,
    .endLocation = &serviceEndLocation};

// Implementações

int readDateFromStdin(struct tm *date);

int serviceRegisterClient(const char *name, const char *address)
{
    return clientsRepo.registerClient(name, address);
}

int serviceRegisterVehicle(char *descricao, char *modelo, char *cor,
                           char *placa, double valorDiaria, int qntOcupantes)
{
    return vehicleRepo.registerVehicle(descricao, modelo, cor, placa, valorDiaria, qntOcupantes);
}

void serviceRegisterLocation()
{
    int clientCod;
    int vehicleCap;

    printf("Por favor, informe o cod do cliente: ");
    scanf("%d", &clientCod);
    getchar();
    Client c;
    if (!clientsRepo.clientExists(clientCod, &c))
    {
        printf("Não foi encontrado o cliente com o código informado\n");
        return;
    }
    printf("Cliente %s\n", c.name);

    printf("Por favor, informe a data da retirada no formato \"dd/mm/aaaa hh:mm\": ");
    struct tm withdrawalDate;
    if (!readDateFromStdin(&withdrawalDate))
    {
        printf("A data inserida é inválida\n");
        return;
    }

    printf("Por favor, informe a data de devolução no formato \"dd/mm/aaaa hh:mm\": ");
    struct tm returnDate;
    if (!readDateFromStdin(&returnDate))
    {
        printf("A data inserida é inválida\n");
        return;
    }

    char bufFmtWithdrawalDate[20];
    char bufFmtReturnDate[20];
    formatDate(&withdrawalDate, bufFmtWithdrawalDate, sizeof(bufFmtWithdrawalDate));
    formatDate(&returnDate, bufFmtReturnDate, sizeof(bufFmtReturnDate));
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
    if (!vehicleRepo.findVehicleWithCapacity(vehicleCap, &v))
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

    locationRepo.registerLocation(
        epochWithdrawalDate,
        epochReturnDate,
        hasInsurance,
        clientCod,
        v.cod);

    vehicleRepo.updateVehicleStatus(v.cod, VEHICLE_STATUS_LEASED);
}

void serviceListClients()
{
    clientsRepo.listClients();
}

void serviceListVehicles()
{
    vehicleRepo.listVehicles();
}

void serviceListLocations()
{
    locationRepo.listLocations();
}

int readDateFromStdin(struct tm *date)
{
    const size_t maxStrLength = 100;
    char dateStr[maxStrLength];
    fgets(dateStr, maxStrLength, stdin);
    removeTrailingNewline(dateStr);

    int wdDay, wdMon, wdYear, wdHour, wdMin;
    if (sscanf(dateStr, "%d/%d/%d %d:%d", &wdDay, &wdMon, &wdYear, &wdHour, &wdMin) != 5)
        return 0;

    time_t tNow = time(NULL);
    struct tm tmNow = *localtime(&tNow);

    // Ajusta a data para tm
    memset(date, 0, sizeof(struct tm));
    date->tm_mday = wdDay;
    date->tm_mon = wdMon - 1;
    date->tm_year = wdYear - 1900;
    date->tm_hour = wdHour;
    date->tm_min = wdMin;

    time_t t = mktime(date);
    if (t == -1)
        return 0;

    return 1;
}

void serviceEndLocation()
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
    Location l;
    if (!locationRepo.getLocation(locationCod, &l))
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
    if (!clientsRepo.clientExists(l.clientCod, &c))
    {
        printf("Erro interno ao buscar cliente\n");
        return;
    }

    Vehicle v;
    if (!vehicleRepo.getVehicle(l.vehicleCod, &v))
    {
        printf("Erro interno ao busca veiculo\n");
        return;
    }

    printf("Por favor, informe a data que o veiculo foi devolvido no formato \"dd/mm/aaaa hh:mm\": ");
    struct tm finalReturnDate;
    if (!readDateFromStdin(&finalReturnDate))
    {
        printf("A data inserida é inválida\n");
        return;
    }

    struct tm withdrawalDate = *localtime(&l.withdrawalDate);
    struct tm returnDate = *localtime(&l.returnDate);

    char bufFmtWithdrawalDate[20];
    char bufFmtReturnDate[20];
    formatDate(&withdrawalDate, bufFmtWithdrawalDate, sizeof(bufFmtWithdrawalDate));
    formatDate(&returnDate, bufFmtReturnDate, sizeof(bufFmtReturnDate));

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
    formatDate(&finalReturnDate, bufFmtFinalReturnDate, sizeof(bufFmtFinalReturnDate));

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
    if (!locationRepo.finalizeLease(l.cod))
    {
        fprintf(stderr, "Erro interno ao finalizar a locação\n");
        return;
    }

    // Altera status do veiculo para disponivel
    if (!vehicleRepo.updateVehicleStatus(v.cod, VEHICLE_STATUS_AVAILABLE))
    {
        fprintf(stderr, "Erro interno ao alterar status do veiculo para disponivel\n");
        return;
    }
}
