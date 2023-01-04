/**
  ******************************************************************************
  * @brief   I2S Audio Example
  * 
  *   This will play a short voice clip through speaker
  * 
  *   HK32F030MF4P6               MAX98357A
  *   PD3(SPI1_SCK/I2S_CK)        -> BCLK
  *   PD4(SPI1_MOSI/I2S_SD)       -> DIN
  *   PA3(SPI1_NSS/I2S_WS)        -> LRC
  *                               GND  -> GND
  *                               VIN  -> 3.3V
  *                               +    -> speaker
  *                               -    -> speaker
  * 
  ******************************************************************************
  */ 
#include "hk32f030m.h"
#include "systick_delay.h"
#include "voice.h"

__IO uint16_t TxIdx = 0;
__IO uint8_t lr = 0;

static void I2Sx_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOD, ENABLE);

  // PD3 BCK
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Schmit = GPIO_Schmit_Disable;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  // PD4 SD
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  // PB4 MCK
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  // PA3 WS,LRCK
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // PinSourcex: specifies the pin of port for the AF
  // PD3 BCK
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_2);
  // PD4 SD
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_2);
  // PB4 MCK
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_2);
  // PA3 WS,LRCK
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_2);
}

void I2Sx_Config(void)
{
  I2S_InitTypeDef I2S_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE );

  // I2S peripheral configuration
  I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
  // 8K sample rate
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_8k;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  // Master mode
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
  I2S_Init(SPI1, &I2S_InitStructure);

  // SPI1 IRQ Channel configuration
  NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void SPI1_IRQHandler(void)
{
  // If TX Empty flag is set
  if ((SPI1->SR & SPI_I2S_FLAG_TXE) == SPI_I2S_FLAG_TXE)
  {
    // Use lr to put all data to one channel
    if (lr == 0)
    {
      lr = 1;
      SPI_I2S_SendData16(SPI1, (uint16_t)voice_data[TxIdx++]*0x2F);
      if (TxIdx == VOICE_SIZE)
      {
        TxIdx = 0;
        // Disable the I2S1 TXE Interrupt to stop playing
        SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, DISABLE);
      }
    }
    else
    {
      lr = 0;
      SPI_I2S_SendData16(SPI1, 0);
    }
  }
}

int main(void)
{
  SysTick_Init();

  SysTick_DelayMs(1000);

  I2Sx_GPIO_Config();
  I2Sx_Config();
  /* Enable the I2S */
  I2S_Cmd(SPI1, ENABLE);

  while (1)
  {
    // Enable the I2S1 TX Interrupt, restart the playing
    SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, ENABLE);
    SysTick_DelayMs(3000);
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
