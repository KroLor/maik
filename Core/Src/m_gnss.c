#include "m_gnss.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Глобальные переменные для хранения GPS-данных
volatile float latitude = 0.0;
volatile float longitude = 0.0;
volatile uint8_t fix_status = 0;
volatile uint8_t new_data_available = 0;

// Функция преобразования NMEA-формата в десятичные градусы
float nmea_to_decimal(float nmea_coord, char direction) {
    int degrees = (int)(nmea_coord / 100);
    float minutes = nmea_coord - (degrees * 100.0);
    float decimal = degrees + (minutes / 60.0);
    return (direction == 'S' || direction == 'W') ? -decimal : decimal;
}

// Функция парсинга NMEA-строк
void parse_nmea(char* buffer) {
    if (strstr(buffer, "$GPGGA")) {
        char* tokens[15];
        uint8_t i = 0;
        
        // Разбиваем строку на токены
        char* token = strtok(buffer, ",");
        while (token != NULL && i < 15) {
            tokens[i++] = token;
            token = strtok(NULL, ",");
        }

        // Обрабатываем только валидные строки
        if (i > 9) {
            fix_status = atoi(tokens[6]);
            
            if (fix_status >= 1) {
                float lat = atof(tokens[2]);
                float lon = atof(tokens[4]);
                
                // Обновляем глобальные переменные с координатами
                latitude = nmea_to_decimal(lat, tokens[3][0]);
                longitude = nmea_to_decimal(lon, tokens[5][0]);
                new_data_available = 1;
            }
        }
    }
}