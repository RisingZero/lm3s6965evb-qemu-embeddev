/******************************************************************************
 * @file     startup_ARMCM3.c
 * @brief    CMSIS-Core(M) Device Startup File for a Cortex-M3 Device
 * @version  V2.0.3
 * @date     31. March 2020
 ******************************************************************************/
/*
 * Copyright (c) 2009-2020 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ARMCM3.h"

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;

extern __NO_RETURN void __PROGRAM_START(void);
/* FreeRTOS handlers */
extern void vPortSVCHandler         (void);
extern void xPortPendSVHandler      (void);
extern void xPortSysTickHandler     (void);
extern void Timer0IntHandler        (void);
extern void vT2InterruptHandler     (void);
extern void vT3InterruptHandler     (void);

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler  (void);
            void Default_Handler(void);

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
/* Exceptions */
void NMI_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler      (void) __attribute__ ((weak));
void MemManage_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));

void IntDefaultHandler      (void) __attribute__ ((weak, alias("Default_Handler")));

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

extern const VECTOR_TABLE_Type __VECTOR_TABLE[240];
       const VECTOR_TABLE_Type __VECTOR_TABLE[240] __VECTOR_TABLE_ATTRIBUTE = {
  (VECTOR_TABLE_Type)(&__INITIAL_SP),       /*     Initial Stack Pointer */
  Reset_Handler,                            /*     Reset Handler */
  NMI_Handler,                              /* -14 NMI Handler */
  HardFault_Handler,                        /* -13 Hard Fault Handler */
  MemManage_Handler,                        /* -12 MPU Fault Handler */
  BusFault_Handler,                         /* -11 Bus Fault Handler */
  UsageFault_Handler,                       /* -10 Usage Fault Handler */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  vPortSVCHandler,                              /*  -5 SVC Handler */
  DebugMon_Handler,                         /*  -4 Debug Monitor Handler */
  0,                                        /*     Reserved */
  xPortPendSVHandler,                           /*  -2 PendSV Handler */
  xPortSysTickHandler,                          /*  -1 SysTick Handler */

  IntDefaultHandler,                      // GPIO Port A
  IntDefaultHandler,                      // GPIO Port B
  IntDefaultHandler,                      // GPIO Port C
  IntDefaultHandler,                      // GPIO Port D
  IntDefaultHandler,                      // GPIO Port E
  IntDefaultHandler,                      // UART0 Rx and Tx
  IntDefaultHandler,                      // UART1 Rx and Tx
  IntDefaultHandler,                      // SSI Rx and Tx
  IntDefaultHandler,                      // I2C Master and Slave
  IntDefaultHandler,                      // PWM Fault
  IntDefaultHandler,                      // PWM Generator 0
  IntDefaultHandler,                      // PWM Generator 1
  IntDefaultHandler,                      // PWM Generator 2
  IntDefaultHandler,                      // Quadrature Encoder
  IntDefaultHandler,                      // ADC Sequence 0
  IntDefaultHandler,                      // ADC Sequence 1
  IntDefaultHandler,                      // ADC Sequence 2
  IntDefaultHandler,                      // ADC Sequence 3
  IntDefaultHandler,                      // Watchdog timer
  IntDefaultHandler,                       // Timer 0 subtimer A
  IntDefaultHandler,                      // Timer 0 subtimer B
  IntDefaultHandler,                      // Timer 1 subtimer A
  IntDefaultHandler,                      // Timer 1 subtimer B
  IntDefaultHandler,                    // Timer 2 subtimer A
  IntDefaultHandler,                      // Timer 2 subtimer B
  IntDefaultHandler,                      // Analog Comparator 0
  IntDefaultHandler,                      // Analog Comparator 1
  IntDefaultHandler,                      // Analog Comparator 2
  IntDefaultHandler,                      // System Control (PLL, OSC, BO)
  IntDefaultHandler,                      // FLASH Control
  IntDefaultHandler,                      // GPIO Port F
  IntDefaultHandler,                      // GPIO Port G
  IntDefaultHandler,                      // GPIO Port H
  IntDefaultHandler,                      // UART2 Rx and Tx
  IntDefaultHandler,                      // SSI1 Rx and Tx
  IntDefaultHandler,                    // Timer 3 subtimer A
  IntDefaultHandler,                      // Timer 3 subtimer B
  IntDefaultHandler,                      // I2C1 Master and Slave
  IntDefaultHandler,                      // Quadrature Encoder 1
  IntDefaultHandler,                      // CAN0
  IntDefaultHandler,                      // CAN1
  0,                                      // Reserved
  IntDefaultHandler,                      // Ethernet
  IntDefaultHandler                       // Hibernate
};

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler( void )
{
  SystemInit();                             /* CMSIS System Initialization */
  __PROGRAM_START();                        /* Enter PreMain (C library entry point) */
}


#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wmissing-noreturn"
#endif

/*----------------------------------------------------------------------------
  Hard Fault Handler
 *----------------------------------------------------------------------------*/
void HardFault_Handler( void )
{
  while(1);
}

/*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
void Default_Handler( void )
{
  while(1);
}

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic pop
#endif

