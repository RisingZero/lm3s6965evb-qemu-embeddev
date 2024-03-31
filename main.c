/* Standard includes. */
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* Hardware library includes. */
#include "sysctl.h"
#include "osram128x64x4.h"

/* FreeRTOS includes */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

/* Custom libraries includes */
#include "async_io.h"

/* Constants used when writing strings to the display. */
#define mainCHARACTER_HEIGHT				( 9 )
#define mainMAX_ROWS_128					( mainCHARACTER_HEIGHT * 14 )
#define mainMAX_ROWS_96						( mainCHARACTER_HEIGHT * 10 )
#define mainMAX_ROWS_64						( mainCHARACTER_HEIGHT * 7 )
#define mainFULL_SCALE						( 15 )
#define ulSSI_FREQUENCY						( 3500000UL )
#define STACK_SIZE ( configMINIMAL_STACK_SIZE + 40 )

#include "pong.h"

static void prvSetupHardware(void);
static void task_game(void *pvParameters);
static void task_user(void *pvParameters);

Pong pong;

void vAssertCalled( const char * pcFile,
                    uint32_t ulLine )
{
    volatile uint32_t ulSetTo1InDebuggerToExit = 0;

    taskENTER_CRITICAL();
    {
        while( ulSetTo1InDebuggerToExit == 0 )
        {
            /* Nothing to do here.  Set the loop variable to a non zero value in
             * the debugger to step out of this function to the point that caused
             * the assertion. */
            ( void ) pcFile;
            ( void ) ulLine;
        }
    }
    taskEXIT_CRITICAL();
}

void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    StackType_t ** ppxIdleTaskStackBuffer,
                                    uint32_t * pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
     * state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     StackType_t ** ppxTimerTaskStackBuffer,
                                     uint32_t * pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

volatile char * pcOverflowedTask = NULL; /* Prevent task name being optimised away. */
void vApplicationStackOverflowHook( TaskHandle_t pxTask,
                                    char * pcTaskName )
{
    ( void ) pxTask;
    pcOverflowedTask = pcTaskName;
    vAssertCalled( __FILE__, __LINE__ );
    for( ;; );
}

int main(void) {
    prvSetupHardware();
    pong = pong_init();

    xTaskCreate(
            task_game,
            "game",
            STACK_SIZE,
            NULL,
            tskIDLE_PRIORITY + 4,
            NULL
            );
    xTaskCreate(
            task_user,
            "user",
            STACK_SIZE,
            NULL,
            tskIDLE_PRIORITY + 4,
            NULL
    );

    vTaskStartScheduler();

    /* Should never get here */
    for( ;; );
}

void prvSetupHardware(void) {
    SysCtlClockSet( SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ );
    SysCtlPeripheralEnable( SYSCTL_PERIPH_UART0 );
    xAsyncIOInitialize();       // setup asynchronous io data structures
    OSRAM128x64x4Enable(ulSSI_FREQUENCY);
    OSRAM128x64x4Init(ulSSI_FREQUENCY);
    OSRAM128x64x4Clear();
}

void task_game(void *pvParameters) {
    pong_start_game_loop(pong);
}

void task_user(void *pvParameters) {
    Player* player = get_user_player(pong);
    while (1)
    {
        char x = cAsyncIOGetc();
        PlayerDirection move;

        if (x == 'w') {
            move = UP;
        } else if(x == 's') {
            move = DOWN;
        } else {
            continue;
        }

        move_player(player, move);
    }
}