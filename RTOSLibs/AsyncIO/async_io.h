#ifndef RTOSLIBS_ASYNC_UART_H
#define RTOSLIBS_ASYNC_UART_H

/*
 * This library contains non kernel-blocking functions to access UART I/O functionalities.
 * The following APIs should be called in the context of a FreeRTOS task.
 * */

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define ASYNC_UART_MAX_TIMEOUT portMAX_DELAY
#define ASYNC_UART_2s_TIMEOUT pdMS_TO_TICKS(2000)

void xAsyncIOInitialize();
char cAsyncIOGetc();
BaseType_t xAsyncIOPutc(char cc);
BaseType_t xAsyncIOGets(char *pcDst, unsigned long ulN);
BaseType_t xAsyncIOPuts(const char *pcSrc);

#endif //RTOSLIBS_ASYNC_UART_H
