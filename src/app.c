/* app.c
 *
 * Este arquivo representa a interface com o usuário via linha de comando (CLI).
 *
 *********************************************************************/

#include "app.h"
#include "service.h"

// Tipos

// Prototipos privados

// Variaveis globais

// Implementações

void appRegisterClient(void)
{
    service.registerClient();
}

void appRegisterVehicle(void)
{
    service.registerVehicle();
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
