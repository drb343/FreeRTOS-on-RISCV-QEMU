#include <FreeRTOS.h>
#include <task.h>
#include "plic.h"
#include "uart.h"
#include <stdint.h>
#include <stdio.h>

#define PLIC_BASE       0x0c000000
#define PLIC_PRIORITY   (PLIC_BASE + 0x000000)
#define PLIC_PENDING    (PLIC_BASE + 0x001000)
#define PLIC_MENABLE    (PLIC_BASE + 0x002000)
#define PLIC_MPRIORITY  (PLIC_BASE + 0x200000)
#define PLIC_MCLAIM     (PLIC_BASE + 0x200004)

void plic_init(void){
    //set priority to 1
    *(volatile uint32_t *)(PLIC_PRIORITY + UART_IRQ * 4) = 1;
}

void plicinithart(void)
{
    //Enable UART interrupt for hart 0 (enable irq)
    volatile uint32_t *enable = (volatile uint32_t *)(PLIC_MENABLE);
    enable[UART_IRQ / 32] |= (1 << (UART_IRQ % 32));

    //Allow all priorities > 0
    *(volatile uint32_t *)(PLIC_MPRIORITY) = 0;
}

int plic_claim(void){
    return *(volatile uint32_t *)(PLIC_MCLAIM);
}

void plic_complete(int irq)
{
    *(volatile uint32_t *)(PLIC_MCLAIM) = irq;
}
