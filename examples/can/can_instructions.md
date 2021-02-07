# CAN :: Instructions

## Driver

L9616 is compatible with the [NXP TJA1042](https://www.nxp.com/docs/en/data-sheet/TJA1042.pdf) CAN driver.

The TJA1042 driver for RIOT should be added as a module.

```makefile

TRX_TO_BUILD ?= tja1042

ifneq (,$(filter tja1042,$(TRX_TO_BUILD)))
  USEMODULE += tja1042
	TJA1042_STB_PIN ?= GPIO_PIN\(0,0\)

	CFLAGS += -DTJA1042_STB_PIN=$(TJA1042_STB_PIN)
endif

```



## Test F446RE CAN (x2)


### Build

```bash
cd ~/github/RIOT-OS/RIOT
cd tests/conn_can/
gmake BOARD=nucleo-f446re
cat README.md
```

Normalement, il faudrait modifier le Makefile en commentant TRX_TO_BUILD

```makefile
...
# TRX_TO_BUILD ?= tja1042
...
```
Cependant, en faisant cela, ca ne fonctionne plus (12/11/2020)


### On Nucleo #1 and #2
Test démarré le 9/11/2020
TD  démarré le 9/11/2020

```
ps
help
can list
```

> Remarque : Il n'y a qu'une des 2 interfaces CAN du Nucleo F446RE !!!

To initialize a CAN transceiver device (trx_id = 0)
```
init 0
```

To set a CAN transceiver device's (trx_id = 0) mode to TRX_NORMAL_MODE
```
set_mode 0 0
```

The CAN interfaces are registered at startup to the dll. The list of registered
interfaces and their RIOT names can be retrieved with:
```
test_can list
```

### On Nucleo #1

To send a raw CAN frame, id 0x100 with 2 bytes of data 01 02 on interface 0:
```
test_can send 0 100 01 02
```

To send a raw CAN remote request frame, id 0x100 on interface 0 and datalenght 7:
```
test_can sendrtr 0 100 7
```
A remote request frame has a NULL payload but can have a specific data length code (DLC).
Valid DLC val: 0..8

To send a raw CAN remote request frame, id 0x100 and id 0x500 on interface 0:
> can send ifnum id [B1 .. B8]
```
can send 0 100 AA BB CC DD EE FF
can send 0 500 CC DD EE FF AA BB
```

### On Nucleo #2

Two threads are launched to enable receiving frames. To receive raw CAN frames,
ids 0x100 and 0x500 with thread 0 on interface 1, with 10s timeout:

> test_can send ifnum can_id [B1 [B2 [B3 [B4 [B5 [B6 [B7 [B8]]]]]]]]              
> test_can recv ifnum user_id timeout can_id1 [can_id2..can_id16]                 

```
test_can recv 0 0 10000000 100 500
```

### On Nucleo #1 and #2

A connection can be closed with its thread id, for instance:
```
test_can close 0
```

### On Nucleo #1 and #2 (ERROR)

To send an ISO-TP datagram, first bind a connection with one of the threads,
source id 700, dest id 708, thread 1 and interface 0:
```
test_can bind_isotp 0 1 700 708
```
Then send the data 01 02 03 04 05 0a 0b 0c:
```
test_can send_isotp 1 01 02 03 04 05 0a 0b 0c
```

To receive from an ISO-TP channel, it must be bound, then with the previous channel,
and 10s timeout:
```
test_can recv_isotp 1 10000000
```

An ISO-TP channel can be closed with:
```
test_can close_isotp 1
```

### On Nucleo #1 and #2

```
test_can get_bitrate 0
```

You can also set a bitrate (this won't work on native with vcan, only with real
interfaces, but then root access are needed), for instance 250000 bit/s with
sampling point 87.5%:
```

test_can set_bitrate 0 250000 875

test_can get_bitrate 0

```


For SatRev

```
> test_can set_bitrate 0 1000000 750                                            
Setting bitrate=1000000, sample point=750                                       
Bittimings successfully set                                                     
> test_can get_bitrate 0                                                        
Bitrate read: bitrate=1000000, sample_point=733                                 
brp=3phase-seg1=5, phase-seg2=4, sjw=1                                          
```




### Test set_mode

```
> set_mode                                                                      
usage: set_mode [trx_id] [mode]                                                 
modes:                                                                          
        0: normal mode                                                          
        1: silent mode                                                          
        2: standby mode                                                         
        3: high-speed mode (SW CAN only)                                        
        4: high-voltage wakeup mode (SW CAN only)                               
> set_mode 0 4                                                                  
Error when setting mode: -1                                                     
> set_mode 0 3                                                                  
Error when setting mode: -1                                                     
> set_mode 0 2                                                                  
Mode successfully set                                                           
> set_mode 0 1                                                                  
Mode successfully set                                                           
> set_mode 0 0                                                                  
Mode successfully set                                                           
 
```



### Test ISO-TP


#### Nucleo-F446RE #1 (700 --> 708)
To receive an ISO-TP datagram, first bind a connection with one of the threads,
source id 700, dest id 708, thread 1 and interface 1:
```
> test_can bind_isotp 1 1 700 708                                               
> test_can send_isotp 1 01 02 03 04 05 0a 0b 0c                                 
> test_can send_isotp 1 01 02 03 04 05 0a 0b 0c                                 
> test_can send_isotp 1 01 02 03 04 05 0a 0b 0c

Attendre que la réception fasse un timeout
                                 
> test_can send_isotp 1 01 02 03 04 05 0a 0b 01                                 
> test_can send_isotp 1 01 02 03 04 05 0a 0b 02                                 
                           
Relancez la réception sur le Nucleo-F446RE : il semblerait qu'il y a une bufferisation quelque part

```

#### Nucleo-F446RE #2 (708 --> 700) : 
To send an ISO-TP datagram, first bind a connection with one of the threads,
source id 708, dest id 700, thread 1 and interface 0:
```
> test_can bind_isotp 0 1 708 700                                               
> test_can recv_isotp 1 100000000                                               
> 1: can_stm32_0 ISOTP [8]  01 02 03 04 05 0A 0B 0C                             
1: recv terminated: ret=-116                                                    
test_can recv_isotp 1 100000000                                                 
1: can_stm32_0 ISOTP [8]  01 02 03 04 05 0A 0B 0C                               
> 1: can_stm32_0 ISOTP [8]  01 02 03 04 05 0A 0B 0C                             
test_can get_filter 0                                                           
Filter 0: 0x700                                                                 
Mask 0: 0x7ff                                                                   
> 1: recv terminated: ret=-116                                                  
test_can recv_isotp 1 100000000                                                 
1: can_stm32_0 ISOTP [8]  01 02 03 04 05 0A 0B 01                               
1: can_stm32_0 ISOTP [8]  01 02 03 04 05 0A 0B 02                               

```

Pour les 2 boards : An ISO-TP channel can be closed with:
```
test_can close_isotp 1
```


### Filter et ISO-TP
```
> test_can get_filter 0                                                         
No filter set                                                                   
> test_can bind_isotp 0 1 700 708                                               
> test_can get_filter 0                                                         
Filter 0: 0x708                                                                 
Mask 0: 0x7ff                                                                   
> test_can close_isotp 1                                                        
> test_can get_filter 0                                                         
No filter set                                                                   
> 
```

### Test init

To initialize a CAN transceiver device (trx_id = 0)
```
init 1
init 0
```
Le programme bloque sur l'init. La led TX de FTDI est rouge mais rien n'apparait sur minicom.



### Test driver bxCAN pour STM32

http://doc.riot-os.org/group__candev__stm32.html

TODO


```
> cd github/RIOT-OS/RIOT/
> git grep trx_driver_t
drivers/include/can/can_trx.h:typedef struct trx_driver trx_driver_t;
drivers/include/can/can_trx.h:    const trx_driver_t *driver; /**< driver */
drivers/include/ncv7356.h:extern const trx_driver_t ncv7356_driver;
drivers/include/tja1042.h:extern const trx_driver_t tja1042_driver;
drivers/ncv7356/ncv7356.c:const trx_driver_t ncv7356_driver = {
drivers/tja1042/tja1042.c:const trx_driver_t tja1042_driver = {
```

