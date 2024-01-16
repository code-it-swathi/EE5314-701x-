// Copyright 701x Inc - All Rights Reserved

#include "csv.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define DELIMITERS ", \r\n"

bool CSV_read(const char * path, CSV_Column ** columns, uint32_t numberOfColumns)
{
    FILE * file = fopen(path, "r");
    if(!file)
    {
        printf("[CSV] Failed to open %s\n", path);
        return false;
    }

    for(uint32_t i = 0; i < numberOfColumns; i++)
    {
        columns[i]->index = -1;
    }

    // Parse column names and assign indices
    char line[512] = {0};
    fseek(file, 0, SEEK_SET);
    if(!fgets(line, sizeof(line), file))
    {
        return false;
    }
    uint32_t columnsFound = 0;
    uint32_t nameLength = 0;
    for(char * c = line; c <= line + strlen(line); c++, nameLength++)
    {
        if((*c == ',') || (*c == '\n'))
        {
            for(uint32_t i = 0; i < numberOfColumns; i++)
            {
                if(memcmp(c - nameLength, columns[i]->name, nameLength) == 0)
                {
                    columns[i]->index = columnsFound;
                    break;
                }
            }
            nameLength = 0;
            columnsFound++;
            c++;
        }
    }

    bool allColumnsFound = true;
    for(uint32_t i = 0; i < numberOfColumns; i++)
    {
        if(columns[i]->index < 0)
        {
            allColumnsFound = false;
            printf("[CSV] failed to find column '%s' in %s\n", columns[i]->name, path);
        }
    }
    if(!allColumnsFound)
    {
        return false;
    }

    // Count rows and allocate memory
    uint32_t numberOfRows = 0;
    while(fgets(line, sizeof(line), file))
    {
        ++numberOfRows;
    }
    if(numberOfRows == 0)
    {
        printf("[CSV] %s has no data rows\n", path);
        return false;
    }
    for(uint32_t i = 0; i < numberOfColumns; i++)
    {
        columns[i]->length = numberOfRows;
        switch(columns[i]->dataType)
        {
        case CSV_DATA_TYPE_BOOL:
            columns[i]->dataBool = calloc(columns[i]->length, sizeof(bool));
            break;
        case CSV_DATA_TYPE_INT8:
            columns[i]->dataI8 = calloc(columns[i]->length, sizeof(int8_t));
            break;
        case CSV_DATA_TYPE_INT16:
            columns[i]->dataI16 = calloc(columns[i]->length, sizeof(int16_t));
            break;
        case CSV_DATA_TYPE_INT32:
            columns[i]->dataI32 = calloc(columns[i]->length, sizeof(int32_t));
            break;
        case CSV_DATA_TYPE_UINT8:
            columns[i]->dataU8 = calloc(columns[i]->length, sizeof(uint8_t));
            break;
        case CSV_DATA_TYPE_UINT16:
            columns[i]->dataU16 = calloc(columns[i]->length, sizeof(uint16_t));
            break;
        case CSV_DATA_TYPE_UINT32:
            columns[i]->dataU32 = calloc(columns[i]->length, sizeof(uint32_t));
            break;
        case CSV_DATA_TYPE_FLOAT:
            columns[i]->dataF32 = calloc(columns[i]->length, sizeof(float));
            break;
        case CSV_DATA_TYPE_DOUBLE:
            columns[i]->dataF64 = calloc(columns[i]->length, sizeof(double));
            break;
        case CSV_DATA_TYPE_STRING:
            columns[i]->dataStr = calloc(columns[i]->length, sizeof(char *));
            break;
        default:
            printf("[CSV] unsupported data type %u\n", columns[i]->dataType);
            return false;
        }
    }

    // Read data
    (void)fseek(file, 0, SEEK_SET);
    (void)fgets(line, sizeof(line), file);
    for(uint32_t r = 0; r < numberOfRows; r++)
    {
        if(!fgets(line, sizeof(line), file))
        {
            printf("[CSV] failed to read row %u from %s\n", r, path);
            return false;
        }
        char * start = line;
        for(uint32_t c = 0; c < columnsFound; c++)
        {
            char * end = strpbrk(start, DELIMITERS);
            if(!end)
            {
                printf("[CSV] failed to parse row %u column %u from %s\n", r, c, path);
                return false;
            }
            *end = 0;

            for(uint32_t i = 0; i < numberOfColumns; i++)
            {
                if((uint32_t)columns[i]->index == c)
                {
                    switch(columns[i]->dataType)
                    {
                    case CSV_DATA_TYPE_BOOL:
                        columns[i]->dataBool[r] = start[0] == '1' || start[0] == 'T' || start[0] == 't';
                        break;
                    case CSV_DATA_TYPE_INT8:
                        columns[i]->dataI8[r] = round(strtof(start, NULL));
                        break;
                    case CSV_DATA_TYPE_INT16:
                        columns[i]->dataI16[r] = round(strtof(start, NULL));
                        break;
                    case CSV_DATA_TYPE_INT32:
                        columns[i]->dataI32[r] = round(strtof(start, NULL));
                        break;
                    case CSV_DATA_TYPE_UINT8:
                        columns[i]->dataU8[r] = round(strtod(start, NULL));
                        break;
                    case CSV_DATA_TYPE_UINT16:
                        columns[i]->dataU16[r] = round(strtod(start, NULL));
                        break;
                    case CSV_DATA_TYPE_UINT32:
                        columns[i]->dataU32[r] = round(strtod(start, NULL));
                        break;
                    case CSV_DATA_TYPE_FLOAT:
                        columns[i]->dataF32[r] = strtof(start, NULL);
                        break;
                    case CSV_DATA_TYPE_DOUBLE:
                        columns[i]->dataF64[r] = strtod(start, NULL);
                        break;
                    case CSV_DATA_TYPE_STRING:
                    {
                        columns[i]->dataStr[r] = calloc(end - start + 1, sizeof(char));
                        strcpy(columns[i]->dataStr[r], start);
                        break;
                    }
                    default:
                        printf("[CSV] unsupported data type %u\n", columns[i]->dataType);
                        return false;
                    }
                    break;
                }
            }

            start = end + 1;
        }
    }
    printf("[CSV] read '%s': %u x %u\n", path, numberOfColumns, numberOfRows);
    return true;
}
