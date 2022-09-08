#include "systick_delay.h"
#include "hk32f030m.h"

static uint32_t fac_us = 0;
static uint32_t fac_ms = 0;

/*******************************************************************************
* Function Name  : SysTick_Init
* Description    : SysTick Initialize
* Input          : none
* Output         : none
* Return         : none
*                  Set SysTick clock source to 1/8 of HCLK
*******************************************************************************/
void SysTick_Init(void)
{
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
  fac_us = SystemCoreClock / 8000000;
  fac_ms = SystemCoreClock / 8000;
}

/*******************************************************************************
* Function Name  : SysTick_DelayUs
* Description    : delay by microseconds
* Input          : nus -- number of microseconds
* Output         : none
* Return         : none
*******************************************************************************/
void SysTick_DelayUs(uint32_t nus)
{
  uint32_t temp;

  if (nus > 0)
  {
    SysTick->LOAD = nus * fac_us;               // Set Reload Value
    SysTick->VAL = 0x00;                        // Set Current Value to 0
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   // Start counter
    // Block when SysTick is running and counter flag not set
    do
    {
      temp = SysTick->CTRL;
    } 
    while ((temp & SysTick_CTRL_ENABLE_Msk) && !(temp & SysTick_CTRL_COUNTFLAG_Msk));
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  // Stop counter
  }
}

/*******************************************************************************
* Function Name  : SysTick_DelayMs
* Description    : Delay by milliseconds
                   SysTick->LOAD is 24-bit so the max delay = 0xFFFFFF*8*1000 / SYSCLK
* Input          : nms -- number of milliseconds
*******************************************************************************/
void SysTick_DelayMs(uint16_t nms)
{
  uint32_t temp;

  if (nms > 0)
  {
    SysTick->LOAD = nms * fac_ms;               // Set Reload Value (24-bit)
    SysTick->VAL = 0x00;                        // Set Current Value to 0
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   // Start counter
    // Block when SysTick is running and counter flag not set
    do
    {
      temp = SysTick->CTRL;
    } 
    while ((temp & SysTick_CTRL_ENABLE_Msk) && !(temp & SysTick_CTRL_COUNTFLAG_Msk));
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  // Stop counter
  }
}
