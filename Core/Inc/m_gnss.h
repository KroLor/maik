#pragma once
#include "main.h"
#include <stdint.h>
#include "stm32f4xx_hal.h"

#define GPS_BUFFER_SIZE 128

typedef struct {
    float latitude;
    float longitude;
    float altitude;
    uint8_t fix_status;
    uint8_t satellites;
    char timestamp[10];
} GPS_Data;

void GPS_Init(UART_HandleTypeDef *huart);
void GPS_UART_Callback();
GPS_Data* GPS_GetData();    