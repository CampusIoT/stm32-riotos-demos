# Notes on CAN drivers

Author: Didier DONSEZ

## Repositories
* https://github.com/CANopenNode/CANopenNode
* https://github.com/zenglongGH/stm32f4-CANopen-project

## Links
* https://jeelabs.org/2018/canbus-part-3/
* https://www.instructables.com/id/STM32-CAN-Interface/
* https://community.st.com/s/question/0D50X00009XkeOsSAJ/stm32f4-can-bus-example-using-hal-library
* RIOT CAN http://riot-os.org/files/RIOT-Summit-2017-slides/6-1-Network-Session-OTAkeys-CAN.pdf
* http://wiki.labaixbidouille.com/index.php/Mise_en_place_d%27un_bus_CAN

## High-speed CAN Transceiver 

### Microchip
* https://www.microchip.com/wwwproducts/en/en010405 (Status: Not Recommended for new designs)
* https://www.microchip.com/wwwproducts/en/MCP2561 (Status: In Production)
* https://www.st.com/resource/en/datasheet/l9616.pdf (Status: In Production)

### STMicroelectronics
* [L9616 High Speed Can Bus Transceiver](https://www.st.com/en/automotive-analog-and-power/l9616.html)

### NXP
* [TJA1042](https://www.nxp.com/products/interfaces/can-transceivers/can-with-flexible-data-rate/high-speed-can-transceiver-with-standby-mode:TJA1042)
* [PCA82C251](https://www.nxp.com/docs/en/data-sheet/PCA82C251.pdf)

### TI
* [SN65HVD230](https://www.ti.com/product/SN65HVD230) compatible with [NXP PCA82C251](https://www.nxp.com/docs/en/data-sheet/PCA82C251.pdf)

## Boards
* https://www.seeedstudio.com/CAN-BUS-Shield-V2.html
* https://fr.aliexpress.com/item/32647794351.html

## USB-CAN adapters
* https://fr.aliexpress.com/wholesale?catId=0&initiative_id=SB_20201102054549&SearchText=USB-CAN+adapter
* https://www.tindie.com/products/lll7/can-usb-converter-uccb/ (recommended by SatRevolution)
    * http://ucandevices.pl/

# RIOT OS
* http://riot-os.org/files/RIOT-Summit-2017-slides/6-1-Network-Session-OTAkeys-CAN.pdf
* http://doc.riot-os.org/group__drivers__can.html
    * http://doc.riot-os.org/group__drivers__tja1042.html
    * http://doc.riot-os.org/group__candev__stm32.html

