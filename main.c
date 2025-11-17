#include <FreeRTOS.h>
#include <task.h>

#include <stdio.h>

#include "tasks.h"
#include "uart.h"

extern void freertos_risc_v_trap_handler(void);

/* ---------- Main Entry ---------- */
int main(void)
{
    /* Set trap handler (required for scheduler & timer interrupts) */
    __asm__ volatile ( "csrw mtvec, %0" : : "r" ( freertos_risc_v_trap_handler ) );

    //Initialize UART
    uart_init();


    /* Create user task */
    xTaskCreate(vHeartbeatTask, "HeartbeatTask", 1000, NULL, 1, NULL);
    xTaskCreate(uart_txt_test, "UARTTask", 1000, NULL, 1, NULL);

    /* Start the scheduler */
    vTaskStartScheduler();

    for (;;); /* Should never reach here */
}


void vApplicationMallocFailedHook(void)
{
    taskDISABLE_INTERRUPTS();
    for (;;);
}

void vApplicationIdleHook(void)
{
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask,
                                   char *pcTaskName)
{
    (void)pcTaskName;
    (void)pxTask;
    taskDISABLE_INTERRUPTS();
    for (;;);
}

void vApplicationTickHook(void)
{
}

void vAssertCalled(void)
{
    volatile uint32_t ul = 0;
    taskDISABLE_INTERRUPTS();
    while (ul != 1)
    {
        __asm volatile ("nop");
    }
}
