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

#include "xtimer.h"

#include "net/loramac.h"
#include "semtech_loramac.h"

#include "ds75lx.h"
#include "ds75lx_params.h"

#include "cayenne_lpp.h"

#include "board.h"

/* Declare globally the loramac descriptor */
static semtech_loramac_t loramac;

/* Declare globally the sensor device descriptor */
ds75lx_t ds75lx;

/* Cayenne LPP buffer */
static cayenne_lpp_t lpp;

#include "activation.h"

#include "loramac_utils.h"

// LoRaMac values
#define JOIN_NEXT_TENTATIVE 60 // Next join tentative in 1 minute
#define SECONDS_PER_DAY 24 * 60 * 60

#define DR_INIT 5
#define ADR_ON true

#define TX_PERIOD 10 // Tx period 5 minutes

#define DATA_PORT 2
#define ERROR_PORT 99

static void sender(void)
{
    while (1)
    {
        char message[64];

        // TODO introduire un alea dans la TX_PERIOD
        // TODO verifier que la TX_PERIOD est compatible avec le DC (sinon, le Tx retourne le code=13)

        /* sleep TX_PERIOD secs */
        xtimer_sleep(TX_PERIOD);

        // TODO si l'initialisation du ds75lx a échoué, il faut envoyer un message avec le fPort ERROR_PORT

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

        sprintf(message, "T:%c%d.%02d",
                negative ? '-' : ' ',
                (int)(temperature / 100),
                (int)(temperature % 100));

        //uint8_t dataRate = semtech_loramac_get_dr(&loramac);
        //printf("Sending LPP payload with : %s at dr=%d\n", message, dataRate);
        printf("Sending LPP payload with : %s\n", message);

        /* send the LoRaWAN message */
        uint8_t ret = semtech_loramac_send(&loramac, lpp.buffer, lpp.cursor);

        if (ret != SEMTECH_LORAMAC_TX_DONE)
        {
            printf("Cannot send LPP payload: ret code: %d %s\n", ret, semtech_loramac_err_message(ret));
        }

        /* clear buffer once done */
        cayenne_lpp_reset(&lpp);
    }

    /* this should never be reached */
    return;
}

int main(void)
{
    int result;
    uint8_t port = DATA_PORT;

    result = ds75lx_init(&ds75lx, &ds75lx_params[0]);
    if (result != DS75LX_OK)
    {
        puts("[Error] Failed to initialize DS75LX sensor");
        port = ERROR_PORT;
    }
    semtech_loramac_set_tx_port(&loramac, port);

    /* initialize the loramac stack */
    semtech_loramac_init(&loramac);

    /* set ADR flag */
    semtech_loramac_set_adr(&loramac, ADR_ON);

    /* set the LoRaWAN keys */
    // TODO for DevEUI, find the id of the MCU
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);

    /* start the OTAA join procedure (and retries in required) */
    /*uint8_t joinRes = */ loramac_join_tentative_loop(&loramac, DR_INIT, JOIN_NEXT_TENTATIVE, SECONDS_PER_DAY);

    /* call the sender */
    sender();

    return 0; /* should never be reached */
}
