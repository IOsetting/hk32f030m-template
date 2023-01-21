#include "hk32f030m.h"
#include "systick_delay.h"

// LED1: PD1
#define LED1_GPIO_PORT        GPIOD
#define LED1_GPIO_CLK         RCC_AHBPeriph_GPIOD
#define LED1_GPIO_PIN         GPIO_Pin_1

volatile uint32_t time = 0;

void LED_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  // Enable AHB Clock
  RCC_AHBPeriphClockCmd(LED1_GPIO_CLK, ENABLE);

  // LED1: Output, push-pull, speed 10Mhz
  GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);
}

/*
typedef struct
{ TIM_Prescaler            TIM1,2,6
	TIM_CounterMode			     TIM1,2
  TIM_Period               TIM1,2,6
  TIM_ClockDivision        TIM1,2
  TIM_RepetitionCounter    TIM1
}TIM_TimeBaseInitTypeDef; 

TIM6 only got TIM_Prescaler and TIM_Period
 */
void BasicTim_Config(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
  // TIM6 Clock = 32,000,000 / (31 + 1) = 1,000,000 Hz
  TIM_TimeBaseStructure.TIM_Prescaler = 31;
  // Interrupt frequency = 1,000,000 / (999 + 1) = 1,000 Hz
  TIM_TimeBaseStructure.TIM_Period = 999;
  // Not for basic timer
  // TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // Not for basic timer. Basic timer always counts upwards
  // TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
  // Not for basic timer
  // TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
  // Initialize
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
  // Clear interrupt flag
  TIM_ClearFlag(TIM6, TIM_FLAG_Update);
  // Enable interrupt
  TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
  // Enable timer
  TIM_Cmd(TIM6, ENABLE);
}

void BasicTim_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  // Interrupt source
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
  // Priority
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void TIM6_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
  {
    time++;
    TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
  }
}

int main(void)
{
  SysTick_Init();
  LED_GPIO_Config();

  BasicTim_Config();
  BasicTim_NVIC_Config();

  while (1)
  {
    if (time == 1000)
    {
      time = 0;
      GPIO_Toggle(LED1_GPIO_PORT, LED1_GPIO_PIN);
    }
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
