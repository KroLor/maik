#include "m_gnss.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

GNSS_Data gnss_data;
uint8_t gnss_rx_buffer[GNSS_BUFFER_SIZE];
uint16_t gnss_index = 0;

void GNSS_Init(void) {
    gnss_index = 0;
    memset(&gnss_data, 0, sizeof(gnss_data));
    HAL_UART_Receive_IT(&huart2, &gnss_rx_buffer[gnss_index], 1);
}

void GNSS_UART_Callback(void) {
    if (gnss_index >= GNSS_BUFFER_SIZE - 1) gnss_index = 0;
    
    // Проверка конца строки
    if (gnss_rx_buffer[gnss_index] == '\n') {
        gnss_rx_buffer[gnss_index + 1] = '\0';
        GNSS_Process();
        gnss_index = 0;
    } else {
        gnss_index++;
    }
    
    HAL_UART_Receive_IT(&huart2, &gnss_rx_buffer[gnss_index], 1);
}

void GNSS_Process(void) {
    char* nmea = (char*)gnss_rx_buffer;
    
    // Проверка заголовка GGA
    if (strstr(nmea, "$GPGGA")) {
        char* tokens[15];
        uint8_t token_count = 0;
        
        // Разбивка на токены
        char* token = strtok(nmea, ",");
        while (token && token_count < 15) {
            tokens[token_count++] = token;
            token = strtok(NULL, ",");
        }

        if (token_count < 10) return;

        // Парсинг времени
        if (strlen(tokens[1]) >= 6) {
            char time_buf[3] = {0};
            memcpy(time_buf, tokens[1], 2);
            gnss_data.hour = atoi(time_buf);
            memcpy(time_buf, tokens[1] + 2, 2);
            gnss_data.minute = atoi(time_buf);
            memcpy(time_buf, tokens[1] + 4, 2);
            gnss_data.second = atoi(time_buf);
        }

        // Парсинг координат
        if (strlen(tokens[2]) > 0 && strlen(tokens[4]) > 0) {
            gnss_data.latitude = atof(tokens[2]);
            gnss_data.longitude = atof(tokens[4]);
            
            // Конвертация формата DDMM.MMMM -> DD.DDDD
            gnss_data.latitude = (int)(gnss_data.latitude / 100) + 
                                fmod(gnss_data.latitude, 100) / 60;
            gnss_data.longitude = (int)(gnss_data.longitude / 100) + 
                                 fmod(gnss_data.longitude, 100) / 60;
            
            // Коррекция полушарий
            if (tokens[3][0] == 'S') gnss_data.latitude *= -1;
            if (tokens[5][0] == 'W') gnss_data.longitude *= -1;
        }

        // Количество спутников
        gnss_data.satellites = atoi(tokens[7]);
        
        // Высота
        if (strlen(tokens[9]) > 0) {
            gnss_data.altitude = atof(tokens[9]);
        }
        
        #if GNSS_ENABLE_INFO
        printf("GPS: %02d:%02d:%02d | Lat: %.6f | Lon: %.6f | Sats: %d | Alt: %.1fm\n",
               gnss_data.hour, gnss_data.minute, gnss_data.second,
               gnss_data.latitude, gnss_data.longitude,
               gnss_data.satellites, gnss_data.altitude);
        #endif
    }
}