
#include "lcd_demo.h"
#include "ArduinoApi.h"

#include "games.h"

int main ()
{
    arkanoid_setup();
    while (1)
    {
        arkanoid_loop();
        delay(10);
    }

    //
    //lcdDemo();

   return 0;
}