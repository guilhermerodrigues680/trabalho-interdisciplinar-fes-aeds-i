#ifndef HEADER_LOCATION_H
#define HEADER_LOCATION_H

#include <time.h>

typedef struct
{
    int cod;
    int clientCod;
    int vehicleCod;
    time_t withdrawalDate;
    time_t returnDate;
    unsigned char hasInsurance;
    unsigned char finished;
    // TODO: Armazenar dados da locação para relatório
} Location;

typedef struct
{
    void (*registerLocation)(time_t withdrawalDate, time_t returnDate, int hasInsurance, int clientCod, int vehicleCod);
    void (*listLocations)(void);
    int (*getLocation)(int cod, Location *location);
    int (*finalizeLease)(int cod);
} LocationRepo;

extern char *locationDbFile;
extern const LocationRepo locationRepo;

#endif