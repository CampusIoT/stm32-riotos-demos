#include "net/loramac.h"
#include "semtech_loramac.h"

char *semtech_loramac_err_message(uint8_t errCode)
{
    switch (errCode)
    {
    case SEMTECH_LORAMAC_JOIN_SUCCEEDED:
        return "Join procedure succeeded";
    case SEMTECH_LORAMAC_JOIN_FAILED:
        return "Join procedure failed";
    case SEMTECH_LORAMAC_NOT_JOINED:
        return "MAC is not joined";
    case SEMTECH_LORAMAC_ALREADY_JOINED:
        return "MAC is already joined";
    case SEMTECH_LORAMAC_TX_OK:
        return "Transmission is in progress";
    case SEMTECH_LORAMAC_TX_SCHEDULE:
        return "TX needs reschedule";
    case SEMTECH_LORAMAC_TX_DONE:
        return "Transmission completed";
    case SEMTECH_LORAMAC_TX_CNF_FAILED:
        return "Confirmable transmission failed";
    case SEMTECH_LORAMAC_TX_ERROR:
        return "Error in TX (invalid param, unknown service)";
    case SEMTECH_LORAMAC_RX_DATA:
        return "Data received";
    case SEMTECH_LORAMAC_RX_LINK_CHECK:
        return "Link check info received";
    case SEMTECH_LORAMAC_RX_CONFIRMED:
        return "Confirmed ACK received";
    case SEMTECH_LORAMAC_BUSY:
        return "Internal MAC is busy";
    case SEMTECH_LORAMAC_DUTYCYCLE_RESTRICTED:
        return "Restricted access to channels";
    default:
        return "Unknown reason";
    }
}

/**
 * start the OTAA join procedure (and retries if required)
 */
uint8_t loramac_join_tentative_loop(semtech_loramac_t *loramac, uint8_t dataRate, uint32_t nextTentative, uint32_t maxTentative)
{
    printf("Starting join procedure: dr=%d\n", dataRate);

    semtech_loramac_set_dr(loramac, dataRate);

    uint8_t joinRes;
    while ((joinRes = semtech_loramac_join(loramac, LORAMAC_JOIN_OTAA)) != SEMTECH_LORAMAC_JOIN_SUCCEEDED)
    {
        printf("Join procedure failed: code=%d %s\n", joinRes, semtech_loramac_err_message(joinRes));

        if (dataRate > 0)
        {
            /* decrement Join Datarate */
            dataRate--;
            semtech_loramac_set_dr(loramac, dataRate);
        }
        else
        {
            /* double nextTentative in order to save the battery */
            if (nextTentative < maxTentative)
            {
                nextTentative *= 2;
            }
            else
            {
                nextTentative = maxTentative;
            }
        }
        printf("Retry join procedure in %ld sec. at dr=%d\n", nextTentative, dataRate);

        /* sleep JOIN_NEXT_TENTATIVE secs */
        xtimer_sleep(nextTentative);
    }

    puts("Join procedure succeeded");
    return joinRes;
}