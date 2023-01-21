/**
  ******************************************************************************
  * @file    main.c
  * @brief   Timer1 Complementary Output
  ******************************************************************************
  */ 
#include "hk32f030m.h"
#include "systick_delay.h"

static void PWM_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // PD1
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_3);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Schmit = GPIO_Schmit_Disable;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  // PA1
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_3);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/* ----------------   PWM Period And Duty Cycle  --------------- 
TIM_CLK = FCLK / (PSC+1)
PWM Frequency = FCLK / (PSC+1) / (ARR+1)
Duty Cycle = CCR1 / (ARR+1)
*/
static void ADVANCE_TIM_Mode_Config(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;

  // Enable Timer1 clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  // Auto-Reload ARR, Timer1 period, Timer1 frequency = Timer1 clock / (ARR + 1)
  TIM_TimeBaseStructure.TIM_Period = (256 - 1);
  // Prescaler PSC, Timer1 clock = FCLK / (PSC + 1)
  TIM_TimeBaseStructure.TIM_Prescaler = (32 - 1);
  // CR1_CKD, for dead-time insertion
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  // Counter mode
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  // RCR, Repetition Counter value
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  // Output Compare Mode 1
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  // Enable output
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  // Enable complementary output
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  // CCR1, Specifies the pulse value to be loaded into the Capture Compare Register
  TIM_OCInitStructure.TIM_Pulse = 4;
  // Output polarity
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  // Complementary output polarity
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  // Pin state during Idle state
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  // Complementary pin state during Idle state
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

  // Enable Timer1
  TIM_Cmd(TIM1, ENABLE);
  // Enable PWM output
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

int main(void)
{
  uint8_t compare = 0;

  SysTick_Init();
  PWM_GPIO_Init();
  ADVANCE_TIM_Mode_Config();
  
  while (1)
	{
    TIM1->CCR1 = compare++;
    SysTick_DelayMs(10);
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
