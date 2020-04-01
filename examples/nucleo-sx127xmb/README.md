# Nucleo Board and SX1272MB2xAS / SX1272MB2DAS / SX1276MB1xAS

The SX1272MB2xAS / SX1272MB2DAS / SX1276MB1xAS boards are evaluation boards from Semtech for evaluating and demonstrating LoRa and LoRaWAN communications.

The demonstration program reads a button press counter and sends it into an uplink message over a LoRaWAN network. The program prints the payload of the downlink messages. 

## Libraries

Board:
* Nucleo boards (64 or 144)
* [SX1272MB2xAS/SX1272MB2DAS](https://os.mbed.com/components/SX1272MB2xAS/)
* [SX1276MB1xAS](https://os.mbed.com/components/SX1276MB1xAS/)

## Build and flash

By default, the DevEUI, the AppEUI and the AppKey are forged using the CPU ID of the MCU. However, you can set the DevEUI, the AppEUI and the AppKey of the LoRaWAN endpoint into the `main.c`.

Optional : Configure the following parameters into the program file `main.c` : `FIRST_TX_PERIOD`, `TX_PERIOD`, `DR_INIT`, `ADR_ON`, `DEBUG_ON` and `SECRET`.

Register the endpoint into a LoRaWAN network (public or private) using the DevEUI, the AppEUI and the AppKey

Set the `DRIVER` variable for [SX1272MB2xAS/SX1272MB2DAS](https://os.mbed.com/components/SX1272MB2xAS/)
```bash
export DRIVER=sx1272
```

Set the `DRIVER` variable for [SX1276MB1xAS](https://os.mbed.com/components/SX1276MB1xAS/)
```bash
export DRIVER=sx1276
```

Select your nucleo board in the list
```bash
export RIOT_BASE=~/github/RIOT-OS/RIOT
make info-boards
```

Build the firmware
```bash
export RIOT_BASE=~/github/RIOT-OS/RIOT
make BOARD=nucleo-f411re DRIVER=$DRIVER binfile
```

Connect the board to the STLink and then flash the firmware
```bash
export RIOT_BASE=~/github/RIOT-OS/RIOT
make flash
```

## Console
Configure and start `minicom`.

```bash
ll /dev/tty.*
minicom -s
```

## AppKey

The AppKey can be retrieved from the DevEUI (displayed at startup) and the SECRET (flashed into the firmware) with the command lines below:

```bash
SECRET=cafebabe02000001cafebabe02ffffff                                         
DevEUI=33323431007f1234                                                         
AppEUI=33323431ffffffff                                                        
SHA=$(echo -n $DevEUI$AppEUI$SECRET | xxd -r -p | shasum -b | awk '{print $1}')
AppKey="${SHA:0:32}"
echo $AppKey
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
Secret:cafebabe02000001cafebabe02ffffff                                         
DevEUI:33323431007f1234                                                         
AppEUI:33323431ffffffff                                                         
AppKey:f482a62f0f1234ac960882a2e25f971b                                         
Starting join procedure: dr=5                                                   
Join procedure succeeded                                                        
Sending LPP payload with : C: 10                                             
Received ACK from network                                                       
Sending LPP payload with : T: 36                                             
Data received: Hello CampusIoT !, port: 10                                      
Received ACK from network                                                       
```

## References
* https://github.com/CampusIoT/tutorial/tree/master/riotos


## TODO
* Read GPIO for the Grove connectors of [SX1272MB2xAS/SX1272MB2DAS](https://os.mbed.com/components/SX1272MB2xAS/)
