#include "async_io.h"

/* Hardware includes */
#include "hw_types.h"
#include "hw_uart.h"
#include "hw_memmap.h"
#include "interrupt.h"
#include "uart.h"
/* ----------------  */

volatile struct aio_s {
    unsigned long ulUART;
    SemaphoreHandle_t xTxSemaphore;
    StaticSemaphore_t xTxSemaphoreBuffer;
    SemaphoreHandle_t xRxSemaphore;
    StaticSemaphore_t xRxSemaphoreBuffer;
    SemaphoreHandle_t xMutex;
    StaticSemaphore_t xMutexBuffer;
    char cCharBuffer;
    char pcStringBuffer[257];
    unsigned uBufferSize;
};

const char hex[] = "0123456789abcdef";
struct aio_s UART0 = { .ulUART = UART0_BASE, .xTxSemaphore = NULL, .xRxSemaphore = NULL, .xMutex = NULL, .cCharBuffer = 0, .pcStringBuffer = {0}, .uBufferSize = 256 };

void vUART_TxRxEndISR(void);

void xAsyncIOInitialize()
{
    UARTEnable( UART0.ulUART );
    UART0.xTxSemaphore = xSemaphoreCreateBinaryStatic(&UART0.xTxSemaphoreBuffer);
    configASSERT(UART0.xTxSemaphore);
    UART0.xRxSemaphore = xSemaphoreCreateBinaryStatic(&UART0.xRxSemaphoreBuffer);
    configASSERT(UART0.xRxSemaphore);
    UART0.xMutex = xSemaphoreCreateMutexStatic(&UART0.xMutexBuffer);
    configASSERT(UART0.xMutex);
    IntPrioritySet(BASE_USER_IRQn + UART0_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY);
    UARTIntRegister(UART0.ulUART, vUART_TxRxEndISR);
    UARTIntEnable(UART0.ulUART, UART_INT_RX | UART_INT_TX);
}

char cAsyncIOGetc()
{
    BaseType_t xResult = pdTRUE;
    xSemaphoreTake(UART0.xMutex, ASYNC_UART_MAX_TIMEOUT);
    xSemaphoreTake(UART0.xRxSemaphore, 0);
    xResult = xSemaphoreTake(UART0.xRxSemaphore, ASYNC_UART_MAX_TIMEOUT);
    xSemaphoreGive(UART0.xMutex);
    return (xResult == pdTRUE) ? UART0.cCharBuffer : 0;
}

BaseType_t xAsyncIOPutc(char cc)
{
    BaseType_t xResult = pdTRUE;
    xSemaphoreTake(UART0.xMutex, ASYNC_UART_MAX_TIMEOUT);
    xSemaphoreTake(UART0.xTxSemaphore, 0);
    UARTCharPutNonBlocking(UART0.ulUART, cc);
    xResult = xSemaphoreTake(UART0.xTxSemaphore, ASYNC_UART_2s_TIMEOUT);
    xSemaphoreGive(UART0.xMutex);
    return xResult;
}

BaseType_t xAsyncIOGets(char *pcDst, unsigned long ulN)
{
    if (ulN > UART0.uBufferSize || ulN == 0)
        return pdFALSE;

    BaseType_t xResult = pdTRUE;
    xSemaphoreTake(UART0.xMutex, ASYNC_UART_MAX_TIMEOUT);
    xSemaphoreTake(UART0.xRxSemaphore, 0);
    char *cursor = UART0.pcStringBuffer;
    do {
        xResult = xSemaphoreTake(UART0.xRxSemaphore, ASYNC_UART_MAX_TIMEOUT);
        if (xResult == pdFALSE)
            break;

        if (UART0.cCharBuffer >= 0x20 && UART0.cCharBuffer < 0x7f)
        {
            UARTCharPutNonBlocking(UART0.ulUART, UART0.cCharBuffer);
            *cursor = UART0.cCharBuffer;
            cursor++;
        }
    } while ((cursor - UART0.pcStringBuffer) < ulN && UART0.cCharBuffer != 0 && UART0.cCharBuffer != 0xa && UART0.cCharBuffer != 0xd);
    UARTCharPutNonBlocking(UART0.ulUART, '\n');
    if (xResult == pdTRUE)
    {
        for (char *p = UART0.pcStringBuffer; p < cursor; p++)
            *(pcDst + (p - UART0.pcStringBuffer)) = *p;
        *(pcDst + (cursor - UART0.pcStringBuffer)) = 0;
    }
    xSemaphoreGive(UART0.xMutex);
    return xResult;
}

BaseType_t xAsyncIOPuts(const char *pcSrc)
{
    BaseType_t xResult = pdTRUE;
    xSemaphoreTake(UART0.xMutex, ASYNC_UART_MAX_TIMEOUT);
    xSemaphoreTake(UART0.xTxSemaphore, 0);
    char *cursor = (char *)pcSrc;
    while (*cursor != 0 && xResult == pdTRUE)
    {
        UARTCharPutNonBlocking(UART0.ulUART, *cursor++);
        xResult = xSemaphoreTake(UART0.xTxSemaphore, ASYNC_UART_2s_TIMEOUT);
    }
    xSemaphoreGive(UART0.xMutex);
    return xResult;
}

/*
 *  ISR routines
 * */
void vUART_TxRxEndISR(void)
{
    BaseType_t xHigherPriorityTaskWokenTx = pdFALSE, xHigherPriorityTaskWokenRx = pdFALSE;
    unsigned long ulINTStatus = UARTIntStatus(UART0.ulUART, pdFALSE);

    if (ulINTStatus & UART_MIS_TXMIS)
    {
        /* TX Service */
        UARTIntClear(UART0.ulUART, UART_INT_TX);
        xSemaphoreGiveFromISR(UART0.xTxSemaphore, &xHigherPriorityTaskWokenTx);
    }

    if (ulINTStatus & UART_MIS_RXMIS)
    {
        /* RX Service */
        UARTIntClear(UART0.ulUART, UART_INT_RX);
        UART0.cCharBuffer = UARTCharGetNonBlocking(UART0.ulUART);
        xSemaphoreGiveFromISR(UART0.xRxSemaphore, &xHigherPriorityTaskWokenRx);
    }

    portYIELD_FROM_ISR(( xHigherPriorityTaskWokenTx | xHigherPriorityTaskWokenRx ));
}
