#ifndef HEADER_LOCATION_H
#define HEADER_LOCATION_H

#include <time.h>

typedef struct
{
    // void (*registerLocation)(int clientCod, int vehicleCod, struct tm withdrawalDate, struct tm returnDate);
    void (*registerLocation)(time_t withdrawalDate, time_t returnDate, int hasInsurance, int clientCod, int vehicleCod)
    // void (*listLocations)(void);
} LocationRepo;

extern const LocationRepo locationRepo;

#endif