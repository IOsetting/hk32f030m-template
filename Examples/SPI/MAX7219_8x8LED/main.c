/**
  ******************************************************************************
  * @file    main.c
  * @brief   MAX7219 8x8LED SPI Example
  ******************************************************************************
  */ 
#include "hk32f030m.h"
#include "systick_delay.h"

#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

#define DECODE_MODE  0x09
#define INTENSITY    0x0A
#define SCAN_LIMIT   0x0B
#define SHUT_DOWN    0x0C
#define DISPLAY_TEST 0x0F

const uint8_t numbers[]={
0x00,0x00,0x7C,0xC6,0xC6,0xCE,0xD6,0xD6,  // -0-.  
0xE6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00,
0x00,0x00,0x18,0x38,0x78,0x18,0x18,0x18,  // -1-  
0x18,0x18,0x18,0x7E,0x00,0x00,0x00,0x00,  
0x00,0x00,0x7C,0xC6,0x06,0x0C,0x18,0x30,  // -2-  
0x60,0xC0,0xC6,0xFE,0x00,0x00,0x00,0x00,  
0x00,0x00,0x7C,0xC6,0x06,0x06,0x3C,0x06,  // -3-  
0x06,0x06,0xC6,0x7C,0x00,0x00,0x00,0x00,  
0x00,0x00,0x0C,0x1C,0x3C,0x6C,0xCC,0xFE,  // -4-  
0x0C,0x0C,0x0C,0x1E,0x00,0x00,0x00,0x00,  
0x00,0x00,0xFE,0xC0,0xC0,0xC0,0xFC,0x0E,  // -5-  
0x06,0x06,0xC6,0x7C,0x00,0x00,0x00,0x00,  
0x00,0x00,0x38,0x60,0xC0,0xC0,0xFC,0xC6,  // -6-  
0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00,  
0x00,0x00,0xFE,0xC6,0x06,0x06,0x0C,0x18,  // -7-  
0x30,0x30,0x30,0x30,0x00,0x00,0x00,0x00,  
0x00,0x00,0x7C,0xC6,0xC6,0xC6,0x7C,0xC6,  // -8-  
0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00, 
0x00,0x00,0x7C,0xC6,0xC6,0xC6,0x7E,0x06,  // -9-  
0x06,0x06,0x0C,0x78,0x00,0x00,0x00,0x00};

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;

static uint16_t Timeout_Callback(uint8_t errorCode)
{
  printf("Timeout errorCode = %d", errorCode);
  return 0;
}

static void SPIx_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOD, ENABLE);
  // PD3 SCK
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_2);
  // PD4 MOSI
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_2);
  // PB4 MISO
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_2);

  // PD7 CS (TSSOP20/QFN20 VCAP pin is PD7)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Schmit = GPIO_Schmit_Disable;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  // PD3 SCK
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  // PD4 MOSI
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  // PB4 MISO
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void SPIx_Config(void)
{
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE );
  // Stop SPI
  GPIO_SetBits(GPIOD, GPIO_Pin_7);

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial =7;
  SPI_Init(SPI1, &SPI_InitStructure);
  /* Initialize the FIFO threshold */
	SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);
  SPI_Cmd(SPI1, ENABLE);
}

uint8_t SPI_TxRxByte(uint8_t byte)
{
  uint32_t spixbase = 0x00;
	SPITimeout = SPIT_FLAG_TIMEOUT;
  /* Check the status of Transmit buffer Empty flag */
  while ((SPI1->SR & SPI_I2S_FLAG_TXE) == (uint16_t)RESET)
  {
    if((SPITimeout--) == 0) return Timeout_Callback(0);
  }
  spixbase = (uint32_t)SPI1;
  spixbase += 0x0C;
  *(__IO uint8_t *) spixbase = byte;

	SPITimeout = SPIT_FLAG_TIMEOUT;
  while ((SPI1->SR & SPI_I2S_FLAG_TXE) == (uint16_t)RESET)
  {
    if((SPITimeout--) == 0) return Timeout_Callback(0);
  }
  // Read from RX buffer
  return *(__IO uint8_t *) spixbase;
}

void MAX7219_write(uint8_t addr, uint8_t dat)
{
  GPIO_ResetBits(GPIOD, GPIO_Pin_7);
  SPI_TxRxByte(addr);
  SPI_TxRxByte(dat);
  GPIO_SetBits(GPIOD, GPIO_Pin_7);
}

void MAX7219_init(void)
{
  MAX7219_write(SHUT_DOWN, 0x01);    // 0x00:shutdown, 0x01:normal
  MAX7219_write(DECODE_MODE, 0x00);  // Bypass code B decoder, no-decode operation
  MAX7219_write(SCAN_LIMIT, 0x07);   // Scan-limit, 0:1-digit, 1:2-digits, ... 7:8-digits
  MAX7219_write(INTENSITY, 0x01);    // 0x00:min, 0xFF:max
  MAX7219_write(DISPLAY_TEST, 0x00); // 0x00:normal, 0x01:test mode
}

int main(void)
{
  uint8_t pos = 0, size = sizeof(numbers), i, j;

  SysTick_Init();

  SPIx_GPIO_Config();
  SPIx_Config();

  MAX7219_init();

  while (1)
  {
    for (i = 0; i < 8; i++)
    {
      j = (pos + i) % size;
      MAX7219_write(i + 1, numbers[j]);
    }
    pos = (pos + 1) % size;
    SysTick_DelayMs(200);
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
