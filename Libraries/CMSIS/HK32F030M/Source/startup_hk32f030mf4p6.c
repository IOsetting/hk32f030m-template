#include <stdint.h>

typedef void (*ptr_func_t)();

extern uint32_t __data_start;
extern uint32_t __data_end;
extern uint32_t __data_load;

extern uint32_t __bss_start__;
extern uint32_t __bss_end__;

extern uint32_t __heap_start__;
extern uint32_t __stacktop;

extern ptr_func_t __init_array_start[];
extern ptr_func_t __init_array_end[];

extern int main(void);
extern void SystemInit(void);

#define WEAK_ALIAS(x) __attribute__ ((weak, alias(#x)))

/* Cortex M0 core interrupt handlers */
void Reset_Handler(void);
void NMI_Handler(void)          WEAK_ALIAS(Dummy_Handler);
void HardFault_Handler(void)    WEAK_ALIAS(Dummy_Handler);
void SVC_Handler(void)          WEAK_ALIAS(Dummy_Handler);
void PendSV_Handler(void)       WEAK_ALIAS(Dummy_Handler);
void SysTick_Handler(void)      WEAK_ALIAS(Dummy_Handler);

void WWDG_IRQHandler(void)      WEAK_ALIAS(Dummy_Handler); /* Window WatchDog              */
void EXTI11_IRQHandler(void)    WEAK_ALIAS(Dummy_Handler); /* EXTI Line 11 interrupt(AWU_WKP) */
void FLASH_IRQHandler(void)     WEAK_ALIAS(Dummy_Handler); /* FLASH                        */
void RCC_IRQHandler(void)       WEAK_ALIAS(Dummy_Handler); /* RCC                          */
void EXTI0_IRQHandler(void)     WEAK_ALIAS(Dummy_Handler); /* EXTI Line 0                  */
void EXTI1_IRQHandler(void)     WEAK_ALIAS(Dummy_Handler); /* EXTI Line 1                  */
void EXTI2_IRQHandler(void)     WEAK_ALIAS(Dummy_Handler); /* EXTI Line 2                  */
void EXTI3_IRQHandler(void)     WEAK_ALIAS(Dummy_Handler); /* EXTI Line 3                  */
void EXTI4_IRQHandler(void)     WEAK_ALIAS(Dummy_Handler); /* EXTI Line 4                  */
void EXTI5_IRQHandler(void)     WEAK_ALIAS(Dummy_Handler); /* EXTI Line 5                  */
void TIM1_BRK_IRQHandler(void)  WEAK_ALIAS(Dummy_Handler); /* TIM1 break interrupt         */
void ADC1_IRQHandler(void)      WEAK_ALIAS(Dummy_Handler); /* ADC1 interrupt, combined with EXTI line 8 */
void TIM1_UP_TRG_COM_IRQHandler(void) WEAK_ALIAS(Dummy_Handler); /* TIM1 Update, Trigger and Commutation */
void TIM1_CC_IRQHandler(void)   WEAK_ALIAS(Dummy_Handler); /* TIM1 Capture Compare         */
void TIM2_IRQHandler(void)      WEAK_ALIAS(Dummy_Handler); /* TIM2                         */
void TIM6_IRQHandler(void)      WEAK_ALIAS(Dummy_Handler); /* TIM6                         */
void EXTI6_IRQHandler(void)     WEAK_ALIAS(Dummy_Handler); /* EXTI Line 6                  */
void EXTI7_IRQHandler(void)     WEAK_ALIAS(Dummy_Handler); /* EXTI Line 7                  */
void I2C1_IRQHandler(void)      WEAK_ALIAS(Dummy_Handler); /* I2C1 global interrupt, combined with EXTI Line 10 */
void SPI1_IRQHandler(void)      WEAK_ALIAS(Dummy_Handler); /* SPI1                         */
void USART1_IRQHandler(void)    WEAK_ALIAS(Dummy_Handler); /* USART1 global interrupt, combined with EXTI Line 9 */


__attribute__((section(".stack"), used)) uint32_t *__stack_init = &__stacktop;

/* Stack top and vector handler table */
__attribute__ ((section(".vectors"), used)) void *vector_table[] = {
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    SVC_Handler,
    0,
    0,
    PendSV_Handler,
    SysTick_Handler,  
    WWDG_IRQHandler,
    0,
    EXTI11_IRQHandler,
    FLASH_IRQHandler,
    RCC_IRQHandler,
    EXTI0_IRQHandler,
    EXTI1_IRQHandler,
    EXTI2_IRQHandler,
    EXTI3_IRQHandler,
    EXTI4_IRQHandler,
    EXTI5_IRQHandler,
    TIM1_BRK_IRQHandler,
    ADC1_IRQHandler,
    TIM1_UP_TRG_COM_IRQHandler,
    TIM1_CC_IRQHandler,
    TIM2_IRQHandler,
    0,
    TIM6_IRQHandler,
    0,
    0,
    0,
    EXTI6_IRQHandler,
    EXTI7_IRQHandler,
    I2C1_IRQHandler,
    0,
    SPI1_IRQHandler,
    0,
    USART1_IRQHandler,
    0,
    0,
    0,
    0};


__attribute__((used)) void Reset_Handler(void)
{
    uint32_t *src, *dst;

    /* Copy data section from Flash to RAM */
    src = &__data_load;
    dst = &__data_start;
    while (dst < &__data_end) *dst++ = *src++;

    /* Clear the bss section */
    dst = &__bss_start__;
    while (dst < &__bss_end__) *dst++ = 0;

    SystemInit();

    for (const ptr_func_t *f = __init_array_start; f < __init_array_end; f++)
    {
        (*f)();
    }

    main();
}

void Dummy_Handler(void)
{
    while (1);
}
