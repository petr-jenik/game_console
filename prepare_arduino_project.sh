#!/bin/sh

DST="../game_console_project_arduino/"

find ./src/ -name *.ino -exec cp {} $DST/game_console_project_arduino.ino \;
find ./src/ -name *.c -exec cp {}  $DST \;
find ./src/ -name *.cpp -exec cp {}  $DST \;
find ./src/ -name *.h -exec cp {}  $DST \;
find ./src/ -name *.hpp -exec cp {}  $DST \;
find ./libs/adafruit_driver/Fonts/ -name *.h -exec cp {}  $DST \;
rm $DST/main.cpp