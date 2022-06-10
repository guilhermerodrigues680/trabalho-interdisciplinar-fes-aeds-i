#include <stdio.h>
#include "service.h"
#include "client.h"
#include "location.h"
#include "vehicle.h"

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

    // locationRepo.registerLocation(clientCod, vehicleCod);
}

void serviceListClients()
{
    clientsRepo.listClients();
}

void serviceListVehicles()
{
    vehicleRepo.listVehicles();
}

const Service service = {
    .registerClient = &serviceRegisterClient,
    .registerVehicle = &serviceRegisterVehicle,
    .registerLocation = &serviceRegisterLocation,
    .listClients = &serviceListClients,
    .listVehicles = &serviceListVehicles};