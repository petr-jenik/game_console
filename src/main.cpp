
#include "lcd_demo.h"
#include "ArduinoApi.h"

#include "games.h"

int main ()
{
    menu_setup();
    while (1)
    {
        menu_loop();
        delay(10);
    }

    //lcdDemo();

   return 0;
}