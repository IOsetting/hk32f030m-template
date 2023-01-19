/**
  ******************************************************************************
  * @brief   FreeRTOS task test - LED Blink
  * 
  ******************************************************************************
  */
#include "hk32f030m.h"
#include "systick_delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

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

void task1(void *pvParameters)
{
  (void)(pvParameters); // Suppress "unused parameter" warning

  while (1)
  {
    GPIO_Toggle(LED1_GPIO_PORT, LED1_GPIO_PIN);
    vTaskDelay(500);
  }
}

void task2(void *pvParameters)
{
  (void)(pvParameters);

  while (1)
  {
    GPIO_Toggle(LED2_GPIO_PORT, LED2_GPIO_PIN);
    vTaskDelay(300);
  }
}

void task3(void *pvParameters)
{
  (void)(pvParameters);

  while (1)
  {
    GPIO_Toggle(LED3_GPIO_PORT, LED3_GPIO_PIN);
    vTaskDelay(800);
  }
}

int main(void)
{
  BaseType_t xReturned;

  LED_GPIO_Config();

  /* Create task 1 */
  xReturned = xTaskCreate(
      task1,                    // Task function point
      "Task1",                  // Task name
      configMINIMAL_STACK_SIZE, // Use the minimum stack size, each take 4 bytes(32bit)
      NULL,                     // Parameters
      2,                        // Priority
      NULL);                    // Task handler
  if (xReturned != pdPASS)
  {
    while (1);
  }

  /* Create task 2 */
  xReturned = xTaskCreate(task2, "Task2", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
  if (xReturned != pdPASS)
  {
    while (1);
  }

  /* Create task 3 */
  xReturned = xTaskCreate(task3, "Task3", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
  if (xReturned != pdPASS)
  {
    while (1);
  }

  /* Start the scheduler. */
  vTaskStartScheduler();

  /* Will only get here if there was not enough heap space to create the idle task. */
  return 0;
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
