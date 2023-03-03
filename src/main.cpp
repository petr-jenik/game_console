#define BUILD_FOR_X86

#ifdef BUILD_FOR_X86

#include "ArduinoApi.h"
#include "games.h" 
#warning "TODO Remove simulation and add real IO pins"

#include <iostream>

#include "my_gui.h"

bool bStarted = false;

void setTrace(bool bEnabled)
{
    bStarted = bEnabled;
}

extern "C" {
	void __cyg_profile_func_enter(void *func,  void *caller) {
        if (bStarted)
            std::cout << "Start: " << func << std::endl;
	}
	void __cyg_profile_func_exit (void *func, void *caller) {
        if (bStarted)
    		std::cout << "Exit: " << func << std::endl;
	}
};

int main ()
{
    //bStarted = true;
    menuSetup();
    while(1)
    {
        menuLoop();
        //delay(50);
    }

    //lcdDemo();

    return 0;
}

#endif