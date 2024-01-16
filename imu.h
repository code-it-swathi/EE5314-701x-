// Copyright 701x Inc - All Rights Reserved

#ifndef IMU_H
#define IMU_H

#include <stdint.h>

/**
 * @brief Process a new 3-axis acceleration measurement from the IMU
 * @details Assume a sample rate of 1Hz
 *
 * @param x The acceleration in the X direction, in milli-Gs
 * @param y The acceleration in the Y direction, in milli-Gs
 * @param z The acceleration in the Z direction, in milli-Gs
 */
void IMU_processAccelerationData(int16_t x, int16_t y, int16_t z);

/**
 * @brief Calculate the total amount of time the animal has spent resting based
 * on the acceleration data provided thus far.
 * @return The estimated total number of seconds spent resting
 */
uint32_t IMU_getSecondsSpentResting(void);
uint32_t getCurrentTimeInSeconds(void);

#endif // IMU_H
