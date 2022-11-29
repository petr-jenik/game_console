#!/bin/sh

export PATH=$PATH:./libs
(cd python_lcd_simulation && python run_lcd_sim.py)&
./bin/game_console.exe