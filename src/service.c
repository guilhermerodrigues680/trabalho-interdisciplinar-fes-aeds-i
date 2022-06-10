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