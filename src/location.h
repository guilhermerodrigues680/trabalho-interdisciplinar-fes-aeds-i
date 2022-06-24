#ifndef HEADER_LOCATION_H
#define HEADER_LOCATION_H

#include "models.h"

typedef struct
{
    int (*registerLocation)(time_t withdrawalDate, time_t returnDate, int hasInsurance, int clientCod, int vehicleCod);
    void (*listLocations)(void);
    int (*getLocation)(int cod, Location *location);
    int (*finalizeLease)(int cod);
} LocationRepo;

extern char *locationDbFile;
extern const LocationRepo locationRepo;

#endif