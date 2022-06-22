#ifndef HEADER_LOCATION_H
#define HEADER_LOCATION_H

#include <time.h>

typedef struct
{
    int cod;
    time_t withdrawalDate;
    time_t returnDate;
    int hasInsurance;
    int clientCod;
    int vehicleCod;
} Location;

typedef struct
{
    void (*registerLocation)(time_t withdrawalDate, time_t returnDate, int hasInsurance, int clientCod, int vehicleCod);
    void (*listLocations)(void);
    int (*getLocation)(int cod, Location *location);
} LocationRepo;

extern const LocationRepo locationRepo;

#endif