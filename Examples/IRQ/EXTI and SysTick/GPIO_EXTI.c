#include "SysTick_IRQ.h"
#include "hk32f030m.h"
#include <stdint.h>

volatile uint8_t encoder_btn_event = 0;
volatile int16_t encoder_direction = 0;
volatile uint32_t encoder_polled = 0;

/// @brief EXTI_Config(void) - configures GPIOs, EXTI, and NVIC to enable pin
/// change interrupts
/// @param  none
void EXTI_Config(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  // Enable peripheral Clocks
  RCC_AHBPeriphClockCmd(
      RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOD, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  // Set up NVIC
  // common
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  // D1 - encoder
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  // D3 - encoder button
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  NVIC_Init(&NVIC_InitStructure);

  // set up GPIOs
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Schmit = GPIO_Schmit_Enable;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  // Set up EXTIs
  EXTI_DeInit();
  // common
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  // ENC1
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, GPIO_PinSource1);
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_Init(&EXTI_InitStructure);
  // ENC3 - encoder button
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, GPIO_PinSource3);
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  EXTI_Init(&EXTI_InitStructure);
}

// ISR to handle rotation events from the encoder
void EXTI1_IRQHandler(void) {
  if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
    if (!encoder_direction)
      encoder_polled = millis();
    if ((!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2)))
      encoder_direction++;
    else
      encoder_direction--;
    EXTI_ClearITPendingBit(EXTI_Line1);
  }
}

// ISR to handle button press events from the encoder button
void EXTI3_IRQHandler(void) {
  if (EXTI_GetITStatus(EXTI_Line3) != RESET) {
    encoder_btn_event = 1;
    EXTI_ClearITPendingBit(EXTI_Line3);
  }
}
