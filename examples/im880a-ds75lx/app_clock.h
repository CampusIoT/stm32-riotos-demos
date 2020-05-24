/*
 * app_clock.h
 * Implementation of LoRaWAN Application Layer Clock Synchronization v1.0.0 Specification
 * https://lora-alliance.org/resource-hub/lorawanr-application-layer-clock-synchronization-specification-v100
 *
 * LoRaWAN® Application Layer Clock Synchronization Specification, authored by the FUOTA Working Group of the
 * LoRa Alliance® Technical Committee, proposes an application layer messaging package running over LoRaWAN®
 * to synchronize the real-time clock of an end-device to the network’s GPS clock with second accuracy.
 */

#ifndef APP_CLOCK_H_
#define APP_CLOCK_H_

#include <inttypes.h>
#include "semtech_loramac.h"

#define APP_CLOCK_PORT								(uint8_t) 202  // Application Layer Clock

// Server : Used by the AS to request the package version implemented by the end-device
#define APP_CLOCK_CID_PackageVersionReq							(uint8_t)0x00
// Endpoint : Conveys the answer to PackageVersionReq
#define APP_CLOCK_CID_PackageVersionAns							(uint8_t)0x00
// Endpoint : Used by end-device to request clock correction
#define APP_CLOCK_CID_AppTimeReq								(uint8_t)0x01
// Server : Conveys the clock timing correction
#define APP_CLOCK_CID_AppTimeAns								(uint8_t)0x01
// Server =  Used by the application server for 2 purposes: Set the periodicity at which the end- device shall transmit AppTimeReq messages and request an immediate transmission of end-device time
#define APP_CLOCK_CID_DeviceAppTimePeriodicityReq				(uint8_t)0x02
// Endpoint
#define APP_CLOCK_CID_DeviceAppTimePeriodicityAns				(uint8_t)0x02
// Server: Used by the application server to the end-device to trigger a clock resynchronization
#define APP_CLOCK_CID_ForceDeviceResyncReq						(uint8_t)0x03

// 3.1 PackageVersionReq & Ans

// No PackageVersionReq struct

typedef struct  {
	// PackageIdentifier uniquely identifies the package. For the “clock synchronization package”
	// this identifier is 1.
	unsigned int PackageIdentifier : 8;
	// PackageVersion corresponds to the version of the package specification implemented by the
	// end-device
	unsigned int PackageVersion : 8;
}__attribute__((packed)) APP_CLOCK_PackageVersionAns_t;

// 3.2 AppTimeReq & Ans

typedef struct {
	// DeviceTime is the current end-device clock and is expressed as the time in seconds
	// since 180 00:00:00, Sunday 6th of January 1980 (start of the GPS epoch) modulo 2^32.
	unsigned int DeviceTime : 32;
	// TokenReq is a 4 bits counter initially set to 0. TokenReq is incremented (modulo 16) each time the end-device receives and processes successfully an AppTimeAns message.
	unsigned int TokenReq :4;
	// If the AnsRequired bit is set to 1 the end-device expects an answer whether its clock is well
	// synchronized or not. If this bit is set to 0, this signals to the AS that it only needs to answer if
	// the end-device clock is de-synchronized.
	unsigned int AnsRequired :1;
	unsigned int RFU :3;
}__attribute__((packed)) APP_CLOCK_AppTimeReq_t;

typedef struct {
	// TimeCorrection is a signed 32-bit integer, stipulating the time delta correction in seconds.
	int TimeCorrection : 32;
	unsigned int TokenAns :4;
	unsigned int RFU :4;
	// TokenReq is a 4 bits counter initially set to 0. TokenReq is incremented (modulo 16) each time the end-device receives and processes successfully an AppTimeAns message.
}__attribute__((packed)) APP_CLOCK_AppTimeAns_t;


typedef struct {
	// Period encodes the periodicity of the AppTimeReq transmissions. The actual periodicity in
	// seconds is 128.2𝑃𝑒𝑟𝑖𝑜𝑑 ±𝑟𝑎𝑛𝑑(30) where 𝑟𝑎𝑛𝑑(30) is a random integer in the +/-30sec
	// range varying with each transmission.
	unsigned int Period :4;
	unsigned int RFU :4;
}__attribute__((packed)) APP_CLOCK_DeviceAppTimePeriodicityReq_t;

typedef struct {
	// NotSupported bit is set to 1 if the end-device’s application does not accept a periodicity set
	// by the application server and manages the clock synchronization process and periodicity
	// itself.
	unsigned int NotSupported :1;
	// Time is the current end-device’s clock time captured immediately before the transmission of
	// the radio message.
	unsigned int RFU :7;
	unsigned int Time :32;
}__attribute__((packed)) APP_CLOCK_DeviceAppTimePeriodicityAns_t;

typedef struct {
	// The end-device responds by sending up to NbTransmissions AppTimeReq messages
	// with the AnsRequired bit set to 0.
	// The end-device stops re-transmissions of the AppTimeReq if a valid AppTimeAns is received.
	// If the NbTransmissions field is 0, the command SHALL be silently discarded.
	// The delay between consecutive transmissions of the AppTimeReq is application specific.
	unsigned int NbTransmissions :3;
	unsigned int RFU :5;
}__attribute__((packed)) APP_CLOCK_ForceDeviceResyncReq_t;


#define APP_CLOCK_OK							(int8_t)0
#define APP_CLOCK_ERROR_OVERFLOW				(int8_t)-1
#define APP_CLOCK_NOT_IMPLEMENTED				(int8_t)-2
#define APP_CLOCK_UNKNOWN_CID					(int8_t)-3
#define APP_CLOCK_BAD_TOKEN						(int8_t)-4

extern void app_clock_print_rtc(void);

extern int8_t app_clock_process_downlink(semtech_loramac_t *loramac);

extern int8_t app_clock_send_app_time_req(semtech_loramac_t *loramac);

#endif /* APP_CLOCK_H_ */
