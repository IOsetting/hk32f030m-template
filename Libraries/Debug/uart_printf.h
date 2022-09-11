#ifndef __UART_PRINTF_H
#define __UART_PRINTF_H

#include "hk32f030m.h"
#include "stdio.h"

#define UART_PORT_TX_A3_RX_D6       0
#define UART_PORT_TX_A3_RX_B4       1
#define UART_PORTx                  UART_PORT_TX_A3_RX_D6

void UART_PrintfInit(void);
void UART_SendByte(uint8_t ch);
void UART_SendHalfWord(uint16_t ch);
void UART_SendArray(uint8_t *array, uint16_t num);
void UART_SendString(char *str);
void UART_TxHex(uint8_t *hex, uint8_t len);

#endif
