#include "common.h"

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

void deepSleep(UInt32 ms)
{
    Task_sleep(ms * (getClkTicksPerSec() / 1000));
}

Boolean pwmSet(UInt8 which, UInt8 bri){

	return false;
}

//
//void spiClockSpeed(Boolean fast){
//}
//
//static _INLINE_ void pinHi(UInt8 bit){
//}
//
//static _INLINE_ void pinLo(UInt8 bit){
//}
//
//UInt8 spiByte(UInt8 v){
//}
//
//void spiSingleClock(void){
//}
//
//void i2cStart(void){
//}
//
//Boolean i2cSend(UInt8 v){
//}
//
//UInt8 i2cRecv(Boolean ack){
//}
//
//void i2cStop(void){
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
