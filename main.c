// Copyright 701x Inc - All Rights Reserved

#include "imu.h"
#include "csv.h"
#include "csv.c"
#include <stdio.h>
#include "imu.c"
#include <math.h>

int main()
{
    CSV_Column x = {.name = "acceleration_mgs_x", .dataType = CSV_DATA_TYPE_INT16};
    CSV_Column y = {.name = "acceleration_mgs_y", .dataType = CSV_DATA_TYPE_INT16};
    CSV_Column z = {.name = "acceleration_mgs_z", .dataType = CSV_DATA_TYPE_INT16};
    CSV_Column * columns[] = {&x, &y, &z};
    if(!CSV_read("acceleration.csv", columns, 3))
    {
        return -1;
    }

    for(uint32_t i = 0; i < x.length; i++)
    {
        IMU_processAccelerationData(x.dataI16[i], y.dataI16[i], z.dataI16[i]);
    }

    const uint32_t secondsResting = IMU_getSecondsSpentResting();
    printf("Animal spent %u minutes resting (%.2f%%)\n", secondsResting / 60, 100.0f * secondsResting / x.length);
}
