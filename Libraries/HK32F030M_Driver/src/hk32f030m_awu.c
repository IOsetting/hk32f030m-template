/**
  ******************************************************************************
  * @file    hk32f030m_awu.c
  * @author  Rakan.z
  * @version V1.0  
  * @brief   This file contains all the functions for the AWU peripheral.  
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "hk32f030m_awu.h"
/* Public functions ----------------------------------------------------------*/

/**
  * @addtogroup AWU_Public_Functions
  * @{
  */

/**
  * @brief  Deinitializes the AWU peripheral registers to their default reset
  * values.
  * @param  None
  * @retval None
  */
void AWU_DeInit(void)
{
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_AWU, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_AWU, DISABLE);
}

/**
  * @brief  config the AWU clock
  * @param   eAWU_CLK :  
  *  AWU_CLK_LSI128
    AWU_CLK_HSE
  * @retval None
  * @par Required preconditions:
  * The LS RC calibration must be performed before calling this function.
  */
void AWU_CLKConfig(AWU_CLK_TYPE eAWU_CLK)
{
    /* Check parameter */
    assert_param(IS_AWU_CLK(eAWU_CLK));
    MODIFY_REG(AWU->CR, 1, eAWU_CLK);
}

/**
  * @brief  loade the AWU timer counter,This load value will be automatically loaded into the 22-bit timer inside the awu
  *  when the mcu enters stop mode and start timing.
  * @param   TimerCounter : the AWU timer counter
  * @note When awu_rlr [22:1] is '0' or is '1' , the loading behavior will not occur and awu will not start working .
  *       when awu_wbusy =1 ,the write operation on the awu-rlr register will be invalid.
  * @return ErrorStatus: the AWU result 
  *       SUCCESS:AWU timer start success
  *       ERROR:AWU timer start error
  */
ErrorStatus AWU_TimerCounterAndStart(uint32_t TimerCounter)
{
    uint32_t TimeoutCnt = 0;
    while (TimeoutCnt++ < 0x1000)
    {
      if(!(AWU->CR & AWU_CR_RLR_WBUSY)){                    /*  Proceed if  AWU bus idle  */
        MODIFY_REG(AWU->CR, 0x7FFFFE, TimerCounter<<1);
        while((AWU->CR & AWU_CR_RLR_WBUSY));                /*  Wait for AWU bus completion  */
        return SUCCESS;
      }
    }
    return ERROR;                                           /*  AWU bus busy */
}

/**
  * @brief  Returns status of the AWU peripheral flag.
  * @param  None
  * @retval FlagStatus : Status of the AWU flag.
  * This parameter can be any of the @ref FlagStatus enumeration.
  */
FlagStatus AWU_GetFlagStatus(void)
{
    return((FlagStatus)(((uint8_t)(AWU->SR & AWU_SR_BUSY) == (uint8_t)0x00) ? RESET : SET));
}


/**
  * @}
  */
  
