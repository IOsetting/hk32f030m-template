#include <stdint.h>

void SysTick_Init(void);
void SysTick_Handler(void);
volatile uint32_t millis(void);
void SysTick_DelayMs(uint16_t ms);
