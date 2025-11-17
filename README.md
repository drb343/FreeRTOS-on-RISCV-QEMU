# FreeRTOS on RISC-V (QEMU virt Machine)

This project runs FreeRTOS on the RISC-V QEMU `virt` machine and includes an interrupt-driven UART driver for the NS16550A UART.  
The UART is implemented using memory-mapped I/O, PLIC interrupt routing, and FreeRTOS stream buffers for non-blocking TX/RX handling.

## Features
- RISC-V FreeRTOS port running on QEMU
- Full UART driver:
  - DLL/DLM baud-rate configuration
  - FIFO enable/clear
  - RX interrupt handling
  - PLIC interrupt setup
  - Integration with FreeRTOS stream buffers
- Basic test tasks (heartbeat, UART send/receive)
- GDB debugging support with QEMU (`-s -S`)

## Build
- make clean
- make

## Launch QEMU with UART0 mapped to terminal
- qemu-system-riscv32 -M virt -m 128M -nographic -bios none -kernel build/RTOSDemo.axf
