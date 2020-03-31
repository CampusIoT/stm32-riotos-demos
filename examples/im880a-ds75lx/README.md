# IMST iM880a Board

The IMST iM880a board is a simple prototyping board with an IMST IMST iM880a LoRa module and a DS75LX temperature sensor.

The demonstration program reads the temperature from the DS75LX sensor and sends it into an uplink message over a LoRaWAN network. The program prints the payload of the downlink messages. 

## Libraries

Board:
* [boards/im880b](https://github.com/RIOT-OS/RIOT/tree/master/boards/im880b)

Drivers:
* [drivers/ds75lx](https://github.com/RIOT-OS/RIOT/tree/master/drivers/ds75lx)


## Build and flash

Configure the DevEUI, the AppEUI and the AppKey of the LoRaWAN endpoint into the header file `activation.h`.

Optional : Configure the following parameters into the program file `main.c` : `TX_PERIOD`, `DR_INIT`, `ADR_ON`.

Register the endpoint into a LoRaWAN network (public or private) using the DevEUI, the AppEUI and the AppKey

Build the firmware
```bash
export RIOT_BASE=~/github/RIOT-OS/RIOT
make binfile
```
Connect the board to the STLink according this [tutorial](https://github.com/CampusIoT/tutorial/tree/master/im880a) and then flash the firmware
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

## Downlink

Send a downlink message to the endpoint throught your network server.

For CampusIoT:
```bash
ORGID=<YOUR_ORG_ID>
BROKER=lora.campusiot.imag.fr
MQTTUSER=org-$ORGID
MQTTPASSWORD=<YOUR_ORG_TOKEN>
applicationID=1
devEUI=1234567890abcdef
mosquitto_pub -h $BROKER -u $MQTTUSER -P $MQTTPASSWORD -t "application/$applicationID/device/$devEUI/tx" -m '{"reference": "abcd1234","confirmed": true, "fPort": 10,"data":"SGVsbG8gQ2FtcHVzSW9UICE="}'
```

The output on the console is:
```bash
main(): This is RIOT! (Version: 2020.04-devel-1660-gb535c)                      
Starting join procedure: dr=5                                                   
Join procedure succeeded                                                        
Sending LPP payload with : T: 22.75                                             
Received ACK from network                                                       
Sending LPP payload with : T: 22.75                                             
Data received: Hello CampusIoT !, port: 10                                      
Received ACK from network                                                       
```

## References
* https://github.com/CampusIoT/tutorial/tree/master/riotos
* https://github.com/CampusIoT/tutorial/tree/master/im880a

