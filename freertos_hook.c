#include <FreeRTOS.h>
#include "plic.h"
#include "uart.h"

void freertos_risc_v_application_interrupt_handler(uint32_t mcause)
{
    if ( (mcause & 0x80000000UL) && ((mcause & 0xFF) == 11) )
    {
        int irq = plic_claim();

        if (irq == UART_IRQ)
            uart_isr();

        plic_complete(irq);
    }
}
