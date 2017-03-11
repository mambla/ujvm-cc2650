/*
 * Copyright (c) 2015-2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== empty.c ========
 */
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/I2C.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>

#include "uj/uc_main.h"
/* Board Header files */
#include "Board.h"
#include "ExtFlash.h"

/*
 * Application LED pin configuration table:
 *   - All LEDs board LEDs are off.
 */
PIN_Config ledPinTable[] = {
    Board_LED0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    Board_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

PIN_Config buttonPinTable[] = {
    Board_BUTTON0  | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,
    Board_BUTTON1  | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,
    PIN_TERMINATE
};

static PIN_State pin_state;
PIN_Handle pin_handle;
static PIN_State button_pin_state;
PIN_Handle button_pin_handle;
UART_Handle uart_handle = 0;

void write_status(char status)
{
    char str[2] = {0};
    str[0] = status;
    UART_write(uart_handle, str, 1);
}

typedef struct {
    uint32_t file_size;
    uint32_t chunk_size;
} SizeInfo;


enum PROGRAM_STATUS
{
    WRITE = 'w',
    PRINT = 'p',
    READY = 'r',
    CHUNK = 'c',
    FINISHED = 'f',
    RUN_JAVA = 'r',
};

#define CHUNK_SIZE 1024

static uint8_t CHUNK_BUF[CHUNK_SIZE] = {0};

void read_all_bytes(uint8_t* buf, uint32_t size)
{
    uint32_t bytes_read = 0;

    while (bytes_read < size)
    {
        bytes_read += UART_read(uart_handle, buf + bytes_read, size - bytes_read);
    }
}

void sleep(uint32_t ms)
{
    Task_sleep(ms);
}

void write_flash()
{
    SizeInfo size_info = {0};
    System_printf("Received write status\n"); System_flush();

    sleep(500);
    write_status(WRITE);

    System_printf("Sent start status\n"); System_flush();

    read_all_bytes((uint8_t*)&size_info, sizeof(size_info));

    if (CHUNK_SIZE != size_info.chunk_size)
    {
        System_printf("Chunk size sent (%u) doesn't equal predefined chunk size (%u)\n", size_info.chunk_size, CHUNK_SIZE); System_flush();
        return;
    }

    System_printf("Received file size (%u) and chunk size (%u)\n", size_info.file_size, size_info.chunk_size); System_flush();

    if (!ExtFlash_erase(0, size_info.file_size))
    {
        System_printf("Failed erasing flash \n"); System_flush();
        return;
    }

    sleep(500);
    write_status(READY);

    uint32_t current = 0;

    for (; current < size_info.file_size; current += size_info.chunk_size)
    {
        uint32_t left = size_info.file_size - current;
        left = left > CHUNK_SIZE ? CHUNK_SIZE : left;

        read_all_bytes(CHUNK_BUF, left);

        System_printf("Received chunk at offset %u, bytes: %u \n", current, left); System_flush();

        if (!ExtFlash_write(current, left, CHUNK_BUF))
        {
            System_printf("Failed to write %u bytes at offset %u \n", left, current); System_flush();
            return;
        }

        sleep(500);
        write_status(CHUNK);

        System_printf("Acked chunk at offset %u\n", current); System_flush();
    }

    sleep(500);
    write_status(FINISHED);
    System_printf("Done!\n"); System_flush();
}

void print_flash()
{
    uint32_t size = 0;
    System_printf("Received print status\n"); System_flush();

    sleep(500);
    write_status(PRINT);

    System_printf("Sent print status\n"); System_flush();

    read_all_bytes((uint8_t*)&size, sizeof(size));

    System_printf("Received print size %u \n", size); System_flush();

    char print_buf[16 + 1] = {0};

    ExtFlash_read(0, sizeof(print_buf) - 1, (uint8_t*) print_buf);

    System_printf("First 16 bytes: %s \n", print_buf);

    ExtFlash_read(size - sizeof(print_buf) + 1, sizeof(print_buf) - 1, (uint8_t*) print_buf);

    System_printf("Last 16 bytes: %s \n", print_buf);
}

Void mainFxn(UArg arg0, UArg arg1)
{
    if (uart_handle == 0)
    {
        UART_Params uartParams;
        UART_Params_init(&uartParams);
        uartParams.baudRate = 115200;
        uart_handle = UART_open(Board_UART, &uartParams);

        if (!ExtFlash_open())
        {
            System_printf("Failed initializing flash \n"); System_flush();
            return;
        }
    }

    uc_main();
}

Void flashFxn(UArg arg0, UArg arg1)
{
    while (true)
    {
        System_printf("Waiting for commands... \n"); System_flush();

        char command;
        UART_read(uart_handle, &command, 1);

        if (!ExtFlash_open())
        {
            System_printf("Failed initializing flash \n"); System_flush();
            return;
        }

        switch(command)
        {
        case WRITE:
            write_flash();
            break;
        case PRINT:
            print_flash();
            break;
        case RUN_JAVA:
            mainFxn(arg0, arg1);
        default:
            System_printf("Received incorrect command %c\n", command); System_flush();
        }

        ExtFlash_close();
    }
}

Task_Struct task0Struct;


void buttonCallbackFxn(PIN_Handle handle, PIN_Id pinId) {
    if (!PIN_getInputValue(pinId)) {
        onButtonPress(pinId);
    }
}

/*
 * - print buffer instead of one character at a time
 * - implement uart driver (configurable with traces)
 * - implement spi / eeprom driver
 * - add deep sleep for battery conservation
 * - refactoring
 * - write html
 */
int main(void)
{
    Task_Params taskParams = {0};

    /* Call board init functions */
    Board_initGeneral();
    //Board_initI2C();
    Board_initSPI();
    Board_initUART();
    // Board_initWatchdog();

    pin_handle = PIN_open(&pin_state, ledPinTable);
    if(!pin_handle) {
        System_abort("Error initializing board LED pins\n");
    }

    button_pin_handle = PIN_open(&button_pin_state, buttonPinTable);
   if(!button_pin_handle) {
       System_abort("Error initializing button pins\n");
   }

   /* Setup callback for button pins */
   if (PIN_registerIntCb(button_pin_handle, &buttonCallbackFxn) != 0) {
       System_abort("Error registering button callback function");
   }
   int i = 0;
    UART_Params uartParams;
    UART_Params_init(&uartParams);
    uartParams.baudRate = 115200;
    uart_handle = UART_open(Board_UART, &uartParams);

    System_printf("Yo Yo Yo\n"); System_flush();
    		   /* Construct heartBeat Task  thread */

	Task_Params_init(&taskParams);
	Task_construct(&task0Struct, (Task_FuncPtr)flashFxn, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();

    return (0);
}
