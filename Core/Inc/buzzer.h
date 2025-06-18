#ifndef BUZZER
#define BUZZER

#include "main.h"
#include "buzzer.h"

void buzzer_set_freq(uint32_t freq_hz);
void buzzer_start();
void buzzer_stop();

#endif