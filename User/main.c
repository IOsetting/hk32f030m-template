/**
  ******************************************************************************
  * @file    main.c
  * @author  Alexander
  * @version V1.0
  * @date    2022-xx-xx
  * @brief   测试led
  ******************************************************************************
  */ 
#include "hk32f030m.h" 
#include "bsp_led.h"

#define SOFT_DELAY  Delay(0x0FFFFF);

void Delay(__IO uint32_t nCount); 

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
 
int main(void)
{
	/* LED 端口初始化 */
	LED_GPIO_Config();	
  
  while (1)
	{
    /* 定义控制IO的宏 */
		LED1_ON;			  // 亮
		SOFT_DELAY;
		LED1_OFF;		   // 灭

		LED2_ON;			 // 亮
		SOFT_DELAY;
		LED2_OFF;		   // 灭

		LED3_ON;			 // 亮
		SOFT_DELAY;
		LED3_OFF;		   // 灭

    /* 使用标准的固件库控制IO*/
    LED1(ON);			 // 亮
		SOFT_DELAY;
		LED1(OFF);		 // 灭
    
    LED1(ON);			 // 亮
		SOFT_DELAY;
		LED1(OFF);		 // 灭
    
    LED1(ON);			 // 亮
		SOFT_DELAY;
		LED1(OFF);		 // 灭
    
    /* 直接操作寄存器的方法控制IO */
    digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN);
		SOFT_DELAY;
    digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN);
    
    digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN);
		SOFT_DELAY;
    digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN);
    
    digitalLo(LED3_GPIO_PORT,LED3_GPIO_PIN);
		SOFT_DELAY;
    digitalHi(LED3_GPIO_PORT,LED3_GPIO_PIN);
    
	}
}

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
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


