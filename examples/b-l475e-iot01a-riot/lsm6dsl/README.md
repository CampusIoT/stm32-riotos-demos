# B-L475E-IOT01A Demos with RIOTOS :: Get acceleration and 3D position.

Goal : Get acceleration and 3D position from the 3D accelerometer and 3D gyroscope (LSM6DSL).

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
cd $RIOT_BASE/tests/driver_lsm6dsl
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
LSM6DSL test application                                                        
Initializing LSM6DSL sensor at I2C_1... [SUCCESS]                               
                                                                                
Powering down LSM6DSL sensor...                                                 
[SUCCESS]                                                                       
                                                                                
Powering up LSM6DSL sensor...                                                   
[SUCCESS]                                                                       
                                                                                
Accelerometer x: 0 y: 0 z: 0                                                    
Gyroscope x: 0 y: 0 z: 0                                                        
Temperature [in °C x 100]: 2865                                                 
                                                                                
Accelerometer x: 9 y: 249 z: 989                                                
Gyroscope x: 0 y: -29 z: 9                                                      
Temperature [in °C x 100]: 2878                                                 
                                                                                
Accelerometer x: 10 y: 246 z: 991                                               
Gyroscope x: 0 y: -28 z: 8                                                      
Temperature [in °C x 100]: 2884                                                 

Accelerometer x: 112 y: 903 z: -161                                             
Gyroscope x: 6 y: -63 z: 113                                                    
Temperature [in °C x 100]: 2867                                                 
                                                                                
Accelerometer x: 164 y: 969 z: -8                                               
Gyroscope x: 16 y: -81 z: -24                                                   
Temperature [in °C x 100]: 2864                                                 
                                         
```
                                        


