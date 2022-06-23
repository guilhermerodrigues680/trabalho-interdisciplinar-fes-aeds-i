#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "service_test.h"
#include "client.h"
#include "location.h"
#include "vehicle.h"
#include "utils.h"

/* Executa os testes no programa e finaliza a execução. */
void serviceRunTests()
{
    printf("Preparando ambiente de testes...\n");
    clientsDbFile = "clients_db_test.dat";
    locationDbFile = "leases_db_test.dat";
    vehicleDbFile = "vehicles_db_test.dat";

    printf("Executando testes...\n");
    time_t epochWithdrawalDate = 1641034800L; // 1 de janeiro de 2022 às 08:00:00 GMT-03:00
    time_t epochReturnDate = 1641294000L;     // 4 de janeiro de 2022 às 08:00:00 GMT-03:00
    locationRepo.registerLocation(
        epochWithdrawalDate,
        epochReturnDate,
        1,
        0,
        0);

    printf("Testes finalizados.\n");
    exit(0);
}
