/**
  ******************************************************************************
  * @file      startup_hk32f030mf4p6.s
  * @brief     HK32F030MF4P6 devices vector table for GCC toolchain.
  *            This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == Reset_Handler,
  *                - Set the vector table entries with the exceptions ISR address
  *                - Branches to main in the C library (which eventually calls main()).
  *            After Reset the Cortex-M0 processor is in Thread mode,
  *            priority is Privileged, and the Stack is set to Main.
  ******************************************************************************
  */

  .syntax unified
  .cpu cortex-m0
  .fpu softvfp
  .thumb

.global g_pfnVectors
.global Default_Handler

/* start address for the initialization values of the .data section.
defined in linker script */
.word _sidata
/* start address for the .data section. defined in linker script */
.word _sdata
/* end address for the .data section. defined in linker script */
.word _edata
/* start address for the .bss section. defined in linker script */
.word _sbss
/* end address for the .bss section. defined in linker script */
.word _ebss

  .section .text.Reset_Handler
  .weak Reset_Handler
  .type Reset_Handler, %function
Reset_Handler:
  ldr   r0, =_estack
  mov   sp, r0          /* set stack pointer */

/* Copy the data segment initializers from flash to SRAM */
  ldr r0, =_sdata
  ldr r1, =_edata
  ldr r2, =_sidata
  movs r3, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r4, [r2, r3]
  str r4, [r0, r3]
  adds r3, r3, #4

LoopCopyDataInit:
  adds r4, r0, r3
  cmp r4, r1
  bcc CopyDataInit
  
/* Zero fill the bss segment. */
  ldr r2, =_sbss
  ldr r4, =_ebss
  movs r3, #0
  b LoopFillZerobss

FillZerobss:
  str  r3, [r2]
  adds r2, r2, #4

LoopFillZerobss:
  cmp r2, r4
  bcc FillZerobss

/* Call the clock system intitialization function.*/
  bl  SystemInit
/* Call static constructors. Remove this line if compile with `-nostartfiles` reports error */
  bl __libc_init_array
/* Call the application's entry point.*/
  bl main

LoopForever:
    b LoopForever


.size Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 *
 * @param  None
 * @retval : None
*/
    .section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b Infinite_Loop
  .size Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M0.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/
   .section .isr_vector,"a",%progbits
  .type g_pfnVectors, %object
  .size g_pfnVectors, .-g_pfnVectors


g_pfnVectors:
  .word  _estack
  .word  Reset_Handler
  .word  NMI_Handler
  .word  HardFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  0
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler  
  .word  WWDG_IRQHandler                   /* Window WatchDog              */
  .word  0                                 /* Reserved                     */
  .word  EXTI11_IRQHandler                 /* EXTI Line 11 interrupt(AWU_WKP) */
  .word  FLASH_IRQHandler                  /* FLASH                        */
  .word  RCC_IRQHandler                    /* RCC                          */
  .word  EXTI0_IRQHandler                  /* EXTI Line 0                  */
  .word  EXTI1_IRQHandler                  /* EXTI Line 1                  */
  .word  EXTI2_IRQHandler                  /* EXTI Line 2                  */
  .word  EXTI3_IRQHandler                  /* EXTI Line 3                  */
  .word  EXTI4_IRQHandler                  /* EXTI Line 4                  */
  .word  EXTI5_IRQHandler                  /* EXTI Line 5                  */
  .word  TIM1_BRK_IRQHandler               /* TIM1 break interrupt         */
  .word  ADC1_IRQHandler                   /* ADC1 interrupt, combined with EXTI line 8 */
  .word  TIM1_UP_TRG_COM_IRQHandler        /* TIM1 Update, Trigger and Commutation */
  .word  TIM1_CC_IRQHandler                /* TIM1 Capture Compare         */
  .word  TIM2_IRQHandler                   /* TIM2                         */
  .word  0                                 /* Reserved                     */
  .word  TIM6_IRQHandler                   /* TIM6                         */
  .word  0                                 /* Reserved                     */
  .word  0                                 /* Reserved                     */
  .word  0                                 /* Reserved                     */
  .word  EXTI6_IRQHandler                  /* EXTI Line 6                  */
  .word  EXTI7_IRQHandler                  /* EXTI Line 7                  */
  .word  I2C1_IRQHandler                   /* I2C1 global interrupt, combined with EXTI Line 10 */
  .word  0                                 /* Reserved                     */
  .word  SPI1_IRQHandler                   /* SPI1                         */
  .word  0                                 /* Reserved                     */
  .word  USART1_IRQHandler                 /* USART1 global interrupt, combined with EXTI Line 9 */
  .word  0                                 /* Reserved                     */
  .word  0                                 /* Reserved                     */
  .word  0                                 /* Reserved                     */
  .word  0                                 /* Reserved                     */

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

  .weak      NMI_Handler
  .thumb_set NMI_Handler,Default_Handler

  .weak      HardFault_Handler
  .thumb_set HardFault_Handler,Default_Handler

  .weak      SVC_Handler
  .thumb_set SVC_Handler,Default_Handler

  .weak      PendSV_Handler
  .thumb_set PendSV_Handler,Default_Handler

  .weak      SysTick_Handler
  .thumb_set SysTick_Handler,Default_Handler

  .weak      WWDG_IRQHandler
  .thumb_set WWDG_IRQHandler,Default_Handler

  .weak      EXTI11_IRQHandler
  .thumb_set EXTI11_IRQHandler,Default_Handler

  .weak      FLASH_IRQHandler
  .thumb_set FLASH_IRQHandler,Default_Handler

  .weak      RCC_IRQHandler
  .thumb_set RCC_IRQHandler,Default_Handler

  .weak      EXTI0_IRQHandler
  .thumb_set EXTI0_IRQHandler,Default_Handler

  .weak      EXTI1_IRQHandler
  .thumb_set EXTI1_IRQHandler,Default_Handler

  .weak      EXTI2_IRQHandler
  .thumb_set EXTI2_IRQHandler,Default_Handler

  .weak      EXTI3_IRQHandler
  .thumb_set EXTI3_IRQHandler,Default_Handler

  .weak      EXTI4_IRQHandler
  .thumb_set EXTI4_IRQHandler,Default_Handler

  .weak      EXTI5_IRQHandler
  .thumb_set EXTI5_IRQHandler,Default_Handler

  .weak      TIM1_BRK_IRQHandler
  .thumb_set TIM1_BRK_IRQHandler,Default_Handler

  .weak      ADC1_IRQHandler
  .thumb_set ADC1_IRQHandler,Default_Handler

  .weak      TIM1_UP_TRG_COM_IRQHandler
  .thumb_set TIM1_UP_TRG_COM_IRQHandler,Default_Handler

  .weak      TIM1_CC_IRQHandler
  .thumb_set TIM1_CC_IRQHandler,Default_Handler

  .weak      TIM2_IRQHandler
  .thumb_set TIM2_IRQHandler,Default_Handler

  .weak      TIM6_IRQHandler
  .thumb_set TIM6_IRQHandler,Default_Handler

  .weak      EXTI6_IRQHandler
  .thumb_set EXTI6_IRQHandler,Default_Handler

  .weak      EXTI7_IRQHandler
  .thumb_set EXTI7_IRQHandler,Default_Handler

  .weak      I2C1_IRQHandler
  .thumb_set I2C1_IRQHandler,Default_Handler

  .weak      SPI1_IRQHandler
  .thumb_set SPI1_IRQHandler,Default_Handler

  .weak      USART1_IRQHandler
  .thumb_set USART1_IRQHandler,Default_Handler
