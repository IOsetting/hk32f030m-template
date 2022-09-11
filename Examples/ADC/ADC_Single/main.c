/* Includes ------------------------------------------------------------------*/
#include "systick_delay.h"
#include "uart_printf.h"

uint16_t adc_channel_1_value;
uint16_t adc_channel_2_value;
uint16_t adc_channel_3_value;
uint16_t adc_channel_4_value;

static void ADCx_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD,ENABLE);
  // PC4 (AIN2), Analog mode
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  // PD3 (AIN3)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  // PD2 (AIN4)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void ADCx_Mode_Config(void)
{
  ADC_InitTypeDef ADC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC ,ENABLE);

  ADC_DeInit(ADC1);

  // Important: This assignment is required
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  // Continous mode
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  // Right alignment
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  // Scan direction
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStructure);
  // Calibration
  ADC_GetCalibrationFactor(ADC1);
  // Enable ADC1
  ADC_Cmd(ADC1, ENABLE);
  // Wait till ADC1 ready
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
}

uint16_t ADC_ConvertByChannel(uint32_t ADC_Channel)
{
  ADC_StopOfConversion(ADC1);
  ADC_ChannelConfig(ADC1, ADC_Channel, ADC_SampleTime_239_5Cycles);
  ADC_StartOfConversion(ADC1);
  // Wait till end-of-conversion is set
  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
  return ADC_GetConversionValue(ADC1);
}

int main(void)
{
  uint16_t ret;

  SysTick_Init();
  UART_PrintfInit();
  ADCx_GPIO_Config();
  ADCx_Mode_Config();

  /* Infinite loop */
  while (1)
  {
    ret = ADC_ConvertByChannel(ADC_Channel_2);
    printf("ADC CH2:%04X\r\n", ret);

    ret = ADC_ConvertByChannel(ADC_Channel_3);
    printf("ADC CH3:%04X\r\n", ret);

    ret = ADC_ConvertByChannel(ADC_Channel_4);
    printf("ADC CH4:%04X\r\n\r\n", ret);

    SysTick_DelayMs(500);
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


