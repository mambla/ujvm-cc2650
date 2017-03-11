#include "common.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

//static int uart_getchar(_UNUSED_ FILE *stream);

UInt32 getClock(void){
    return Clock_getTicks();
}

UInt32 getClkTicksPerSec(void){
	return 100 * 1000; // 10 us tick
}

void fail_msg(UInt8 var){
    ujLog("Fail %d", var);
}

void gpioSetOut(UInt8 port, UInt8 pin, Boolean isOutput){
    PIN_setOutputEnable(pin_handle, pin, isOutput);
}

void gpioSetVal(UInt8 port, UInt8 pin, Boolean isHi){
    PIN_setOutputValue(pin_handle, pin, isHi);
}

Boolean gpioGetVal(UInt8 port, UInt8 pin){
    return PIN_getOutputValue(pin);
}

UInt8 flashRead(UInt16 addr){
    return pgm_read(FLASHROM_SIZE + addr);
}

void flashWrite(UInt16 addr, UInt8 val){
    pgm_write(FLASHROM_SIZE + addr, val);
}

void flashErase(UInt16 addr, UInt16 size){
    while (!ExtFlash_erase(FLASHROM_SIZE + addr, size))
    {
        ujLog("Ahhhhh!!");
    }
}

Boolean pwmSet(UInt8 which, UInt8 bri){

//	switch(which){
//
//		case PWM_WHITE_LED:
//
//			OCR0B = bri;
//			break;
//
//		case PWM_LCD_BL:
//
//			OCR0A = bri;
//			break;
//
//		default:
//
//			return false;
//	}
	return true;
}
//
//__attribute__ ((section (".lastpage"))) void flashromWrite(UInt32 addr, const UInt8* buf, UInt16 key){
//
//	UInt16 i;
//	UInt8 sreg;
//
//	if(key != FLASH_KEY) return;
//
//	sreg = SREG;
//	cli();
//
//	while(EECR & 2);	//wait for eeprom to finish writing
//
//	#if (FLASHROM_SIZE > 64UL*1024UL)
//		RAMPZ = addr >> 16UL;
//	#endif
//
//	asm volatile(					//do the for erase
//			"mov r30, %0\n\t"
//			"mov r31, %1\n\t"
//			"sts 0x57, %2\n\t"
//			"spm\n\t"
//			::
//				"r"((UInt8)(addr)),
//				"r"((UInt8)(addr >> 8)),
//				"r"(3)
//			:"r30","r31"
//		);
//
//	while(SPMCSR & 2);	//wait for erase
//
//	for(i = 0; i < BYTES_PER_PAGE; i+= 2){
//
//		#if (FLASHROM_SIZE > 64UL*1024UL)
//			RAMPZ = (addr + i) >> 16UL;
//		#endif
//
//		asm volatile(					//fill buffer
//			"mov r0, %3\n\t"
//			"mov r1, %4\n\t"
//			"mov r30, %0\n\t"
//			"mov r31, %1\n\t"
//			"sts 0x57, %2\n\t"
//			"spm\n\t"
//			"clr r1\n\t"
//			::
//				"r"((UInt8)(addr + i)),
//				"r"((UInt8)((addr + i) >> 8)),
//				"r"(1),
//				"r"(buf[i + 0]),
//				"r"(buf[i + 1])
//			:"r0","r1","r30","r31"
//		);
//
//		while(SPMCSR & 1);	//wait for buffer write
//	}
//
//	#if (FLASHROM_SIZE > 64UL*1024UL)
//		RAMPZ = addr >> 16UL;
//	#endif
//
//	asm volatile(						//do the write
//			"mov r30, %0\n\t"
//			"mov r31, %1\n\t"
//			"sts 0x57, %2\n\t"
//			"spm"
//			::
//				"r"((UInt8)(addr)),
//				"r"((UInt8)(addr >> 8)),
//				"r"(5)
//			:"r30","r31"
//		);
//
//	while(SPMCSR & 4);	//wait for write
//
//
//	asm volatile(						//re-enable reads there
//			"mov r30, %0\n\t"
//			"mov r31, %1\n\t"
//			"sts 0x57, %2\n\t"
//			"spm"
//			::
//				"r"((UInt8)(addr)),
//				"r"((UInt8)(addr >> 8)),
//				"r"(17)
//			:"r30","r31"
//		);
//
//
//	while(SPMCSR & 1);
//	SREG = sreg;
//}
//
//UInt32 getFlashFreeArea(UInt32* szP){
//
//	UInt32 t, addr, size;
//
//	size = FLASHROM_SIZE;
//	size -= BYTES_PER_PAGE;		//last page is reserved for bootloader-like code that does our writes
//
//	t = GET_FAR_ADDRESS(__data_load_end);
//	addr = GET_FAR_ADDRESS(_etext);
//
//	if(addr < t) addr = t;
//	size -= addr;
//
//	//round the area to page size
//
//	if((t = addr & (FLASHROM_PAGE - 1))){
//
//		t = FLASHROM_PAGE - t;
//
//		addr += t;
//		size -= t;
//	}
//
//	size -= (size & (FLASHROM_PAGE - 1));
//
//	*szP = size;
//
//	ujLog("FLASH area: 0x%04X+0x%04X\n", (UInt16)addr, (UInt16)size);
//
//	return addr;
//}
//
//void spiClockSpeed(Boolean fast){
//
//	gSlow = !fast;
//}
//
//static _INLINE_ void pinHi(UInt8 bit){
//
////	SD_REG(DDR) &=~ bit;	//make input
////	SD_REG(PORT) |= bit;	//pullup
//	SD_REG(DDR) |= bit;	//make input
//	SD_REG(PORT) |= bit;	//pullup
//}
//
//static _INLINE_ void pinLo(UInt8 bit){
//
//	SD_REG(PORT) &=~ bit;	//no pullup
//	SD_REG(DDR) |= bit;	//make output
//}
//
//UInt8 spiByte(UInt8 v){
//
//	UInt8 i;
//
//	for(i = 0; i < 8; i++){
//
//		if(v & 0x80) pinHi(SD_PIN_MOSI);
//		else pinLo(SD_PIN_MOSI);
//
//		if(gSlow) _delay_us(0.5);
//
//		pinHi(SD_PIN_CLK);
//
//		_delay_us(0.8);
//		if(gSlow) _delay_us(1.6);
//
//		v <<= 1;
//		if(SD_REG(PIN) & SD_PIN_MISO) v++;
//		pinLo(SD_PIN_CLK);
//
//		if(gSlow) _delay_us(0.2);
//	}
//
//	pinLo(SD_PIN_MOSI);
//
//	return v;
//}
//
//void spiSingleClock(void){
//
//	pinHi(SD_PIN_MOSI);
//	_delay_us(1);
//	pinHi(SD_PIN_CLK);
//	_delay_us(1);
//	pinLo(SD_PIN_CLK);
//	_delay_us(1);
//}
//
//void i2cStart(void){
//
//	TWCR = 0xA4;
//	while(!(TWCR & 0x80));
//}
//
//Boolean i2cSend(UInt8 v){
//
//	TWDR = v;
//	TWCR = 0x84;
//	while(!(TWCR & 0x80));
//
//	return (TWSR & 0xF8) == 0x18;
//}
//
//UInt8 i2cRecv(Boolean ack){
//
//	TWCR = ack ? 0xC4 : 0x84;
//	while(!(TWCR & 0x80));
//
//	return TWDR;
//}
//
//void i2cStop(void){
//
//	TWCR = 0x94;
//	while(!(TWCR & 0x80));
//}

