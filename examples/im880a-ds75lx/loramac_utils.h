
#ifndef LORAMAC_UTILS_H
#define LORAMAC_UTILS_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C"
{
#endif

    char *semtech_loramac_err_message(uint8_t errCode);

    uint8_t loramac_join_tentative_loop(semtech_loramac_t *loramac, uint8_t dataRate, uint32_t nextTentative, uint32_t maxTentative);

#endif