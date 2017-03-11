
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/Memory.h>
/* TI-RTOS Header files */
// #include <ti/drivers/I2C.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>

/* Board Header files */
#include "Board.h"
#include "ExtFlash.h"

#define TASKSTACKSIZE   512

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];
UART_Handle uart_handle;

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
	FINISHED = 'f'
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
		default:
			System_printf("Received incorrect command %c\n", command); System_flush();
		}

		ExtFlash_close();
	}
}

/*
 *  ======== main ========
 */
int main(void)
{
    Task_Params taskParams;

    /* Call board init functions */
    Board_initGeneral();
    // Board_initI2C();
    Board_initSPI();
    Board_initUART();
    // Board_initWatchdog();

    /* Construct heartBeat Task  thread */
    Task_Params_init(&taskParams);
    taskParams.arg0 = 1000;
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)flashFxn, &taskParams, NULL);


    System_printf("Yo Yo Yo\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    UART_Params uartParams;
    UART_Params_init(&uartParams);
    uartParams.baudRate = 115200;
    uart_handle = UART_open(Board_UART, &uartParams);
    /* Start BIOS */
    BIOS_start();

    return (0);
}
