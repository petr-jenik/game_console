#!/bin/sh

export PATH=$PATH:./libs
(cd python_lcd_simulation && python run_lcd_sim.py)&
./build/bin/game_console.exe