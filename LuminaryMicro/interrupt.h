//*****************************************************************************
//
// interrupt.h - Prototypes for the NVIC Interrupt Controller Driver.
//
// Copyright (c) 2005-2008 Luminary Micro, Inc.  All rights reserved.
// 
// Software License Agreement
// 
// Luminary Micro, Inc. (LMI) is supplying this software for use solely and
// exclusively on LMI's microcontroller products.
// 
// The software is owned by LMI and/or its suppliers, and is protected under
// applicable copyright laws.  All rights are reserved.  You may not combine
// this software with "viral" open-source software in order to form a larger
// program.  Any use in violation of the foregoing restrictions may subject
// the user to criminal sanctions under applicable laws, as well as to civil
// liability for the breach of the terms and conditions of this license.
// 
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
// CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2523 of the Stellaris Peripheral Driver Library.
//
//*****************************************************************************

#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

/* -------------------------  Interrupt Number Definition  ------------------------ */
#define BASE_USER_IRQn 16
typedef enum {
                                 /* ---------------------  LM3S6965 Specific Interrupt Numbers  -------------------- */
    GPIOA_IRQn                    =   0,              /*!<   0  GPIOA                                                            */
    GPIOB_IRQn                    =   1,              /*!<   1  GPIOB                                                            */
    GPIOC_IRQn                    =   2,              /*!<   2  GPIOC                                                            */
    GPIOD_IRQn                    =   3,              /*!<   3  GPIOD                                                            */
    GPIOE_IRQn                    =   4,              /*!<   4  GPIOE                                                            */
    UART0_IRQn                    =   5,              /*!<   5  UART0                                                            */
    UART1_IRQn                    =   6,              /*!<   6  UART1                                                            */
    SSI0_IRQn                     =   7,              /*!<   7  SSI0                                                             */
    I2C0_IRQn                     =   8,              /*!<   8  I2C0                                                             */
    PWM0_FAULT_IRQn               =   9,              /*!<   9  PWM0_FAULT                                                       */
    PWM0_0_IRQn                   =  10,              /*!<  10  PWM0_0                                                           */
    PWM0_1_IRQn                   =  11,              /*!<  11  PWM0_1                                                           */
    PWM0_2_IRQn                   =  12,              /*!<  12  PWM0_2                                                           */
    QEI0_IRQn                     =  13,              /*!<  13  QEI0                                                             */
    ADC0SS0_IRQn                  =  14,              /*!<  14  ADC0SS0                                                          */
    ADC0SS1_IRQn                  =  15,              /*!<  15  ADC0SS1                                                          */
    ADC0SS2_IRQn                  =  16,              /*!<  16  ADC0SS2                                                          */
    ADC0SS3_IRQn                  =  17,              /*!<  17  ADC0SS3                                                          */
    WATCHDOG0_IRQn                =  18,              /*!<  18  WATCHDOG0                                                        */
    TIMER0A_IRQn                  =  19,              /*!<  19  TIMER0A                                                          */
    TIMER0B_IRQn                  =  20,              /*!<  20  TIMER0B                                                          */
    TIMER1A_IRQn                  =  21,              /*!<  21  TIMER1A                                                          */
    TIMER1B_IRQn                  =  22,              /*!<  22  TIMER1B                                                          */
    TIMER2A_IRQn                  =  23,              /*!<  23  TIMER2A                                                          */
    TIMER2B_IRQn                  =  24,              /*!<  24  TIMER2B                                                          */
    COMP0_IRQn                    =  25,              /*!<  25  COMP0                                                            */
    COMP1_IRQn                    =  26,              /*!<  26  COMP1                                                            */
    COMP2_IRQn                    =  27,              /*!<  27  COMP2                                                            */
    SYSCTL_IRQn                   =  28,              /*!<  28  SYSCTL                                                           */
    FLASH_CTRL_IRQn               =  29,              /*!<  29  FLASH_CTRL                                                       */
    GPIOF_IRQn                    =  30,              /*!<  30  GPIOF                                                            */
    GPIOG_IRQn                    =  31,              /*!<  31  GPIOG                                                            */
    UART2_IRQn                    =  33,              /*!<  33  UART2                                                            */
    TIMER3A_IRQn                  =  35,              /*!<  35  TIMER3A                                                          */
    TIMER3B_IRQn                  =  36,              /*!<  36  TIMER3B                                                          */
    I2C1_IRQn                     =  37,              /*!<  37  I2C1                                                             */
    QEI1_IRQn                     =  38,              /*!<  38  QEI1                                                             */
    ETH_IRQn                      =  42,              /*!<  42  ETH                                                              */
    HIB_IRQn                      =  43,              /*!<  43  HIB                                                              */
    PWM0_3_IRQn                   =  45               /*!<  45  PWM0_3                                                           */
} IRQn_Type;

//*****************************************************************************
//
// Macro to generate an interrupt priority mask based on the number of bits
// of priority supported by the hardware.
//
//*****************************************************************************
#define INT_PRIORITY_MASK       ((0xFF << (8 - NUM_PRIORITY_BITS)) & 0xFF)

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
extern tBoolean IntMasterEnable(void);
extern tBoolean IntMasterDisable(void);
extern void IntRegister(unsigned long ulInterrupt, void (*pfnHandler)(void));
extern void IntUnregister(unsigned long ulInterrupt);
extern void IntPriorityGroupingSet(unsigned long ulBits);
extern unsigned long IntPriorityGroupingGet(void);
extern void IntPrioritySet(unsigned long ulInterrupt,
                           unsigned char ucPriority);
extern long IntPriorityGet(unsigned long ulInterrupt);
extern void IntEnable(unsigned long ulInterrupt);
extern void IntDisable(unsigned long ulInterrupt);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __INTERRUPT_H__
