/**
  ******************************************************************************
  * @file    hk32f030m_it.h
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  *
 ******************************************************************************
  */
#ifndef __HK32F030M_IT_H
#define __HK32F030M_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Exported functions prototypes ---------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __HK32F030M_IT_H */
