# Test App Clock


https://lns.campusiot.imag.fr/console/applications/riotds75/devices/riotds75-9855

Key app_clock
NNSXS.XXXXXXXXX.35NA2BCTHFX2LL3N4ZLN45ONXSQGUO3CPZCGWJEJP4AD47RX6M2A

Pour MQTTS
Username riotds75
Password La clé qui est spécifiée dans l'onglet Link

fPort for downlink are 202


Topic v3/riotds75/devices/riotds75-9855/down/push


## APP_CLOCK_CID_PackageVersionReq
Server : Used by the AS to request the package version implemented by the end-device

Downlink
00

{
  "downlinks": [{
    "f_port": 202,
    "frm_payload": "AA==",
    "priority": "NORMAL"
  }]
}



Endpoint should answer
000101


## APP_CLOCK_CID_DeviceAppTimePeriodicityReq
Server =  Used by the application server for 2 purposes: Set the periodicity at which the end-device shall transmit AppTimeReq messages and request an immediate transmission of end-device time

0200

> The Period is 0 (i.e. 128 +- 30 sec)

Endpoint should answer
0200GHIJKLMN

GHIJKLMN = endpoind epoch since GPS (1/1/1980)

Attention: DeviceTime est encodé en big-endian (LSBF)


## APP_CLOCK_CID_ForceDeviceResyncReq
Server: Used by the application server to the end-device to trigger a clock resynchronization

Downlink
03


## APP_CLOCK_CID_AppTimeReq

Endpoint : Used by end-device to request clock correction

01

Server : Conveys the clock timing correction


Attention: DeviceTime et TimeCorrection sont encodés en big-endian (LSBF)

1590332449 - (631160360 + ((365*8 + 366*2)*(24*60*60))) = 643639289

01F9275D2600 (TimeCorrection=643639289; Token=00)
AfknXSYA
{
  "downlinks": [{
    "f_port": 202,
    "frm_payload": "AfknXSYA",
    "priority": "HIGHEST"
  }]
}


app_clock_process_downlink                                                                                
APP_CLOCK_CID_AppTimeAns                                                                                  
Current time    :   2000-01-01 00:01:40                                                                   
Time Correction : 643639289
RTC time fixed  :   2020-05-24 12:43:09                                        
sent_buffer:                                                                                              
Current RTC time :   2020-05-24 12:43:09                                                                  
Last correction  :   2020-05-24 12:43:09                                                                  
Received ACK from network                                                                                 
Current RTC time :   2020-05-24 12:43:09                                                                  
Last correction  :   2020-05-24 12:43:09                                                                  
Read temperature= 25.00                                                                                   



010A00000001 (TimeCorrection=10; Token=01)
AQoAAAAB
{
  "downlinks": [{
    "f_port": 202,
    "frm_payload": "AQoAAAAB",
    "priority": "HIGHEST"
  }]
}

app_clock_process_downlink                                                                                
APP_CLOCK_CID_AppTimeAns                                                                                  
Current time    :   2020-05-24 12:43:44                                                                   
Time Correction : 10RTC time fixed  :   2020-05-24 12:43:54                                               
sent_buffer:                                                                                              
Current RTC time :   2020-05-24 12:43:54                                                                  
Last correction  :   2020-05-24 12:43:54                                                                  
Received ACK from network                                                                                 
Current RTC time :   2020-05-24 12:43:54                                                                  
Last correction  :   2020-05-24 12:43:54                                                                  



## X_APP_CLOCK_CID_AppTimeSetReq (Experimental)

1590332449 is GMT: Sunday 24 May 2020 15:00:49
1590332449 - ((365*8 + 366*2)*(24*60*60)) = 1274799649
FE21E6FB4B
/iHm+0s=

{
  "downlinks": [{
    "f_port": 202,
    "frm_payload": "/iHm+0s=",
    "priority": "HIGHEST"
  }]
}

app_clock_process_downlink                                                                                
X_APP_CLOCK_CID_AppTimeSetReq                                                                             
Current time    :   2020-05-24 15:05:45                                                                   
RTC time fixed  :   2020-05-24 15:00:49                                                                   
sent_buffer:                                                                                              


## Combination

00020003
AAIAAw==

{
  "downlinks": [{
    "f_port": 202,
    "frm_payload": "AAIAAw==",
    "priority": "HIGHEST"
  }]
}


(05 n'existe pas)
0200020000000005
AgACAAAAAAU=
{
  "downlinks": [{
    "f_port": 202,
    "frm_payload": "AgACAAAAAAU=",
    "priority": "HIGHEST"
  }]
}

