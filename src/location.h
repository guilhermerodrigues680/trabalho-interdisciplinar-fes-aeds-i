#ifndef HEADER_LOCATION_H
#define HEADER_LOCATION_H

typedef struct
{
    void (*registerLocation)(void);
    // void (*listLocations)(void);
} LocationRepo;

extern const LocationRepo locationRepo;

#endif