
/*
 * app_clock.h
 * Implementation of LoRaWAN Application Layer Clock Synchronization v1.0.0 Specification
 * https://lora-alliance.org/resource-hub/lorawanr-application-layer-clock-synchronization-specification-v100
 *
 * LoRaWAN® Application Layer Clock Synchronization Specification, authored by the FUOTA Working Group of the
 * LoRa Alliance® Technical Committee, proposes an application layer messaging package running over LoRaWAN®
 * to synchronize the real-time clock of an end-device to the network’s GPS clock with second accuracy.
 */

#include "app_clock.h"

#include "xtimer.h"
#include <time.h>
//#include <stdbool.h>

#include "debug.h"
#include "net/loramac.h"
#include "semtech_loramac.h"

#include "periph_conf.h"
#include "periph/rtc.h"

#include "time_utils.h"

// 1972 and 1976 have 366 days
#define DELTA_EPOCH_GPS ((365*8 + 366*2)*(24*60*60))

// The end-device responds by sending up to NbTransmissions AppTimeReq messages
// with the AnsRequired bit set to 0.
// The end-device stops re-transmissions of the AppTimeReq if a valid AppTimeAns is received.
// If the NbTransmissions field is 0, the command SHALL be silently discarded.
// The delay between consecutive transmissions of the AppTimeReq is application specific.
// TODO static unsigned int NbTransmissions = 0;

// TokenReq is a 4 bits counter initially set to 0. TokenReq is incremented (modulo 16) each time the end-device receives and processes successfully an AppTimeAns message.
static unsigned int TokenReq = 0;

// If the AnsRequired bit is set to 1 the end-device expects an answer whether its clock is well
// synchronized or not. If this bit is set to 0, this signals to the AS that it only needs to answer if
// the end-device clock is de-synchronized.
// TODO static unsigned int AnsRequired = 1;

// Period encodes the periodicity of the AppTimeReq transmissions. The actual periodicity in
// seconds is 128.2𝑃𝑒𝑟𝑖𝑜𝑑 ±𝑟𝑎𝑛𝑑(30) where 𝑟𝑎𝑛𝑑(30) is a random integer in the +/-30sec
// range varying with each transmission.
static bool isPeriodDefined = false;
static unsigned int Period = 0;

static uint8_t sent_buffer[64];
static uint32_t sent_buffer_cursor = 0;

static time_t lastTimeCorrection = 0; // 01/01/1970

void app_clock_print_rtc(void) {
	/* read RTC */
	struct tm current_time;
	rtc_get_time(&current_time);
	print_time("Current RTC time : ", &current_time);
	struct tm lastTimeCorrectionTime = *localtime(&lastTimeCorrection);
	if(lastTimeCorrection == 0) {
		DEBUG("Last correction  : never");
	} else {
		print_time("Last correction  : ", &lastTimeCorrectionTime);
	}
}

