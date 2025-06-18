#pragma once
#include "main.h"

#define GNSS_ENABLE_INFO true
#define GNSS_BUFFER_SIZE 256

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    float latitude;
    float longitude;
    uint8_t satellites;
    float altitude;
} GNSS_Data;

extern GNSS_Data gnss_data;
extern UART_HandleTypeDef huart2;

void GNSS_Init(void);
void GNSS_Process(void);
void GNSS_UART_Callback(void);