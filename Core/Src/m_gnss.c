#include "m_gnss.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

static UART_HandleTypeDef *gps_huart;
static uint8_t gps_rx_buffer[GPS_BUFFER_SIZE];
static uint16_t gps_index = 0;
static GPS_Data current_gps_data = {0};

void GPS_Init(UART_HandleTypeDef *huart) {
    gps_huart = huart;
    HAL_UART_Receive_IT(gps_huart, &gps_rx_buffer[gps_index], 1);
}

void GPS_UART_Callback() {
    uint8_t rx_char = gps_rx_buffer[gps_index];
    
    // Проверка конца строки
    if (rx_char == '\n' || gps_index >= GPS_BUFFER_SIZE - 1) {
        gps_rx_buffer[gps_index] = '\0'; // Null-terminate
        
        // Парсим только GGA строки
        if (strstr((char*)gps_rx_buffer, "$GPGGA")) {
            char *token = strtok((char*)gps_rx_buffer, ",");
            uint8_t field = 0;
            
            while (token != NULL) {
                switch (field++) {
                    case 1:  // Время UTC
                        strncpy(current_gps_data.timestamp, token, 9);
                        break;
                    case 2:  // Широта
                        current_gps_data.latitude = atof(token) / 100;
                        break;
                    case 3:  // N/S
                        if (*token == 'S') current_gps_data.latitude *= -1;
                        break;
                    case 4:  // Долгота
                        current_gps_data.longitude = atof(token) / 100;
                        break;
                    case 5:  // E/W
                        if (*token == 'W') current_gps_data.longitude *= -1;
                        break;
                    case 6:  // Fix status
                        current_gps_data.fix_status = atoi(token);
                        break;
                    case 7:  // Спутники
                        current_gps_data.satellites = atoi(token);
                        break;
                    case 9:  // Высота
                        current_gps_data.altitude = atof(token);
                        break;
                }
                token = strtok(NULL, ",");
            }
        }
        gps_index = 0;
    } else {
        gps_index++;
    }
    HAL_UART_Receive_IT(gps_huart, &gps_rx_buffer[gps_index], 1);
}

GPS_Data* GPS_GetData() {
    return &current_gps_data;
}