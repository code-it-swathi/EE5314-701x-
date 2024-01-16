#include "imu.h"
#include <time.h>
#include <math.h>

// Define necessary variables for IMU variables
static int16_t last_x = 0;
static int16_t last_y = 0;
static int16_t last_z = 0;

static uint32_t resting_start_time = 0;
static uint32_t total_resting_time = 0;

void IMU_processAccelerationData(int16_t x, int16_t y, int16_t z) {
    // Assuming sample rate of 1Hz
    //Checking if the magnitude is below a threshold
    int16_t magnitude = (int16_t)sqrt((double)(x * x + y * y + z * z));

    // Assuming a threshold of 30 for resting
    if (magnitude < 30) {
        // If not already resting, start resting period
        if (resting_start_time == 0) {
            resting_start_time = getCurrentTimeInSeconds();
        }
    } else {
        // If resting, calculate duration and reset start time
        if (resting_start_time != 0) {
            total_resting_time += getCurrentTimeInSeconds() - resting_start_time;
            resting_start_time = 0;
        }
    }

    // Save current values for next iteration
    last_x = x;
    last_y = y;
    last_z = z;
}

uint32_t IMU_getSecondsSpentResting(void) {
    return total_resting_time;
}

//Function to get the current time
uint32_t getCurrentTimeInSeconds(void) {
    time_t currentTime = time(NULL);
    // Check if time retrieval was successful
    if (currentTime != (time_t)-1) {
        return (uint32_t)currentTime;
    } else {
        // Return 0 for error encountered
        return 0;
    }
}
