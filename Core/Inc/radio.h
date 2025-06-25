#ifndef RADIO_H
#define RADIO_H

#include "main.h"
#include "stdbool.h"

bool E220_CheckStatus();
void E220_Init(void);
void E220_SendCommand(const char *cmd);
uint8_t E220_ReceiveHandler(void);

#endif