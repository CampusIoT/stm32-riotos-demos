# Air Quality (BME680, CCS811)

## Components
* https://fr.aliexpress.com/item/32903358923.html
* https://fr.aliexpress.com/item/4001113450307.html

* [BME680 (relative humidity, barometric pressure, ambient temperature and VOC)](https://riot-os.org/api/group__drivers__bme680.html)
* [CCS811 (TVOC, eCO2)](https://riot-os.org/api/group__drivers__ccs811.html)

## Build

```bash
export RIOT_BASE=~/github/RIOT-OS/RIOT
cd ~/github/RIOT-OS/RIOT/tests/driver_ccs811
make BOARD=nucleo-f446re flash
```

```bash
export RIOT_BASE=~/github/RIOT-OS/RIOT
cd ~/github/RIOT-OS/RIOT/tests/driver_bme680
make BOARD=nucleo-f446re flash
```

## Console

```bash
make BOARD=nucleo-f446re term
```

```bash
ll /dev/tty.*
minicom -s
```
