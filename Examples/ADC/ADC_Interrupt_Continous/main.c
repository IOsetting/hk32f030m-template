/**
  ******************************************************************************
  * @file    main.c
  * @version V1.0
  * @brief   ADC 1-channel continuous conversion using interrupt
  ******************************************************************************
  */ 
#include "hk32f030m.h"
#include "systick_delay.h"
#include "uart_printf.h"

__IO uint16_t ret_val;

static void ADCx_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
  // Analog mode
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

static void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;	

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC, ENABLE);

  ADC_DeInit(ADC1);
  // Important: This assignment is required
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  // Continous mode
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  // Right alignment
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  // Scan direction
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStructure);

  // Enable channel 4 for conversion
  ADC_ChannelConfig(ADC1, ADC_Channel_4, ADC_SampleTime_239_5Cycles);
  // Calibration
  ADC_GetCalibrationFactor(ADC1);
  // Enable end-of-conversion interrupt
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
  // Enable ADC1
  ADC_Cmd(ADC1, ENABLE);
  // Wait till ADC1 ready
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
  // Start conversion
	ADC_StartOfConversion(ADC1);
}

static void ADC_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void ADC1_IRQHandler(void)
{
  if (ADC_GetITStatus(ADC1, ADC_IT_EOC) == SET)
  {
    ret_val = ADC_GetConversionValue(ADC1);
    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
  }
}

int main(void)
{
  SysTick_Init();
  UART_PrintfInit();
  
  ADCx_GPIO_Config();
	ADCx_Mode_Config();
	ADC_NVIC_Config();

  while (1)
  {
    printf("ADC: 0x%04X\r\n", ret_val);
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
	
	while (1);
}
#endif /* USE_FULL_ASSERT */
