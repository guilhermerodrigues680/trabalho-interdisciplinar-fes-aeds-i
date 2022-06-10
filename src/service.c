#include <stdio.h>
#include <time.h>
#include <string.h>
#include "service.h"
#include "client.h"
#include "location.h"
#include "vehicle.h"

int readDateFromStdin(struct tm *date);
formatDate(struct tm *date, char *buffer, size_t sizeBuffer);

void serviceRegisterClient()
{
    clientsRepo.registerClient();
}

void serviceRegisterVehicle()
{
    vehicleRepo.registerVehicle();
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
        printf("Não há nenhum veiculo que atenda essa capacidade\n");
        return;
    }
    printf("Veiculo %s, %s, %s , %d lugares\n", v.descricao, v.modelo, v.cor, v.qntOcupantes);

    // locationRepo.registerLocation(clientCod, v.cod);
}

void serviceListClients()
{
    clientsRepo.listClients();
}

void serviceListVehicles()
{
    vehicleRepo.listVehicles();
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

int formatDate(struct tm *date, char *buffer, size_t sizeBuffer)
{
    if (strftime(buffer, sizeBuffer, "%d/%m/%Y %H:%M", date) > sizeBuffer)
        return 0;
    else
        return 1;
}

const Service service = {
    .registerClient = &serviceRegisterClient,
    .registerVehicle = &serviceRegisterVehicle,
    .registerLocation = &serviceRegisterLocation,
    .listClients = &serviceListClients,
    .listVehicles = &serviceListVehicles};