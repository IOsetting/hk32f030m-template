#include "hk32f030m.h"
#include <stdint.h>

/// @brief Configure TIM1CH1 for 2kHz 14-bit PWM
/// @param  none
void TIM1_Config(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;

  TIM_DeInit(TIM1);
  // Enable peripheral Clocks
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  // PWM: Output, push-pull, speed 10Mhz
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6,
                   GPIO_AF_3); // Assign to AF3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Schmit = GPIO_Schmit_Disable;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // Auto-Reload ARR, Timer1 period, Timer1 frequency = Timer1 clock / (ARR + 1)
  TIM_TimeBaseStructure.TIM_Period = (16384 - 1);
  // Prescaler PSC, Timer1 clock = FCLK / (PSC + 1)
  TIM_TimeBaseStructure.TIM_Prescaler = (0);
  // CR1_CKD, for dead-time insertion
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  // Counter mode
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
  // RCR, Repetition Counter value
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  // Output Compare Mode 1
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  // Enable output
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  // CCR1, Specifies the pulse value to be loaded into the Capture Compare
  // Register
  TIM_OCInitStructure.TIM_Pulse = 0;
  // Output polarity
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  // Pin state during Idle state
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;

  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

  // Enable Timer1
  TIM_Cmd(TIM1, ENABLE);
  // Enable PWM output
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
}