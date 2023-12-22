#ifdef BUILD_FOR_X86

#include "Common.h"

#include <chrono> // Needed by delay
#include <thread> // Needed by delay
#include <cstring> // Needed by strlen
#include <iostream> // Neeed by cout

#include "Buttons.h" // Needed to be able to simulate a key press
#include "my_gui.h"

#include <stdlib.h>

void yield(void){}


void init(void) {}
void initVariant(void) {}

/*
#ifndef HOST
int atexit(void (*func)()) __attribute__((weak));
#endif
int main() __attribute__((weak));
*/

PinStatus fakeGpios[255];



void Button_processKeyStates(const char * rxMessage)
{
    if (strlen(rxMessage) != (Buttons::eKey_COUNT))
    {
        std::cout << "Invalid state of keys received" << std::endl;
        return;
    }

    bool retval = false;
    for (int keyIndex = static_cast<int>(Buttons::eKey_FIRST); keyIndex <= static_cast<int>(Buttons::eKey_LAST); keyIndex++)
    {
        char c = rxMessage[keyIndex];

        if ((c != '0') && (c != '1'))
        {
            std::cout << "Invalid state of keys received" << std::endl;
            return;
        }

        GUI& gui  = GUI::getInstance();
        // Simulate a key press on a corresponding GPIO pin
        digitalWrite(gui.buttons.pins[keyIndex], (c == '1') ? LOW : HIGH);
    }
}

void pinMode(pin_size_t pinNumber, PinMode pinMode){}

void digitalWrite(pin_size_t pinNumber, PinStatus status)
{
    fakeGpios[pinNumber] = status;
    //std::cout << "Set pin " << pinNumber << " to state " << status << std::endl;
}

PinStatus digitalRead(pin_size_t pinNumber)
{
    return fakeGpios[pinNumber];
}


int analogRead(pin_size_t pinNumber) {
    static int voltage = 0;
    voltage++;
    voltage = voltage % 4096;
    return voltage;
    }
void analogReference(uint8_t mode){}
void analogWrite(pin_size_t pinNumber, int value){}

//unsigned long millis(void){return 0}
//unsigned long micros(void){return 0};

using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

void delay(unsigned long timeMs)
{
    volatile int32_t x = 1;
    auto endTime = millis() + timeMs;
    while(endTime > millis())
    {
        x++;
        /*if (x % 500 == 0)
        {
            std::cout << "******************************" << std::endl;
            std::cout << "Delay is running" << std::endl;
            std::cout << "Current time: " << millis() << std::endl;
            std::cout << "End time: " << endTime << std::endl;
            std::cout << "X: " << x << std::endl;
        }*/
    }
    //sleep_for(milliseconds(timeMs));
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
long random(long nMax)
{
    long value = rand() % nMax;      // Returns a pseudo-random integer between 0 and RAND_MAX.
    std::cout << "Random value: " << value << std::endl;
    return value;
}
long random(long nMin, long nMax)
{
    return rand() % nMax;
    //return 3;
}

void randomSeed(unsigned long)
{
    srand(time(NULL));   // Initialization, should only be called once.
}
//long map(long, long, long, long, long);

#endif //BUILD_FOR_X86
