#ifndef _COMMON_H_
#define _COMMON_H_

#include <xdc/std.h>

typedef unsigned char Boolean;
typedef float UjFloat;

#ifdef __HITECH__
	typedef unsigned short long UInt24;
#else
	typedef unsigned long UInt24;
#endif

#define true	1
#define false	0
#ifndef NULL
	#define NULL	0
#endif
#ifndef NAN
	#define NAN	(0.0f/0.0f)
#endif

void err(const char* str);

#include <stdio.h>
#define TL(...)		//ujLog(__VA_ARGS__)//fprintf(stderr, "**UL** " __VA_ARGS__)

#define DEBUG		1

#define _UNUSED_	__attribute__((unused))
#define _INLINE_	__attribute__((always_inline)) inline 


#define GET_FAR_ADDRESS(var)                          \
({                                                    \
    uint_farptr_t tmp;                                \
                                                      \
    __asm__ __volatile__(                             \
                                                      \
            "ldi    %A0, lo8(%1)"           "\n\t"    \
            "ldi    %B0, hi8(%1)"           "\n\t"    \
            "ldi    %C0, hh8(%1)"           "\n\t"    \
            "clr    %D0"                    "\n\t"    \
        :   "=d" (tmp)                                \
        :   "p"  (&(var))                             \
    );                                                \
    tmp;                                              \
}) 


#if defined(CPU_ATMEGA64)

	#define GET_ADDRESS(x)		((UInt16)(&(x)))
	#define pgm_read		pgm_read_byte
	#define pgm_read_str		pgm_read
	#define _PROGMEM_		PROGMEM
	#define HEAP_ALIGN		1
	#define INCLUDE_ATMEGA_FILES
	#define SDLOADER
	#define GPIO_PORTS		4
	#define GPIO_PINS_PER_PORT	8
	#define EEPROM_SIZE		E2END
	#define _HEAP_ATTRS_		__attribute__((section (".noinit")))
	
#elif defined(CPU_ATMEGA128)

	#define GET_ADDRESS		GET_FAR_ADDRESS
	#define pgm_read		pgm_read_byte_far
	#define pgm_read_str		pgm_read
	#define _PROGMEM_		PROGMEM
	#define HEAP_ALIGN		1
	#define INCLUDE_ATMEGA_FILES
	#define GPIO_PORTS		4
	#define GPIO_PINS_PER_PORT	8
	#define EEPROM_SIZE		E2END
	#define _HEAP_ATTRS_		__attribute__((section (".noinit")))
	
#elif defined(CPU_DSPIC) || defined (CPU_PIC24)

	#define GET_ADDRESS(x)		((UInt32)(&(x)))
	UInt8 pgm_read(UInt32 addr);
	#define pgm_read_str(addr)	(*((UInt8*)(addr)))
	#define _PROGMEM_
	#define HEAP_ALIGN		2
	#define INCLUDE_PIC_FILES
	#define SDLOADER
	#define GPIO_PORTS		2
	#define GPIO_PINS_PER_PORT	16
	#define EEPROM_SIZE		1536
	#define _HEAP_ATTRS_		__attribute__((far))
	#define EEPROM_EMUL_ADDR	0x800000UL	//we use 4 pages
	void pgm_write(UInt32 addr, UInt8 val)

#elif defined(CPU_ARM_CORTEX_M3)
	UInt8 pgm_read(UInt32 addr);
	void pgm_write(UInt32 addr, UInt8 val);

	#define GET_ADDRESS(x)		((UInt32)(&(x)))
	#define _PROGMEM_
	#define HEAP_ALIGN		1
	#define _HEAP_ATTRS_

	
#elif defined (CPU_X86)
	
	#define HEAP_ALIGN	4
	#define _HEAP_ATTRS_
	
#else

	#error "NO CPU_* macro defined"
	
#endif

#if defined(BOARD_MEGA128)

	#define F_CPU			20000000UL
	#define HEAP_SZ			14336
	#define FLASHROM_PAGE		256UL		//in bytes
	#define FLASHROM_SIZE		(128UL*1024UL)
	#define UJ_LOG
	
#elif defined(BOARD_DIANA)
	
	#define F_CPU			20000000UL
	#define HEAP_SZ			2040
	#define HAVE_I2C
	#define HAVE_CHAR_LCD
	#define FLASHROM_PAGE		256UL		//in bytes
	#define FLASHROM_SIZE		(64UL*1024UL)
	
