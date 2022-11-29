#include "Common.h"

#include <chrono> // Needed by delay
#include <thread> // Needed by delay

void yield(void){}


void init(void) {}
void initVariant(void) {}

/*
#ifndef HOST
int atexit(void (*func)()) __attribute__((weak));
#endif
int main() __attribute__((weak));
*/

void pinMode(pin_size_t pinNumber, PinMode pinMode){}
void digitalWrite(pin_size_t pinNumber, PinStatus status){}
PinStatus digitalRead(pin_size_t pinNumber){ return LOW;}
int analogRead(pin_size_t pinNumber) {return -1;}
void analogReference(uint8_t mode){}
void analogWrite(pin_size_t pinNumber, int value){}

//unsigned long millis(void){return 0}
//unsigned long micros(void){return 0};

using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

void delay(unsigned long timeMs)
{
    sleep_for(milliseconds(timeMs));
    //sleep_until(system_clock::now() + seconds(1));
}

void delayMicroseconds(unsigned int us)
{
    sleep_for(microseconds(us));

}
//unsigned long pulseIn(pin_size_t pin, uint8_t state, unsigned long timeout);
//unsigned long pulseInLong(pin_size_t pin, uint8_t state, unsigned long timeout);

//void shiftOut(pin_size_t dataPin, pin_size_t clockPin, BitOrder bitOrder, uint8_t val);
//uint8_t shiftIn(pin_size_t dataPin, pin_size_t clockPin, BitOrder bitOrder);

//void attachInterrupt(pin_size_t interruptNumber, voidFuncPtr callback, PinStatus mode);
//void attachInterruptParam(pin_size_t interruptNumber, voidFuncPtrParam callback, PinStatus mode, void* param);
//void detachInterrupt(pin_size_t interruptNumber);

//void setup(void);
//void loop(void);

// WMath prototypes
long random(long){return 3;}
long random(long, long){ return 2;}
void randomSeed(unsigned long){}
//long map(long, long, long, long, long);

