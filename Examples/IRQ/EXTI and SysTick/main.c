/**
  ******************************************************************************
  * @file       main.c
  * @author     Brad
  * @version    V1.0
  * @date       2024-xx-xx
  * @brief      IkeaDimmer - reads value from a rotary encoder, and sets a PWM
                output
  ******************************************************************************
  */
#include "GPIO_EXTI.h"
#include "SysTick_IRQ.h"
#include "TIM1PWM.h"
#include "hk32f030m.h"
#include <stdint.h>

/// @brief enc_read(void) - read and process rotary encoder inputs
/// @param  none (reads global 'encoder_direction', updated by EXTI ISR)
/// @return Shaped value from the encoder (shape is linear, *8)
int16_t enc_read(void) {
#define POLLRATE 10
  // ms, how often to check for input and
  // accelerate/decelerate
  //(acceleration/velocity detection not currently implemented)

  int16_t Delta = 0;
  if ((millis() - encoder_polled) > POLLRATE) {
    Delta = encoder_direction * 8;
    encoder_direction = 0; // reset ISR counter
  }
  return Delta; // e.g. A cubic acceleration curve could be 'return
                // Delta*Delta*Delta'
}

int main(void) {
  // initialize Systick and start interrupt
  SysTick_Init();
  // not sure why this is necessary,
  // but TIM PWM polarity is unpredictable without
  // a delay before init. Power supply glitching?
  SysTick_DelayMs(100);
  // initialize TIM1, and start PWM output on tim1ch1
  TIM1_Config();
  // initialize exti interrupts on encoder gpio inputs
  EXTI_Config();

  uint8_t brightness = 0;
  uint8_t saved_brightness = 255;
  int16_t new_brightness = 0;
  TIM_SetCompare1(TIM1, brightness);

  const uint32_t FadeTime = 3600000; // ms; 1hr = 3600*1000ms
  uint32_t FadeInterval = (FadeTime / (brightness + 1));
  uint32_t FadeEvent = millis();

  // Blip the LED to indicate life
  for (uint8_t i = 0; i < 254; i += 2) {
    TIM_SetCompare1(TIM1, (((uint16_t)i * (uint16_t)i) >> 2));
    SysTick_DelayMs(1);
  }
  TIM_SetCompare1(TIM1, brightness);

  // main program loop
  while (1) {
    int32_t user_input = enc_read(); // check for activity on the encoder wheel

    // Limit checking
    new_brightness += user_input;
    if (new_brightness < 0)
      new_brightness = 0;
    if (new_brightness > 0xFF)
      new_brightness = 0xFF;

    if (encoder_btn_event) { // check for activity from the encoder button
      user_input = 1;
      encoder_btn_event = 0;
      if (brightness) {
        new_brightness = 0;
        saved_brightness = brightness;
      } else
        new_brightness = saved_brightness;
    }

    // Update LED brightness, if needed
    if ((uint8_t)new_brightness != brightness) {
      brightness = (uint8_t)new_brightness;
      TIM_SetCompare1(TIM1, ((uint16_t)brightness * (uint16_t)brightness) >> 2);
    }

    // Reset Fade out timer if the user has made a setting
    if (user_input)
      FadeInterval = FadeTime / (brightness + 1);

    // Handle fading
    if ((millis() - FadeEvent) > FadeInterval) {
      if (brightness > 1)
        new_brightness--;
      FadeEvent = millis();
    }

    SysTick_DelayMs(10);
    // Is there a sleep mode I can enter here instead that leaves TIM1 running?
  }
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(char *file, uint32_t line) {
  /* User can add his own implementation to report the file name and line
     number, tex: printf("Wrong parameters value: file %s on line %d\r\n", file,
     line) */
  /* Infinite loop */

  while (1) {
  }
}
#endif /* USE_FULL_ASSERT */