#elif defined(BOARD_HELSINKI)

	#ifdef CPU_DSPIC
		#define F_CPU			106865000UL
		#define C_PLLDIV		201	//106.865 MHzMHz speed with 7.37 osc (80017143 Hz)
		#define C_PLLPRE		5
		#define C_PLLPOST		0
		#define HEAP_SZ			14336

		#define UJ_FTR_SUPPORT_DOUBLE
		#define UJ_FTR_SUPPORT_LONG
		#define UJ_FTR_SUPPORT_FLOAT
		#define UJ_LOG

	#endif
	#ifdef CPU_PIC24
		#define F_CPU			32000000UL
		#define HEAP_SZ			6336
	#endif
	#define HAVE_I2C
	#define HAVE_GFX_LCD
	#define HAVE_CHAR_LCD
	#define FLASHROM_PAGE		1024	//actually later they become 512, but who cares :)
	
#elif defined (BOARD_PC)

	#define HEAP_SZ			3172
	
#elif defined (BOARD_CC2650_LAUNCHPAD)
	#define F_CPU				48000000UL
	#define HEAP_SZ				(12 * 1024)
	#define FLASH_SIZE (64UL*1024UL)
    #define FLASHROM_SIZE       (1024UL*1024UL - FLASH_SIZE)
	#define GPIO_PORTS 1
	#define GPIO_PINS_PER_PORT 20
#else

	#error "NO BOARD_* macro defined"
	
#endif


#ifdef INCLUDE_ATMEGA_FILES

	#define BAUD 38400UL
	#include <avr/io.h>
	#include <avr/pgmspace.h>
	#include <avr/eeprom.h>
	#include <avr/interrupt.h>
	#include <util/setbaud.h>
	#include <util/delay.h>
	#include <avr/boot.h>
#endif

#ifdef INCLUDE_PIC_FILES

	#ifdef CPU_DSPIC
		#include <p33FJ128GP802.h>
	#endif

	#ifdef CPU_PIC24
		#include <p24FJ32GA002.h>
	#endif

#endif

#ifdef INCLUDE_TI_RTOS_FILES
    #include "ExtFlash.h"
    #include "Board.h"
    #include <ti/drivers/UART.h>
    #include <ti/drivers/PIN.h>
    #include <ti/sysbios/knl/Clock.h>
#endif

#ifdef UJ_LOG

	void ujLog(const char* fmtStr, ...);

#else

	#define ujLog(...)
#endif 

//common board api
	void board_init(void);
	UInt32 getClock(void);
	UInt32 getClkTicksPerSec(void);
	void fail_msg(UInt8 var);

//i2c api
	void i2cStart(void);
	Boolean i2cSend(UInt8 v);
	UInt8 i2cRecv(Boolean ack);
	void i2cStop(void);

//SPI api
	void spiClockSpeed(Boolean fast);
	UInt8 spiByte(UInt8 v);
	void spiSingleClock(void);

//gpio api
	extern PIN_Handle pin_handle;
	void gpioSetOut(UInt8 port, UInt8 pin, Boolean isOutput);
	void gpioSetVal(UInt8 port, UInt8 pin, Boolean isHi);
	Boolean gpioGetVal(UInt8 port, UInt8 pin);

// button api
	void clearLastButtonPress(UInt32 buttonToQuery);
	UInt8 getLastButtonPress(UInt32 buttonToQuery);

//Flash api
	UInt8 flashRead(UInt16 addr);
	void flashWrite(UInt16 addr, UInt8 val);
	void flashErase(UInt16 addr, UInt16 size);

// Sleep api
	void deepSleep(UInt32 ms);

//character LCD api
	void clcdClear(void);
	void clcdGotoXY(UInt8 x, UInt8 y);
	void clcdChar(char c);
	void clcdRawWrite(UInt8 val);


//GLCD api
	void glcdClear(void);
	void glcdSetPixels(UInt8 rowOver8, UInt8 c, UInt8 vals);				//8 pixels, vertically. LSB = top
	void glcdSetManyPixels(UInt8 rowOver8, UInt8 c, const UInt8* vals, UInt16 nCols);	//8 pixels, vertically. LSB = top

//PWM api
	#define PWM_WHITE_LED	0
	#define PWM_LCD_BL	1
	Boolean pwmSet(UInt8 which, UInt8 bri);

//flashrom API

	#define FLASH_KEY	0x4648
	UInt32 getFlashFreeArea(UInt32* sz);	//return addr
	void flashromWrite(UInt32 addr, const UInt8* data, UInt16 key);

// uart api
	extern UART_Handle uart_handle;
	void uart_putchar(char c);
#endif
