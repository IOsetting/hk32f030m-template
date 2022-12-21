#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "uart_printf.h"

#define UART_PRINTF_BAUDRATE 115200

const uint8_t HEX_TABLE[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void UART_PrintfInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

#if UART_PORTx == UART_PORT_TX_A3_RX_D6
  // Enable GPIO and UART clock
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOD, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  // AF config
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_1);
  // PA3 As USART TX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  // PD6 As USART RX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

#elif UART_PORTx == UART_PORT_TX_A3_RX_B4
  // Enable GPIO and UART clock
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  // AF config
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);   // TX
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_4);   // RX
  // PA3 As USART TX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  // PB4 As USART RX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

#endif
  USART_InitStructure.USART_BaudRate = UART_PRINTF_BAUDRATE;
  // 8-bit, 1 stop bit, no parity, no hardware flow control
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  // Enable Tx
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  // Init and enable UART1
  USART_Init(USART1, &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);
}

void UART_SendByte(uint8_t ch)
{
  /* Send one byte to USART */
  USART_SendData(USART1, ch);
  /* Block till tx empty flag is set */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void UART_SendArray(uint8_t *array, uint16_t num)
{
  uint8_t i;
  for (i = 0; i < num; i++)
  {
    UART_SendByte(array[i]);
  }
  /* Block till transmission is completed */
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
}

void UART_SendString(char *str)
{
  while (*str)
  {
    UART_SendByte(*str++);
  }
  /* Block till transmission is completed */
  while (USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
}

void UART_TxHex(uint8_t *hex, uint8_t len)
{
    while (len--)
    {
        UART_SendByte(HEX_TABLE[*(hex + len) >> 4 & 0xF]);
        UART_SendByte(HEX_TABLE[*(hex + len) & 0xF]);
    }
}

int __io_getchar(void)
{
  return USART_ReceiveData(USART1);
}

int __io_putchar(int ch)
{
  UART_SendByte((uint8_t)ch);
  return ch;
}

__attribute__((weak)) int _read(int file, char *ptr, int len)
{
    (void)file;
    int DataIdx;
    for (DataIdx = 0; DataIdx < len; DataIdx++)
    {
        *ptr++ = __io_getchar();
    }
    return len;
}

__attribute__((weak)) int _write(int file, char *ptr, int len)
{
    (void)file;
    int DataIdx;
    for (DataIdx = 0; DataIdx < len; DataIdx++)
    {
        __io_putchar(*ptr++);
    }
    return len;
}


__attribute__((weak)) int _isatty(int fd)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 1;

    errno = EBADF;
    return 0;
}

__attribute__((weak)) int _close(int fd)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 0;

    errno = EBADF;
    return -1;
}

__attribute__((weak)) int _lseek(int fd, int ptr, int dir)
{
    (void)fd;
    (void)ptr;
    (void)dir;

    errno = EBADF;
    return -1;
}

__attribute__((weak)) int _fstat(int fd, struct stat *st)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
    {
        st->st_mode = S_IFCHR;
        return 0;
    }

    errno = EBADF;
    return 0;
}