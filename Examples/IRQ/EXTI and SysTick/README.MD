demonstrates the configuration and use of EXTI GPIO interrupts, as well as configuring a SysTick interrupt.


This code configures GPIO EXTI lines to read a rotary encoder, and adjust the duty cycle of a PWM output.

Additionally, this code demonstrates systick as an interrupt that fires every millisecond. Make sure you don't remove the Debug/systick_delay.c from your project as it won't be compatible with SysTick IRQ implemented here

### Connection map
 - Encoder output 'A' - PD1
 - Encoder output 'B' - PD2
 - Encoder pushbutton - PD3

 - PWM output - PC6
