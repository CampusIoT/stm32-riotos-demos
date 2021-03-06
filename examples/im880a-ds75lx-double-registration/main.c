/*
 * Read pediodically the temperature from the DS75LX sensor
 * then send to the LoRaWAN network in which the endpoint is registered.
 * The payload format is LPP Cayenne.
 * 
 * Copyright (C) 2020 LIG Université Grenoble Alpes
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <string.h>

#define ENABLE_DEBUG (1)
#include "debug.h"

#include "xtimer.h"
#include <time.h>

#include "mutex.h"
#include "periph_conf.h"
#include "periph/rtc.h"

#include "board.h"

#include "fmt.h"

#include "net/loramac.h"
#include "semtech_loramac.h"
#include "loramac_utils.h"
#include "cayenne_lpp.h"

#include "ds75lx.h"
#include "ds75lx_params.h"

#include "time_utils.h"


/* Declare globally the loramac descriptor */
static semtech_loramac_t loramac;
//static semtech_loramac_t loramac2;

/* Declare globally the sensor device descriptor */
ds75lx_t ds75lx;

/* Cayenne LPP buffer */
static cayenne_lpp_t lpp;

// LoRaMac values
#define JOIN_NEXT_RETRY_TIME            60 // Next join tentative in 1 minute
#define SECONDS_PER_DAY                 24 * 60 * 60

/* Use a fast datarate, e.g. BW125/SF7 in EU868 */
#define DR_INIT                         LORAMAC_DR_5
#define ADR_ON                          true

#define FIRST_TX_PERIOD                 30 // First Tx period 2 minutes
#define TX_PERIOD                       30 // Tx period 5 minutes
#define TIME_AFTER_TX1                  15  // Time after the first tx

#define PORT_UP_DATA                    1
#define PORT_UP_GET_EPOCH               2
#define PORT_UP_ERROR                   99

#define PORT_DN_TEXT                    1
#define PORT_DN_EPOCH                   2
#define PORT_DN_SET_TX_PERIOD           3

/* Implement the receiver thread */
#define RECEIVER_MSG_QUEUE                          (4U)

#ifdef FORGE_DEVEUI_APPEUI_APPKEY
static uint8_t secret[LORAMAC_APPKEY_LEN];
#endif

static uint8_t deveui[LORAMAC_DEVEUI_LEN];
static uint8_t appeui[LORAMAC_APPEUI_LEN];
static uint8_t appkey[LORAMAC_APPKEY_LEN];

static msg_t _receiver_queue[RECEIVER_MSG_QUEUE];
static char _receiver_stack[THREAD_STACKSIZE_DEFAULT];

static uint32_t epoch = 0;
static bool epoch_set = false;
static struct tm current_time;

static uint16_t tx_period = TX_PERIOD;


static void sender(void)
{
    while (1)
    {
        char message[64];

        // TODO si l'initialisation du ds75lx a échoué, il faut envoyer un message avec le fPort PORT_UP_ERROR

        /* do some measurements */
        int16_t temperature;
        ds75lx_wakeup(&ds75lx);
        /* Get temperature in degrees celsius */
        ds75lx_read_temperature(&ds75lx, &temperature);
        ds75lx_shutdown(&ds75lx);

        cayenne_lpp_add_temperature(&lpp, 0, (float)temperature / 100);

        bool negative = (temperature < 0);
        if (negative)
        {
            temperature = -temperature;
        }

        DEBUG("TX1: Sending LPP payload with : T=%c%d.%02d\n",
                negative ? '-' : ' ',
                (int)(temperature / 100),
                (int)(temperature % 100));

        /* send the LoRaWAN message */
        uint8_t ret;
        ret = semtech_loramac_send(&loramac, lpp.buffer, lpp.cursor);

        if (ret != SEMTECH_LORAMAC_TX_DONE)
        {
            DEBUG("TX1: Cannot send LPP payload: ret code: %d (%s)\n", ret, semtech_loramac_err_message(ret));
        }


        xtimer_sleep(TIME_AFTER_TX1);

        DEBUG("TX2: Sending LPP payload with : %s\n", message);
        /* send the LoRaWAN message */
        //ret = semtech_loramac_send(&loramac, lpp.buffer, lpp.cursor);

        if (ret != SEMTECH_LORAMAC_TX_DONE)
        {
            DEBUG("TX2: Cannot send LPP payload: ret code: %d (%s)\n", ret, semtech_loramac_err_message(ret));
        }


        /* clear buffer once done */
        cayenne_lpp_reset(&lpp);

        /* sleep tx_period secs */
        // TODO introduire un alea de quelques secondes dans la tx_period pour éviter que des endpoints qui redémarrent ensemble se brouillent les uns les autres.
        // TODO verifier que la tx_period est compatible avec le DC (sinon, le Tx retourne le code=13)
        xtimer_sleep(tx_period);
    }

    /* this should never be reached */
    return;
}