void uart_putchar(char c)
{
    UART_write(uart_handle, &c, 1);
}

void pgm_write(UInt32 addr, UInt8 val)
{
    while (!ExtFlash_write(addr, sizeof(val), &val))
    {
        ujLog("Ahhhhh!!");
    }
}


UInt8 pgm_read(UInt32 addr){
    UInt8 out;
    while (!ExtFlash_read(addr, sizeof(out), &out))
    {
        ujLog("Ahhhhh!!");
    }
    return out;
    //return uj_code[addr];

}

enum ButtonPressed {
    Both = 0,
    Button1 = 1,
    Button2 = 2
};

static UInt8 button1_pressed;
static UInt8 button2_pressed;

void clearLastButtonPress(UInt32 buttonToClear)
{
    if (buttonToClear == Button1 || buttonToClear == Both)
    {
        button1_pressed = 0;
    }

    if (buttonToClear == Button2 || buttonToClear == Both)
    {
        button2_pressed = 0;
    }
}

UInt8 getLastButtonPress(UInt32 buttonToQuery)
{
    switch (buttonToQuery)
    {
    case Button1:
        return button1_pressed;
    case Button2:
        return button2_pressed;
    case Both:
        return button1_pressed ? Button1 : button2_pressed ? Button2 : 0;
    }

    return 0;
}

void onButtonPress(PIN_Id pinId)
{
    if (pinId == Board_BUTTON0)
    {
        button1_pressed = 1;
    }
    else if (pinId == Board_BUTTON1)
    {
        button2_pressed = 1;
    }
}
