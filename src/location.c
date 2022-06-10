#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.c"

typedef struct
{
    int cod;
    time_t withdrawalDate;
    time_t returnDate;
    double insurance;
    int clientCod;
    int vehicleCod;
} Location;

// global
const char *locationDbFile = "location_db.dat";

// prototypes
void registerLocation();
// void listLocations();
int getLastLocationId();

void registerLocation()
{
    const size_t maxStrLength = 100;

    Location l;
    l.cod = getLastVehicleId() + 1;
    // TODO: validar se cliente e veiculo existem
    // l.clientCod = -1;
    // l.vehicleCod = -1;
    // l.insurance

    char withdrawalDateStr[maxStrLength];
    printf("Por favor, informe a data da retirada no formato \"dd/mm/aaaa hh:mm\": ");
    fgets(withdrawalDateStr, maxStrLength, stdin);
    removeTrailingNewline(withdrawalDateStr);

    // time_t time_raw_format;
    // struct tm *ptr_time = localtime(&time_raw_format);
    // strftime(withdrawalDateStr, maxStrLength, "%d/%m/%Y %H:%M", ptr_time);

    int wdDay;
    int wdMon;
    int wdYear;
    int wdHour;
    int wdMin;
    if (sscanf(withdrawalDateStr, "%d/%d/%d %d:%d", &wdDay, &wdMon, &wdYear, &wdHour, &wdMin) != 5)
    {
        printf("A data inserida é inválida\n");
        return;
    }

    // Ajusta a data para tm
    struct tm withdrawalDate;
    memset(&withdrawalDate, 0, sizeof(withdrawalDate));
    withdrawalDate.tm_mday = wdDay;
    withdrawalDate.tm_mon = wdMon - 1;
    withdrawalDate.tm_year = wdYear - 1900;
    withdrawalDate.tm_hour = wdHour;
    withdrawalDate.tm_min = wdMin;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char buffer[100];
    const char *format;
    format = "%d/%m/%Y %H:%M";
    if (strftime(buffer, sizeof(buffer), format, &withdrawalDate) > sizeof(buffer))
        printf("there was a problem converting the string\n");
    else
        printf("%s\n", buffer);

    struct tm timetest;
    strptime(withdrawalDateStr, "%d/%m/%Y %H:%M", &timetest);
    if (strftime(buffer, sizeof(buffer), format, &timetest) > sizeof(buffer))
        printf("there was a problem converting the string\n");
    else
        printf("%s\n", buffer);

    time_t t1 = mktime(&withdrawalDate);
    time_t t2 = mktime(&timetest);

    // TODO: validar se cliente e veiculo existem
    printf("Por favor, informe o cod do cliente: ");
    scanf("%d", &l.clientCod);
    printf("Por favor, informe o cod do veiculo: ");
    scanf("%d", &l.vehicleCod);
    printf("Por favor, informe o valor do seguro: ");
    scanf("%lf", &l.insurance);

    FILE *fVehiclePtr = fopen(locationDbFile, "a+");
    fwrite(&l, sizeof(Location), 1, fVehiclePtr);
    if (fwrite == 0)
        printf("erro interno ao cadastrar veiculo.\n");
    else
        printf("Locação cadastrada. Cod: %d\n", l.cod);
    fclose(fVehiclePtr);
}

// void listLocations()
// {
//     FILE *fPtr = fopen(locationDbFile, "r");
//     if (fPtr == NULL) // Arquivo não existe
//     {
//         printf("* Nenhuma locação cadastrado.\n");
//         return;
//     }

//     printf("* COD - DESCRICAO : MODELO : COR : PLACA : VALOR DIARIA : QNT. OCUPANTES : STATUS\n");
//     Location l;
//     while (fread(&l, sizeof(Location), 1, fPtr))
//         printf("* %d - %s : %s : %s : %s : R$%.2f : %d : %s\n",
//                v.cod,
//                v.descricao,
//                v.modelo,
//                v.cor,
//                v.placa,
//                v.valorDiaria,
//                v.qntOcupantes,
//                v.status ? "Indisponivel" : "Disponivel");

//     fclose(fPtr);
// }

int getLastLocationId()
{
    int lastId = -1;
    FILE *fPtr = fopen(locationDbFile, "r");
    if (fPtr == NULL) // Arquivo não existe
        return lastId;

    // lê todos as locacoes
    Location l;
    while (fread(&l, sizeof(Location), 1, fPtr))
        lastId = l.cod;

    fclose(fPtr);
    return lastId;
}