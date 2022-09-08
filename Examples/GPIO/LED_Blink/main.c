/**
  ******************************************************************************
  * @file    main.c
  * @author  Alexander
  * @version V1.0
  * @date    2022-xx-xx
  * @brief   blink test
  ******************************************************************************
  */ 
#include "hk32f030m.h"
#include "systick_delay.h"

// LED1: PD1
#define LED1_GPIO_PORT        GPIOD
#define LED1_GPIO_CLK         RCC_AHBPeriph_GPIOD
#define LED1_GPIO_PIN         GPIO_Pin_1
// LED2: PC7
#define LED2_GPIO_PORT        GPIOC
#define LED2_GPIO_CLK         RCC_AHBPeriph_GPIOC
#define LED2_GPIO_PIN         GPIO_Pin_7
// LED3: PA2
#define LED3_GPIO_PORT        GPIOA
#define LED3_GPIO_CLK         RCC_AHBPeriph_GPIOA
#define LED3_GPIO_PIN         GPIO_Pin_2

void LED_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  // Enable AHB Clock
  RCC_AHBPeriphClockCmd(LED1_GPIO_CLK | LED2_GPIO_CLK | LED3_GPIO_CLK, ENABLE);

  // LED1: Output, push-pull, speed 10Mhz
  GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);

  // LED2
  GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;
  GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);

  // LED3
  GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;
  GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);

  // Turn off all
  GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
  GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
  GPIO_ResetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
}

int main(void)
{
  SysTick_Init();
  /* LED 端口初始化 */
  LED_GPIO_Config();

  while (1)
  {
    // LED1
    GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
    SysTick_DelayMs(500);
    GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
    // LED2
    GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
    SysTick_DelayMs(500);
    GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
    // LED3
    GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
    SysTick_DelayMs(500);
    GPIO_ResetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
    SysTick_DelayMs(500);
    // All
    GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
    GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
    GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
    SysTick_DelayMs(500);
    GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
    GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
    GPIO_ResetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
    SysTick_DelayMs(500);

    // LED1
    GPIO_Toggle(LED1_GPIO_PORT, LED1_GPIO_PIN);
    SysTick_DelayMs(500);
    GPIO_Toggle(LED1_GPIO_PORT, LED1_GPIO_PIN);
    // LED2
    GPIO_Toggle(LED2_GPIO_PORT, LED2_GPIO_PIN);
    SysTick_DelayMs(500);
    GPIO_Toggle(LED2_GPIO_PORT, LED2_GPIO_PIN);
    // LED3
    GPIO_Toggle(LED3_GPIO_PORT, LED3_GPIO_PIN);
    SysTick_DelayMs(500);
    GPIO_Toggle(LED3_GPIO_PORT, LED3_GPIO_PIN);
    SysTick_DelayMs(500);
    // All
    GPIO_Toggle(LED1_GPIO_PORT, LED1_GPIO_PIN);
    GPIO_Toggle(LED2_GPIO_PORT, LED2_GPIO_PIN);
    GPIO_Toggle(LED3_GPIO_PORT, LED3_GPIO_PIN);
    SysTick_DelayMs(500);
    GPIO_Toggle(LED1_GPIO_PORT, LED1_GPIO_PIN);
    GPIO_Toggle(LED2_GPIO_PORT, LED2_GPIO_PIN);
    GPIO_Toggle(LED3_GPIO_PORT, LED3_GPIO_PIN);
    SysTick_DelayMs(500);

    // LED1
    LED1_GPIO_PORT->BSRR = LED1_GPIO_PIN;
    SysTick_DelayMs(500);
    LED1_GPIO_PORT->BRR = LED1_GPIO_PIN;
    // LED2
    LED2_GPIO_PORT->BSRR = LED2_GPIO_PIN;
    SysTick_DelayMs(500);
    LED2_GPIO_PORT->BRR = LED2_GPIO_PIN;
    // LED3
    LED3_GPIO_PORT->BSRR = LED3_GPIO_PIN;
    SysTick_DelayMs(500);
    LED3_GPIO_PORT->BRR = LED3_GPIO_PIN;
    SysTick_DelayMs(500);
    // All
    LED1_GPIO_PORT->BSRR = LED1_GPIO_PIN;
    LED2_GPIO_PORT->BSRR = LED2_GPIO_PIN;
    LED3_GPIO_PORT->BSRR = LED3_GPIO_PIN;
    SysTick_DelayMs(500);
    LED1_GPIO_PORT->BRR = LED1_GPIO_PIN;
    LED2_GPIO_PORT->BRR = LED2_GPIO_PIN;
    LED3_GPIO_PORT->BRR = LED3_GPIO_PIN;
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
