#include "hk32f030m.h"
#include <stdint.h>

volatile uint32_t millis_ticks = 0;

// Set up SysTick to interrupt every millisecond
void SysTick_Init(void) {
  // set up systick and enable interrupt
  SysTick_Config(SystemCoreClock / 8000); // trigger ISR (32MHz/8000 Hz)
  // set SysTick clock source to 1/8 of HCLK (SysTick_Config seems to set to
  // Div1)
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
  // will now run whatever is in SysTick_Handler() every millisecond.
}

// SysTick ISR
void SysTick_Handler(void) {
  millis_ticks++; // rolls over every ~50 days
}

/// @brief millis() - provides timestamps
/// @param  none
/// @return returns the current count (in milliseconds) since boot
volatile uint32_t millis(void) { return millis_ticks; }

/// @brief SysTick_DelayMs(uint16_t ms) - blocking delay
/// @param ms - number of seconds to delay for
void SysTick_DelayMs(uint16_t ms) {
  uint32_t start = millis();
  while ((millis() - start) < ms)
    ;
}
