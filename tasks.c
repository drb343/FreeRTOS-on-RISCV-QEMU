#include <FreeRTOS.h>
#include "FreeRTOSConfig.h"
#include <task.h>
#include <stdio.h>
#include <string.h>
#include "riscv-virt.h"
#include "ns16550.h"
#include "uart.h"
#include "stream_buffer.h"

//Heartbeat Task to make sure RTOS is working
void vHeartbeatTask(void *pvParameters){
    ( void ) pvParameters;
    char buf[40];
    int count = 0;
    
    //Every 1 second, send a string that indicates a heartbeat to buf
    for(;;){
        sprintf(buf, "[HEARTBEAT] System OK\n");
        vSendString(buf);

        count++;

        vTaskDelay(pdMS_TO_TICKS(1000));

        //If number of iterations is greater than 20, then stop the heartbeat and delete the task
        if (count > 20){
            sprintf(buf, "[HEARTBEAT] Ending\n");
            vSendString(buf);
            vTaskDelete(NULL);
        }

    }

}

//Testing the RX task UART task
void vUARTTask(void *pvParameters){
    uart_puts("UART Test Task Running...\r\n");

    uint8_t ch;

    for(;;){
        if (xStreamBufferReceive(RxStreamBuffer, &ch, 1, portMAX_DELAY) == 1){

            //if you receive data in the rx stream buffer, echo it back
            uart_puts("[RX] ");
            uart_putc(ch);
            uart_putc('\n');
        }
    }

}

//UART TX transmitter
void uart_txt_test(void *pvParameters){
    const char* msg = "Testing UART TX Buffer ..\n";

    //Every second return  msg
    for(;;){
        xStreamBufferSend(TxStreamBuffer, msg, strlen(msg), portMAX_DELAY);
        UART_IER |= UART_IIR_THRE;

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