int8_t app_clock_process_downlink(semtech_loramac_t *loramac) {

    //uint8_t fPort = loramac->rx_data.port;

    uint32_t len = loramac->rx_data.payload_len;

    uint32_t idx = 0;

    uint8_t* payload = (uint8_t*) loramac->rx_data.payload;

    int8_t error = APP_CLOCK_OK;

    sent_buffer_cursor = 0;

    while(idx < len  && (error == APP_CLOCK_OK)) {
    	uint8_t cid = payload[idx];
    	switch(cid) {
    	case APP_CLOCK_CID_PackageVersionReq:
    		if(idx + 1 + 0 <= len) {

    			sent_buffer[sent_buffer_cursor] = APP_CLOCK_CID_PackageVersionAns;
    			APP_CLOCK_PackageVersionAns_t *pva  = (APP_CLOCK_PackageVersionAns_t*)(sent_buffer + (1 + sent_buffer_cursor));
    			pva->PackageIdentifier = 1;
    			pva->PackageVersion = 1;

    			sent_buffer_cursor += (1 + sizeof(APP_CLOCK_PackageVersionAns_t));
    			idx += 1;
    		} else {
    			error = APP_CLOCK_ERROR_OVERFLOW;
    	        DEBUG("APP_CLOCK_CID_PackageVersionReq, error=%d\n", error);
    		}
    		break;

    	case APP_CLOCK_CID_DeviceAppTimePeriodicityReq:
    		if(idx + 1 + sizeof(APP_CLOCK_DeviceAppTimePeriodicityReq_t) <= len) {
    			APP_CLOCK_DeviceAppTimePeriodicityReq_t* datpr = (APP_CLOCK_DeviceAppTimePeriodicityReq_t*) (payload + (idx + 1));

    			isPeriodDefined = true;
    			Period = datpr->Period;

    			sent_buffer[sent_buffer_cursor] = APP_CLOCK_CID_DeviceAppTimePeriodicityAns;
    			APP_CLOCK_DeviceAppTimePeriodicityAns_t *datpa  = (APP_CLOCK_DeviceAppTimePeriodicityAns_t*)(sent_buffer + (1 + sent_buffer_cursor));
    			datpa->NotSupported = 0; // The endpoint is not supporting periodicity currently

			    struct tm current_time;
		        // Read the RTC current time
			    rtc_get_time(&current_time);
		        print_time("Current time: ", &current_time);
		        time_t timeSinceEpoch = mktime(&current_time);
		        // Substract number of seconds between 1/1/1980 and 1/1/1970
		        timeSinceEpoch -= DELTA_EPOCH_GPS;
    			datpa->Time = (unsigned int) timeSinceEpoch;

    			sent_buffer_cursor += (1 + sizeof(APP_CLOCK_DeviceAppTimePeriodicityAns_t));
    			idx += (1 + sizeof(APP_CLOCK_DeviceAppTimePeriodicityReq_t));
    		} else {
    			error = APP_CLOCK_ERROR_OVERFLOW;
    	        DEBUG("APP_CLOCK_CID_DeviceAppTimePeriodicityReq, error=%d\n", error);
    		}
    		break;

    	case APP_CLOCK_CID_AppTimeAns:
    		if(idx + 1 + sizeof(APP_CLOCK_AppTimeAns_t) <= len) {
    			APP_CLOCK_AppTimeAns_t* ata = (APP_CLOCK_AppTimeAns_t*) (payload + (idx + 1));

				unsigned int TokenAns = ata->TokenAns;
				if(TokenAns != TokenReq) {
	    			error = APP_CLOCK_BAD_TOKEN;
	    	        DEBUG("APP_CLOCK_CID_AppTimeAns, error=%d\n", error);
	    	        break;
				}

			    struct tm current_time;
		        // Read the RTC current time
			    rtc_get_time(&current_time);
		        print_time("Current time    : ", &current_time);
		        time_t timeSinceEpoch = mktime(&current_time);
		        // Apply correction
		        timeSinceEpoch += ata->TimeCorrection;
		        DEBUG(     "Time Correction : %d", ata->TimeCorrection);
		        current_time = *localtime(&timeSinceEpoch);
		        lastTimeCorrection = mktime(&current_time);
		        print_time("RTC time fixed  : ", &current_time);

		        // increment TokenReq
		        TokenReq++; TokenReq %= 16;

    			idx += (1 + sizeof(APP_CLOCK_AppTimeAns_t));
    		} else {
    			error = APP_CLOCK_ERROR_OVERFLOW;
    	        DEBUG("APP_CLOCK_CID_AppTimeAns, error=%d\n", error);
    		}
    		break;

    	case APP_CLOCK_CID_ForceDeviceResyncReq:
    		if(idx + 1 + sizeof(APP_CLOCK_ForceDeviceResyncReq_t) <= len) {
    			APP_CLOCK_ForceDeviceResyncReq_t* fdrr = (APP_CLOCK_ForceDeviceResyncReq_t*) (payload + (idx + 1));
    			unsigned int NbTransmissions = fdrr->NbTransmissions;
    			(void)NbTransmissions;
    			// TODO

    			idx += (1 + sizeof(APP_CLOCK_ForceDeviceResyncReq_t));
    			error = APP_CLOCK_NOT_IMPLEMENTED;
    	        DEBUG("APP_CLOCK_CID_ForceDeviceResyncReq, error=%d\n", error);
    		} else {
    			error = APP_CLOCK_ERROR_OVERFLOW;
    	        DEBUG("APP_CLOCK_CID_ForceDeviceResyncReq, error=%d\n", error);
    		}
    		break;
    	default:
			error = APP_CLOCK_UNKNOWN_CID;
	        DEBUG("APP_CLOCK : Unknown CID, error=%d\n", error);
    		break;
    	}
    }

    // TODO if NbTransmissions > 0, send an APP_CLOCK_CID_AppTimeReq

    return APP_CLOCK_OK;

}


int8_t app_clock_send_app_time_req(semtech_loramac_t *loramac) {

	(void)loramac;

    int8_t error = APP_CLOCK_OK;

	error = APP_CLOCK_NOT_IMPLEMENTED;
    DEBUG("app_clock_send_app_time_req, error=%d\n", error);

    return error;
}

