# Simple game console (TODO Add better name)

This repo contains source code for a simple handheld console based on ESP32 and old Nokia LCD.

All games are (hopefully) platform independent and should be easily portable to various platforms.

Supported features:
- Remote software update over WiFi
- 6 buttons
- 48x84 monochromatic LCD
- Integrated Lipol battery (500 mAh)
- Battery charger (XXX TODO Add number)
- x86 simulation for easier development

Current games:
- Arkanoid
- Maze (ported version of gamebuino-maze by Andy O'Neill. See https://github.com/aoneill01/gamebuino-maze .
- Pong 
- Snake (pixel perfect clone of Snake II from Nokia 3310 - WIP)
- Tower of Hanoi
- Space game (cheap copy of Space Impact - WIP)

This projects reuses code from following projects:
- Gamebuino (buttons and overall inspiration)
- Arduino Core
- ESP32 core
- gamebuino-maze

Build process:
```bash
TODO
```

