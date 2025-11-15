#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <stream_buffer.h>

#include <stdio.h>

#define UART_BASE 0x10000000UL
#define UART_RBR  (*(volatile uint8_t *)(UART_BASE + 0x00)) // Receive buffer
#define UART_THR  (*(volatile uint8_t *)(UART_BASE + 0x00)) // Transmit holding
#define UART_IER  (*(volatile uint8_t *)(UART_BASE + 0x01)) // Interrupt enable
#define UART_IIR  (*(volatile uint8_t *)(UART_BASE + 0x02)) // Interrupt ID
#define UART_FCR  (*(volatile uint8_t *)(UART_BASE + 0x02)) // FIFO control
#define UART_LCR  (*(volatile uint8_t *)(UART_BASE + 0x03)) // Line control
#define UART_MCR  (*(volatile uint8_t *)(UART_BASE + 0x04)) // Modem control
#define UART_LSR  (*(volatile uint8_t *)(UART_BASE + 0x05)) // Line status
#define UART_DLL  (*(volatile uint8_t *)(UART_BASE + 0x00)) // Baud rate low
#define UART_DLM  (*(volatile uint8_t *)(UART_BASE + 0x01)) // Baud rate high


void uart_init(void) {
    //Create FreeRTOS stream buffers
    TxStreamBuffer = xStreamBufferCreate(128, 1);
    RxStreamBuffer = xStreamBufferCreate(128, 1);

    //Disable interrupts
    UART_IER = 0x00;

    //Set the baud rate
    uart_set_baud(115200);

    //Enable FIFO, clear TX/RX FIFO
    UART_FCR =0x07;

    //Enable RX interrupt
    UART_IER = 0x01;

    //Register ISR with PLIC
    plic_set_priority(UART_IRQ, 1);
    plic_enable_irq(UART_IRQ);
    plic_register_handler(UART_IRQ, uart_isr);

    //Enable machine external interrupts
    set_csr(mie, MIE_MEIE);

    //Enable global interrupts
    set_csr(mstatus, MSTATUS_MIE);

}

void uart_set_baud(uint32_t baud) {
    uint32_t divisor = 1843200 / (16 * baud);

    UART_LCR |= 0x80;

    UART_DLL = divisor & 0xFF;
    UART_DLM = (divisor >> 8) & 0xFF;

    UART_LCR = 0x03;
}