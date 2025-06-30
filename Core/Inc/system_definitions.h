#include <stm32f411xe.h>
#include <usart.h>

#ifndef __SYSTEM_DEFINITIONS_H
#define __SYSTEM_DEFINITIONS_H

typedef enum
{
	SYS_STATE_NONE = 0,
	SYS_STATE_INIT,
	SYS_STATE_STANDBY,
	SYS_STATE_LIFTOFF,
	SYS_STATE_ASCENT,
	SYS_STATE_APOGY,
	SYS_STATE_DESCENT,
	SYS_STATE_GROUND
} SystemState;

//______________________________RADIO
#define RADIO_M0_PORT GPIOB
#define RADIO_M0_PIN GPIO_PIN_12
#define RADIO_M1_PORT GPIOB
#define RADIO_M1_PIN GPIO_PIN_13

#define radio_aux_GPIO_Port GPIOB
#define radio_aux_Pin GPIO_PIN_9

#define RADIO_UART_HANDLE huart1

//_____________________________LED
#define led_radio_GPIO_Port GPIOB
#define led_radio_Pin GPIO_PIN_15

//______________________________USB UART

// #define USB_UART_HANDLE huart1

#endif