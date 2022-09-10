/**
  ******************************************************************************
  * @file    main.c
  * @version V1.0
  * @brief   UART1 TX And RX Example
  ******************************************************************************
  */
#include "hk32f030m.h" 
#include "systick_delay.h"

#define  DEBUG_USART_BAUDRATE           115200


void USART1_IRQHandler(void)
{
  uint8_t ucTemp;
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    ucTemp = USART_ReceiveData(USART1);
    USART_SendData(USART1, ucTemp);
  }
}

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  // value between 0 and 3
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief TX:PA3, RX:PD6
 * 
 */
void USART_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// 8-bit
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 1 stop bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// No parity
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// No hardware flow control
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// Enable Tx and Rx
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	// UART Interrupt
	NVIC_Configuration();
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
	// Enable UART1
	USART_Cmd(USART1, ENABLE);	    
}

void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch)
{
  /* Send one byte to USART */
  USART_SendData(pUSARTx, ch);
  /* Block till tx empty flag is set */
  while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	for(i=0; i<num; i++)
  {
	    Usart_SendByte(pUSARTx,array[i]);	
  }
	/* Block till transmission is completed */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
  while (*str)
  {
    Usart_SendByte( pUSARTx, *str++);
  }
  /* Block till transmission is completed */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
  USART_SendData(pUSARTx, ch >> 8);
  while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
  USART_SendData(pUSARTx, ch & 0xFF);
  while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

int main(void)
{
  SysTick_Init();
  USART_Configuration();

  while (1)
  {
    Usart_SendString(USART1, "Hello World!\r\n");
    SysTick_DelayMs(1000);
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char* file , uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */	
       /* Infinite loop */
	
	while (1)
  {		
  }
}
#endif /* USE_FULL_ASSERT */


