# Press Button and Toogle LED

This example show how to execute a callback function on button press

## Build and flash

Build the firmware
```bash
export RIOT_BASE=~/github/RIOT-OS/RIOT
gmake BOARD=b-l475e-iot01a binfile
```
Connect the board to the STLink and then flash the firmware
```bash
export RIOT_BASE=~/github/RIOT-OS/RIOT
gmake BOARD=b-l475e-iot01a flash
```
## Console
Connect the board TX pin to USBSerial port and then configure and start `minicom`.
```bash
ll /dev/tty.*
minicom -s
```

```
main(): This is RIOT! (Version: 2020.04-devel-1660-gb535c)                      
Press User Button                                                                  
cpt=1                                                                           
cpt=2                                                                           
cpt=3                                                                           
cpt=4                                                                           
cpt=5                                                                           
cpt=6                                                                           
```
