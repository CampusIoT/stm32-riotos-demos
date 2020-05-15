
#include "app_clock.h"

#include "xtimer.h"
#include <time.h>

#include "debug.h"
#include "net/loramac.h"
#include "semtech_loramac.h"
#include "time_utils.h"
#include "periph/rtc.h"

static uint32_t epoch = 0;
static bool epoch_set = false;
static struct tm current_time;


uint8_t process_app_clock(semtech_loramac_t *loramac) {

    (void)current_time;


    if(loramac->rx_data.payload_len == sizeof(epoch)) {
        epoch=*((uint32_t*)loramac->rx_data.payload);
        DEBUG("Data received: epoch=%ld, port: %d\n",
            epoch, loramac->rx_data.port);
        struct tm new_time;
        epoch_to_time(&new_time, epoch);
        rtc_set_time(&new_time);
        print_time("Clock value is set to ", &new_time);
        epoch_set = true;

    } else {
        DEBUG("Data received: bad size for epoch, port: %d\n",
             loramac->rx_data.port);
    }

    return 0;

}
