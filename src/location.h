#ifndef HEADER_LOCATION_H
#define HEADER_LOCATION_H

typedef struct
{
    void (*registerLocation)(int clientCod, int vehicleCod);
    // void (*listLocations)(void);
} LocationRepo;

extern const LocationRepo locationRepo;

#endif