#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include <stdio.h>

#include "riscv-virt.h"
#include "ns16550.h"


void vHeartbeatTask(void *pvParameters){
    ( void ) pvParameters;
    char buf[40];
    int count = 0;

    for(;;){
        sprintf(buf, "[HEARTBEAT] System OK\n");
        vSendString(buf);

        count++;

        vTaskDelay(pdMS_TO_TICKS(1000));

        if (count > 20){
            sprintf(buf, "[HEARTBEAT] Ending\n");
            vSendString(buf);
            vTaskDelete(NULL);
        }

    }

}