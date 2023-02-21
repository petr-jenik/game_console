
#ifdef BUILD_FOR_X86
#include <ArduinoApi.h>
#else
#include <Arduino.h>
#endif //BUILD_FOR_X86

#include "games.h"

void setup()
{
    menuSetup();
}

void loop()
{
    menuLoop();
}
