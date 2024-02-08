#include <stdint.h>

void EXTI_Config(void);
void EXTI1_IRQHandler(void);
void EXTI3_IRQHandler(void);

extern volatile uint8_t encoder_btn_event;
extern volatile int16_t encoder_direction;
extern volatile uint32_t encoder_polled;