static void *receiver(void *arg)
{
    msg_init_queue(_receiver_queue, RECEIVER_MSG_QUEUE);

    (void)arg;
    while (1) {
        rtc_get_time(&current_time);
        print_time("Clock value is now ", &current_time);

        /* blocks until something is received */
        switch (semtech_loramac_recv(&loramac)) {
            case SEMTECH_LORAMAC_RX_DATA:
                // TODO process Downlink payload
                switch(loramac.rx_data.port) {
                    case PORT_DN_TEXT:
                        loramac.rx_data.payload[loramac.rx_data.payload_len] = 0;
                        DEBUG("Data received: text=%s, port: %d \n",
                            (char *)loramac.rx_data.payload, loramac.rx_data.port);
                        break;
                    case PORT_DN_EPOCH:
                        if(loramac.rx_data.payload_len == sizeof(epoch)) {
                            epoch=*((uint32_t*)loramac.rx_data.payload);
                            printf("Data received: epoch=%ld, port: %d\n",
                                epoch, loramac.rx_data.port);
                            struct tm new_time;
                            epoch_to_time(&new_time, epoch);
                            rtc_set_time(&new_time);
                            print_time("Clock value is set to ", &new_time);
                            epoch_set = true;

                        } else {
                            DEBUG("Data received: bad size for epoch, port: %d\n",
                                 loramac.rx_data.port);
                        }
                        break;
                    case PORT_DN_SET_TX_PERIOD:
                        if(loramac.rx_data.payload_len == sizeof(tx_period)) {
                            tx_period=*((uint16_t*)loramac.rx_data.payload);
                            printf("Data received: tx_period=%d, port: %d\n",
                                tx_period, loramac.rx_data.port);
                        } else {
                            DEBUG("Data received: bad size for tx_period, port: %d\n",
                                 loramac.rx_data.port);
                        }
                        break;
                    default:
                        DEBUG("Data received: ");
                        printf_ba(loramac.rx_data.payload, loramac.rx_data.payload_len);
                        DEBUG(", port: %d\n",loramac.rx_data.port);
                        break;
                }
                break;

            case SEMTECH_LORAMAC_RX_CONFIRMED:
                // TODO if too much unconfirmed Tx frames --> rejoin
                DEBUG("Received ACK from network\n");
                break;

            default:
                break;
        }
    }
    return NULL;
}


int main(void)
{
    int result;
    uint8_t port = PORT_UP_DATA;

    /* read RTC to confirm value */

    rtc_get_time(&current_time);
    print_time("Clock value is now ", &current_time);

    result = ds75lx_init(&ds75lx, &ds75lx_params[0]);
    if (result != DS75LX_OK)
    {
        puts("[Error] Failed to initialize DS75LX sensor");
        port = PORT_UP_ERROR;
    }
    semtech_loramac_set_tx_port(&loramac, port);

    /* initialize the loramac stack */
    semtech_loramac_init(&loramac);
    //semtech_loramac_init(&loramac2);

    /* set ADR flag */
    semtech_loramac_set_adr(&loramac, ADR_ON);
    //semtech_loramac_set_adr(&loramac2, ADR_ON);

#ifdef FORGE_DEVEUI_APPEUI_APPKEY
    /* forge the deveui, appeui and appkey of the endpoint */
    fmt_hex_bytes(secret, SECRET);
    loramac_forge_deveui(deveui,appeui,appkey,secret);
    DEBUG("Secret:"); printf_ba(secret,LORAMAC_APPKEY_LEN); printf("\n");
#else
    /* Convert identifiers and application key */
    fmt_hex_bytes(deveui, DEVEUI);
    fmt_hex_bytes(appeui, APPEUI);
    fmt_hex_bytes(appkey, APPKEY);
#endif

    DEBUG("DevEUI:"); printf_ba(deveui,LORAMAC_DEVEUI_LEN); DEBUG("\n");
    DEBUG("AppEUI:"); printf_ba(appeui,LORAMAC_APPEUI_LEN); DEBUG("\n");
    DEBUG("AppKey:"); printf_ba(appkey,LORAMAC_APPKEY_LEN); DEBUG("\n");

    /* set the LoRaWAN keys */
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);

    deveui[7]++;
    DEBUG("DevEUI:"); printf_ba(deveui,LORAMAC_DEVEUI_LEN); printf("\n");
    DEBUG("AppEUI:"); printf_ba(appeui,LORAMAC_APPEUI_LEN); printf("\n");
    DEBUG("AppKey:"); printf_ba(appkey,LORAMAC_APPKEY_LEN); printf("\n");

    /* set the LoRaWAN keys */
    //semtech_loramac_set_deveui(&loramac2, deveui);
    //semtech_loramac_set_appeui(&loramac2, appeui);
    //semtech_loramac_set_appkey(&loramac2, appkey);



    /* start the OTAA join procedure (and retries in required) */
    /*uint8_t joinRes = */ loramac_join_retry_loop(&loramac, DR_INIT, JOIN_NEXT_RETRY_TIME, SECONDS_PER_DAY);
    // /*uint8_t joinRes = */ loramac_join_retry_loop(&loramac2, DR_INIT, JOIN_NEXT_RETRY_TIME, SECONDS_PER_DAY);

    /* start the receiver thread */
    thread_create(_receiver_stack, sizeof(_receiver_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, receiver, NULL, "receiver thread");

    /* sleep FIRST_TX_PERIOD secs */
    xtimer_sleep(FIRST_TX_PERIOD);

    /* call the sender */
    sender();

    return 0; /* should never be reached */
}