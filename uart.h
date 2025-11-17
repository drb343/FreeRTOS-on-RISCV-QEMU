#ifndef UART_H
#define UART_H

#include <stdint.h>
#include "stream_buffer.h"

#define UART_IRQ 10   // QEMU virt UART interrupt line

#define UART_BASE 0x10000000UL

//UART Registers for 16550 
#define UART_RBR  (*(volatile uint8_t *)(UART_BASE + 0x00)) // Receive Buffer
#define UART_THR  (*(volatile uint8_t *)(UART_BASE + 0x00)) // Transmit Hold
#define UART_IER  (*(volatile uint8_t *)(UART_BASE + 0x01)) // Interrupt Enable
#define UART_IIR  (*(volatile uint8_t *)(UART_BASE + 0x02)) // Interrupt ID
#define UART_FCR  (*(volatile uint8_t *)(UART_BASE + 0x02)) // FIFO Control
#define UART_LCR  (*(volatile uint8_t *)(UART_BASE + 0x03)) // Line Control
#define UART_MCR  (*(volatile uint8_t *)(UART_BASE + 0x04))
#define UART_LSR  (*(volatile uint8_t *)(UART_BASE + 0x05)) // Line Status
#define UART_MSR  (*(volatile uint8_t *)(UART_BASE + 0x06))
#define UART_SCR  (*(volatile uint8_t *)(UART_BASE + 0x07))

//DLL/DLM share same addresses as RBR/IER when DLAB=1 
#define UART_DLL  (*(volatile uint8_t *)(UART_BASE + 0x00))
#define UART_DLM  (*(volatile uint8_t *)(UART_BASE + 0x01))

//LCR bits
#define UART_LCR_DLAB     (1 << 7)     //Divisor Latch Access Bit

//LSR bits
#define UART_LSR_DR        (1 << 0)  //Data Ready
#define UART_LSR_OE        (1 << 1)  //overrun error
#define UART_LSR_PE        (1 << 2)  //parity error
#define UART_LSR_FE        (1 << 3)
#define UART_LSR_BI        (1 << 4)
#define UART_LSR_THRE      (1 << 5)  //THR Empty
#define UART_LSR_TEMT      (1 << 6)  //Transmitter Empty
#define UART_LSR_FIFOERR   (1 << 7)

//IER bits
#define UART_IER_RDA_INT   (1 << 0)  //Enable Received Data Available
#define UART_IER_THRE_INT  (1 << 1)  //Enable THR empty interrupt

//IIR interrupt IDs
#define UART_IIR_INT_PENDING    0x01
#define UART_IIR_THRE           0x02
#define UART_IIR_RDA            0x04
#define UART_IIR_LINE_STATUS    0x06
#define UART_IIR_TIMEOUT        0x0C

//FCR bits
#define UART_FCR_ENABLE_FIFO   (1 << 0)
#define UART_FCR_CLEAR_RX      (1 << 1)
#define UART_FCR_CLEAR_TX      (1 << 2)

extern StreamBufferHandle_t RxStreamBuffer;
extern StreamBufferHandle_t TxStreamBuffer;


void uart_init(void);
void uart_set_baud(uint32_t baud);
void uart_isr(void);
void uart_putc(char c);
void uart_puts(const char *s);

#endif
