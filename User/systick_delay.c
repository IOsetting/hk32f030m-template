#include "systick_delay.h"
#include "hk32f030m.h"

static uint8_t fac_us = 0;
static uint16_t fac_ms = 0;

/*******************************************************************************
*                           @2017-12-16
* Function Name  : delay_init
* Description    : 延时函数初始化
* Input          : none
* Output         : none
* Return         : none
*                  SYSTICK的时钟固定为HCLK时钟的1/8
*                  SYSCLK:系统时钟
*******************************************************************************/
void delay_init(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    fac_us = SystemCoreClock/8000000;
    fac_ms = (uint16_t)fac_us*1000;
}

/*******************************************************************************
*                           @2017-12-16
* Function Name  : delay_us
* Description    : us延时函数
* Input          : nus--延时时间数值
* Output         : none
* Return         : none
*******************************************************************************/
void delay_us(uint32_t nus)
{
  if( nus > 0 )
  {
    uint32_t temp;	    	 
    SysTick->LOAD=nus*fac_us;                      //时间加载	  		 
    SysTick->VAL=0x00;                             //清空计数器
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;       //开始倒数	 
    do
    {
      temp=SysTick->CTRL;
    }
    while(temp&0x01&&!(temp&(1<<16)));             //等待时间到达   
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
    SysTick->VAL =0X00;                            //清空计数器	  
  }
}

/*******************************************************************************
*                           @2017-12-16
* Function Name  : delay_ms
* Description    : 延时nms毫秒时间
                  注意nms的范围
                  SysTick->LOAD为24位寄存器,所以,最大延时为:
                  nms<=0xffffff*8*1000/SYSCLK
                  SYSCLK单位为Hz,nms单位为ms
                  对32M条件下,nms<=4194 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void delay_ms(uint16_t nms)
{	 		  	  
	uint32_t temp;	
  
  if( nms > 0 )
  {
    SysTick->LOAD=(uint32_t)nms*fac_ms;             //时间加载(SysTick->LOAD为24bit)
    SysTick->VAL =0x00;                             //清空计数器
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;        //开始倒数  
    do
    {
      temp=SysTick->CTRL;
    }
    while(temp&0x01&&!(temp&(1<<16)));               //等待时间到达   
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;         //关闭计数器
    SysTick->VAL =0X00;                              //清空计数器	  	    
  }
} 



























