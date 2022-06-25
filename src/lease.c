/* lease.c
 *
 * Este arquivo representa a implementação de funções para manipulação
 * de locações no sistema.
 *
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lease.h"
#include "utils.h"

static int get_last_lease_id(void);

char *lease_db_file = "_db_lease.dat";

static int get_last_lease_id(void)
{
    int lastId = -1;
    FILE *f_ptr = fopen(lease_db_file, "r");
    if (f_ptr == NULL) // Arquivo não existe
        return lastId;

    // lê todos as locacoes
    Lease l;
    while (fread(&l, sizeof(Lease), 1, f_ptr))
        lastId = l.cod;

    fclose(f_ptr);
    return lastId;
}

int lease_register(time_t withdrawal_date, time_t return_date, int has_insurance, int client_cod, int vehicle_cod)
{
    Lease l = {
        .cod = get_last_lease_id() + 1,
        .clientCod = client_cod,
        .vehicleCod = vehicle_cod,
        .hasInsurance = has_insurance,
        .withdrawalDate = withdrawal_date,
        .returnDate = return_date,
        .finished = 0,
    };

    FILE *f_ptr = fopen(lease_db_file, "a+");
    if (fwrite(&l, sizeof(Lease), 1, f_ptr) != 1)
    {
        printf("erro interno ao cadastrar locação.\n");
        fclose(f_ptr);
        return EXIT_FAILURE;
    }

    printf("Locação cadastrada. Cod: %d\n", l.cod);
    fclose(f_ptr);
}

void lease_list(void)
{
    FILE *f_ptr = fopen(lease_db_file, "r");
    if (f_ptr == NULL) // Arquivo não existe
    {
        printf("* Nenhuma locação cadastrado.\n");
        return;
    }

    printf("* COD - (STATUS) - CLIENTE : VEICULO : INICIO -> FIM : SEGURO\n");
    Lease l;
    while (fread(&l, sizeof(Lease), 1, f_ptr))
    {
        struct tm withdrawalDate = *localtime(&l.withdrawalDate);
        struct tm returnDate = *localtime(&l.returnDate);
        // struct tm tm2 = *gmtime(&t);

        char bufFmtWithdrawalDate[20];
        char bufFmtReturnDate[20];
        utils_format_date(&withdrawalDate, bufFmtWithdrawalDate, sizeof(bufFmtWithdrawalDate));
        utils_format_date(&returnDate, bufFmtReturnDate, sizeof(bufFmtReturnDate));

        printf("* %d - %s - %d : %d : %s -> %s : %s\n",
               l.cod,
               l.finished ? "(FINALIZADA)" : "()",
               l.clientCod,
               l.vehicleCod,
               bufFmtWithdrawalDate,
               bufFmtReturnDate,
               l.hasInsurance ? "Possui Seguro" : "Não Possui Seguro");
    }

    fclose(f_ptr);
}

int lease_get_by_cod(int cod, Lease *lease)
{
    FILE *f_ptr = fopen(lease_db_file, "r");
    if (f_ptr == NULL) // Arquivo não existe, logo locação não existe
        return 0;

    // https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
    fseek(f_ptr, 0L, SEEK_END);
    long size = ftell(f_ptr);
    rewind(f_ptr);

    // como o id do cliente é incremental
    long offset = sizeof(Lease) * cod;
    if (offset >= size) // se o offset é maior que o tamanho do arquivo, ou seja a locação não existe
        return 0;

    fseek(f_ptr, offset, SEEK_SET);
    int exits = fread(lease, sizeof(Lease), 1, f_ptr) == 1;
    fclose(f_ptr);
    return exits;
}

int lease_finalize(int cod)
{
    FILE *f_ptr = fopen(lease_db_file, "r+");
    if (f_ptr == NULL) // Arquivo não existe, logo locacao não existe
        return 0;

    // https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
    fseek(f_ptr, 0L, SEEK_END);
    long size = ftell(f_ptr);
    rewind(f_ptr);

    // como o id do cliente é incremental
    long offset = sizeof(Lease) * cod;
    if (offset >= size) // se o offset é maior que o tamanho do arquivo, ou seja o veiculo não existe
        return 0;

    Lease l;
    fseek(f_ptr, offset, SEEK_SET);
    int exits = fread(&l, sizeof(Lease), 1, f_ptr) == 1;
    if (!exits)
    {
        printf("erro interno ao ler locacao.\n");
        return 0;
    }

    l.finished = 1;

    fseek(f_ptr, offset, SEEK_SET);
    if (fwrite(&l, sizeof(Lease), 1, f_ptr) == 0)
    {
        printf("erro interno ao atualizar locação.\n");
        return 0;
    }
    else
    {
        printf("Locação atualizada. Cod: %d\n", l.cod);
    }

    fclose(f_ptr);
    return 1;
}
