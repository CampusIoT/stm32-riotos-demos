# Press Button and Toogle LED

This example show how to execute a callback function on button press

## Build and flash

Build the firmware
```bash
export RIOT_BASE=~/github/RIOT-OS/RIOT
make binfile
```
Connect the board to the STLink and then flash the firmware
```bash
export RIOT_BASE=~/github/RIOT-OS/RIOT
make flash
```
## Console
Connect the board TX pin to USBSerial port and then configure and start `minicom`.
```bash
ll /dev/tty.*
minicom -s
```
