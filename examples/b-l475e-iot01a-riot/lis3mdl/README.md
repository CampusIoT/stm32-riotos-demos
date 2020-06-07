# B-L475E-IOT01A Demos with RIOTOS :: Get temperature and orientation.

Goal : Get temperature and orientation from the High-performance 3-axis magnetometer (LIS3MDL).

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
cd $RIOT_BASE/tests/driver_lis3mdl
gmake BOARD=b-l475e-iot01a flash
```

## Serial Console

Configure and start the serial terminal minicom
```bash
ll /dev/tty.*
minicom -s
```

Press the reset button (the black one) and change the position of the board several times

```
main(): This is RIOT! (Version: 2020.04-devel-1660-gb535c)                      
LIS3MDL test application                                                        
Initializing LIS3MDL sensor                                                     
[ OK ]                                                                          
                                                                                
Magnetometer [G]:       X:  1   Y: -5   Z:  2                                   
Temperature:            25°C                                                    
Magnetometer [G]:       X:  1   Y: -5   Z:  2                                   
Temperature:            25°C                                                    
Magnetometer [G]:       X:  1   Y: -5   Z:  2                                                  
Temperature:            25°C                                                    
Magnetometer [G]:       X:  0   Y: -2   Z:  3                                   
Temperature:            25°C                                                    
Magnetometer [G]:       X:  0   Y: -2   Z:  3                                   
Temperature:            25°C                                                    
Magnetometer [G]:       X:  0   Y: -2   Z:  3                                   
Temperature:            25°C                                                    
Magnetometer [G]:       X:  0   Y: -2   Z:  3                                   
Temperature:            25°C                                                    
Magnetometer [G]:       X:  0   Y: -2   Z:  3                                   
Temperature:            25°C                                                    
Magnetometer [G]:       X:  1   Y: -5   Z:  2                                   
Temperature:            25°C                                                    
Magnetometer [G]:       X:  1   Y: -5   Z:  2                                   
Temperature:            25°C                                                    
```
                                        


