#ifndef HEADER_VEHICLE_H
#define HEADER_VEHICLE_H

typedef struct
{
    void (*registerVehicle)(void);
    void (*listVehicles)(void);
} VehicleRepo;

extern const VehicleRepo vehicleRepo;

#endif