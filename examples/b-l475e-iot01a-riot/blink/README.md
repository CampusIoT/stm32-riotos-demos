# B-L475E-IOT01A Demos with RIOTOS :: Blink on-board LEDs.

![b-l475e-iot01a](../images/b-l475e-iot01a.jpg)

## Install RIOT OS

Install RIOT OS and tools (gcc, gmake openocd)

```bash
mkdir -p ~/github/RIOT-OS
cd ~/github/RIOT-OS
git clone git://github.com/RIOT-OS/RIOT.git
export RIOT_BASE=~/github/RIOT-OS/RIOT/
```

## Build and flash

Plug an USB cable into the on-board STLink nicro-USB connector (not the USB OTG one). 

Flash the board
```bash
cd $RIOT_BASE/tests/leds
gmake BOARD=b-l475e-iot01a flash
```

## Serial Console

Configure and start the serial terminal minicom
```bash
ll /dev/tty.*
minicom -s
```

Press the reset button (the black one)

```
main(): This is RIOT! (Version: 2020.04-devel-1660-gb535c)                   
On-board LED test                                                            
                                                                             
Available LEDs: 2                                                            
                                                                             
Will now light up each LED once short and twice long in a loop
```
                                        


