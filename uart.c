#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include <stdio.h>

#include "uart.h"
#include "plic.h"
#include "riscv-virt.h"
#include "stream_buffer.h"
#include <stdint.h>

StreamBufferHandle_t TxStreamBuffer;
StreamBufferHandle_t RxStreamBuffer;

static inline void handle_rx(void);
static inline void handle_tx(void);

//Initialize UART
void uart_init(void) {
    //Create FreeRTOS stream buffers
    TxStreamBuffer = xStreamBufferCreate(128, 1);
    RxStreamBuffer = xStreamBufferCreate(128, 1);

    //Disable interrupts
    UART_IER = 0x00;

    //Set the baud rate
    uart_set_baud(115200); //baud rate to 115200

    //Enable FIFO, clear TX/RX FIFO
    UART_FCR =0x07;

    //Enable RX interrupt
    UART_IER = 0x01;

    //Register ISR with PLIC
    plic_init();
    plicinithart();

}

//ISR function for UART
void uart_isr(void){
    //Identify the reason for UART interrupt ID lower byte
    uint32_t reason = UART_IIR & 0x0F;

    //Look up reason
    switch(reason){
        case 0x04: //Rx available
        case 0x0C: //Rx FIFO
            handle_rx();
            break;

        case 0x02: //TX interrupt
            handle_tx();
            break;

        default:
            break;

    }

}

//Set baud rate to 115200
void uart_set_baud(uint32_t baud) {
    uint32_t divisor = 1843200 / (16 * baud);

    UART_LCR |= 0x80;

    UART_DLL = divisor & 0xFF;
    UART_DLM = (divisor >> 8) & 0xFF;

    UART_LCR = 0x03;
}

//Handle the RX interrupt
static inline void handle_rx(void){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    //while Line status register indicates that data is ready
    while (UART_LSR & UART_LSR_DR){
        uint8_t ch = UART_RBR; //get the first byte from the receive buffer

        //send to the Receiver buffer from ISR
        xStreamBufferSendFromISR(RxStreamBuffer, &ch, 1, &xHigherPriorityTaskWoken);

    }

    //Yield once all bytes are processed
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

//Handle the TX interrupt
static inline void handle_tx(void){

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t ch;
    size_t xReceivedBytes;

    //if transmitter is ready for another byte
    if(UART_LSR & UART_LSR_THRE){
        //Receive the bytes from the buffer
        xReceivedBytes = xStreamBufferReceiveFromISR(TxStreamBuffer, &ch, 1, &xHigherPriorityTaskWoken);

        //if bytes, received, write to UART transmit hold
        if (xReceivedBytes > 0){
            UART_THR = ch;
        } else {
            //no more data, thus disable off the TX interrupt
            UART_IER &= ~UART_IER_THRE_INT;
        }
    }

    //Yield once all bytes are processed
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);


}

void uart_putc(char c) {
    // Wait for THR empty (bit 5 of LSR)
    while ((UART_LSR & (1 << 5)) == 0) {
        // spin
    }

    UART_THR = c;
}

void uart_puts(const char *s) {
    while (*s)
        uart_putc(*s++);
}

