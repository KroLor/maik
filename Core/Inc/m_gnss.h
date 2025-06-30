#pragma once
#include "main.h"
#include <stdint.h>
#include "stm32f4xx_hal.h"

float nmea_to_decimal(float nmea_coord, char direction);
void parse_nmea(char* buffer);