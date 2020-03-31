/*
 * Copyright (C) 2020 INRIA
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     pkg_semtech_loramac
 * @{
 *
 * @file
 * @brief       Utility functions for Semtech LoRaMac library.
 *
 * @author      Didier Donsez <didier.donsez@univ-grenoble-alpes.fr>
 *
 * @}
 */

#ifndef LORAMAC_UTILS_H
#define LORAMAC_UTILS_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C"
{
#endif

    char *semtech_loramac_err_message(uint8_t errCode);

    uint8_t loramac_join_retry_loop(semtech_loramac_t *loramac, uint8_t initDataRate, uint32_t nextRetryTime, uint32_t maxNextRetryTime);

#endif