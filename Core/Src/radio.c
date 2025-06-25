#include "main.h"
#include "stdbool.h"

bool E220_CheckStatus() {
    // Переход в конфигурационный режим
    HAL_GPIO_WritePin(M0_GPIO_Port, M0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M1_GPIO_Port, M1_Pin, GPIO_PIN_RESET);
    HAL_Delay(200);
    
    // Отправка тестовой команды
    E220_SendCommand("AT\r\n");
    
    // Ожидание ответа
    uint8_t rx_buf[16] = {0};
    HAL_UART_Receive(&huart2, rx_buf, sizeof(rx_buf), 500);
    
    // Возврат в нормальный режим
    HAL_GPIO_WritePin(M0_GPIO_Port, M0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_GPIO_Port, M1_Pin, GPIO_PIN_RESET);
    HAL_Delay(50);
    
    // Проверка ответа
    return (strstr((char*)rx_buf, "OK") != NULL);
}

// Инициализация E220
void E220_Init() {
  // Переход в Normal Mode (M0=0, M1=0)
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);  // M1
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);  // M0
  HAL_Delay(100);
  
  // Настройка модуля
  E220_SendCommand("AT+ADDRESS=1\r\n");
  HAL_Delay(50);
  E220_SendCommand("AT+NETWORKID=5\r\n");
  HAL_Delay(50);
}

// Отправка команд
void E220_SendCommand(const char *cmd) {
  HAL_UART_Transmit(&huart2, (uint8_t*)cmd, strlen(cmd), HAL_MAX_DELAY);
}

// Приём данных
uint8_t E220_ReceiveHandler() {
  uint8_t rx_buffer[128];
  if (HAL_UART_Receive(&huart2, rx_buffer, sizeof(rx_buffer), 100) == HAL_OK) {
    // Обработка данных
    rx_buffer[sizeof(rx_buffer)-1] = '\0';
    return rx_buffer;
  }
}