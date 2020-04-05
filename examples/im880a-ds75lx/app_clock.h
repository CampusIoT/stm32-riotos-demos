/*
 * app_clock.h
 * Implementation of LoRaWAN Application Layer Clock Synchronization v1.0.0 Specification
 * https://lora-alliance.org/resource-hub/lorawanr-application-layer-clock-synchronization-specification-v100
 */

#ifndef APP_CLOCK_H_
#define APP_CLOCK_H_

#include <inttypes.h>

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

struct APP_CLOCK_PackageVersionAns {
	// PackageIdentifier uniquely identifies the package. For the “clock synchronization package”
	// this identifier is 1.
	unsigned int PackageIdentifier : 8;
	// PackageVersion corresponds to the version of the package specification implemented by the
	// end-device
	unsigned int PackageVersion : 8;
}__attribute__((packed));

// 3.2 AppTimeReq & Ans

struct APP_CLOCK_AppTimeReq {
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
}__attribute__((packed));

struct APP_CLOCK_AppTimeAns {
	// TimeCorrection is a signed 32-bit integer, stipulating the time delta correction in seconds.
	int TimeCorrection : 32;
	unsigned int TokenAns :4;
	unsigned int RFU :4;
	// TokenReq is a 4 bits counter initially set to 0. TokenReq is incremented (modulo 16) each time the end-device receives and processes successfully an AppTimeAns message.
}__attribute__((packed));


struct APP_CLOCK_DeviceAppTimePeriodicityReq {
	// Period encodes the periodicity of the AppTimeReq transmissions. The actual periodicity in
	// seconds is 128.2𝑃𝑒𝑟𝑖𝑜𝑑 ±𝑟𝑎𝑛𝑑(30) where 𝑟𝑎𝑛𝑑(30) is a random integer in the +/-30sec
	// range varying with each transmission.
	unsigned int Period :4;
	unsigned int RFU :4;
}__attribute__((packed));

struct APP_CLOCK_DeviceAppTimePeriodicityAns {
	// NotSupported bit is set to 1 if the end-device’s application does not accept a periodicity set
	// by the application server and manages the clock synchronization process and periodicity
	// itself.
	unsigned int NotSupported :1;
	// Time is the current end-device’s clock time captured immediately before the transmission of
	// the radio message.
	unsigned int RFU :7;
	unsigned int Time :32;
}__attribute__((packed));

struct APP_CLOCK_ForceDeviceResyncReq {
	// The end-device responds by sending up to
	//  NbTransmissions AppTimeReq messages with the AnsRequired bit set to 0. The end-
	// device stops re-transmissions of the AppTimeReq if a valid AppTimeAns is received. If the
	// NbTransmissions field is 0, the command SHALL be silently discarded.
	// The delay between consecutive transmissions of the AppTimeReq is application specific.
	unsigned int NbTransmissions :3;
	unsigned int RFU :5;
}__attribute__((packed));

#endif /* APP_CLOCK_H_ */
