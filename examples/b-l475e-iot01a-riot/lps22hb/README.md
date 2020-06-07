# B-L475E-IOT01A Demos with RIOTOS :: Get temperature and pressure.

Goal : Get temperature and pressure from the 260-1260 hPa absolute digital output barometer (LPS22HB).

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
cd $RIOT_BASE/tests/driver_lpsxxx
DRIVER=lps22hb gmake BOARD=b-l475e-iot01a flash
```

## Serial Console

Configure and start the serial terminal minicom
```bash
ll /dev/tty.*
minicom -s
```

The console shows:
```
main(): This is RIOT! (Version: 2020.04-devel-1660-gb535c)
Test application for lps22hb pressure sensor                                    
                                                                                
Initializing lps22hb sensor                                                     
Pressure value: 985hPa - Temperature: 26.60°C                                   
Pressure value: 985hPa - Temperature: 26.63°C                                   
Pressure value: 985hPa - Temperature: 26.64°C                                   
Pressure value: 985hPa - Temperature: 26.65°C                                   
Pressure value: 985hPa - Temperature: 26.66°C                                   
```
                                        


