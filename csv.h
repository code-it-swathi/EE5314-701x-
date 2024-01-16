// Copyright 701x Inc - All Rights Reserved

#ifndef CSV_H
#define CSV_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum CSV_DataType
{
    CSV_DATA_TYPE_BOOL,
    CSV_DATA_TYPE_FLOAT,
    CSV_DATA_TYPE_DOUBLE,
    CSV_DATA_TYPE_STRING,
    CSV_DATA_TYPE_INT8,
    CSV_DATA_TYPE_INT16,
    CSV_DATA_TYPE_INT32,
    CSV_DATA_TYPE_UINT8,
    CSV_DATA_TYPE_UINT16,
    CSV_DATA_TYPE_UINT32,
} CSV_DataType;

typedef struct CSV_Column
{
    const char * name;
    CSV_DataType dataType;
    int32_t index;
    uint32_t length;
    union
    {
        bool * dataBool;
        char ** dataStr;
        int8_t * dataI8;
        int16_t * dataI16;
        int32_t * dataI32;
        uint8_t * dataU8;
        uint16_t * dataU16;
        uint32_t * dataU32;
        float * dataF32;
        double * dataF64;
    };
} CSV_Column;

/**
 * @brief Read data from a CSV file
 * @details The CSV must contain a header row
 *
 * @param path Path to the CSV file
 * @param columns A list of columns that should be read from the CSV
 * @param numberOfColumns The number of columns in the aforementioned list
 * @return True if the CSV was read successfully. False if the file could not be
 * parsed as a CSV.
 */
bool CSV_read(const char * path, CSV_Column ** columns, uint32_t numberOfColumns);

#endif // CSV_H
