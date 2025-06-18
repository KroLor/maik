#include "tim.h"
#include "buzzer.h"

void buzzer_set_freq(uint32_t freq_hz) {
    uint32_t clock = HAL_RCC_GetPCLK1Freq();
    uint32_t prescaler = htim2.Instance->PSC;
    uint32_t period = (clock / ((prescaler + 1) * freq_hz)) - 1;

    __HAL_TIM_SET_PRESCALER(&htim2, prescaler);
    __HAL_TIM_SET_AUTORELOAD(&htim2, period);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, period / 2);
}

void buzzer_start() {
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
}

void buzzer_stop() {
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
}