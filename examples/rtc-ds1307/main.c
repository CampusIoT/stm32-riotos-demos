/*
 * Copyright (C) 2020 Université Grenoble Alpes
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file
 * @brief       Simple example for the DS1307 RTC driver
 *
 * @author      Didier Donsez
 *
 * @}
 */

#include <stdio.h>
#include <stdlib.h>

#include "ds1307.h"
#include "ds1307_params.h"
#include "timex.h"
#include "xtimer.h"
#include "shell.h"

static ds1307_t dev;

#define TM_YEAR_OFFSET      (1900)

static void print_time(const char *label, const struct tm *time)
{
    printf("%s  %04d-%02d-%02d %02d:%02d:%02d\n", label,
            time->tm_year + TM_YEAR_OFFSET,
            time->tm_mon + 1,
            time->tm_mday,
            time->tm_hour,
            time->tm_min,
            time->tm_sec);
}

static int get_rtc(int argc, char **argv)
{
    if (argc != 1) {
        printf("usage: %s\n", argv[0]);
        return 1;
    }

    struct tm time;
    ds1307_get_time(&dev, &time);
    print_time("RTC time : ", &time);
    return 0;
}

static int set_rtc(int argc, char **argv)
{

    if (argc != 7) {
        printf("usage: %s <year> <month> <day> <hour> <min> <sec>\n", argv[0]);
        return 1;
    }

    struct tm init = {
        .tm_sec = atoi(argv[6]),
        .tm_min = atoi(argv[5]),
        .tm_hour = atoi(argv[4]),
        .tm_mday = atoi(argv[3]),
        .tm_mon = atoi(argv[2]) - 1,
        .tm_year = atoi(argv[1]) - TM_YEAR_OFFSET
    };
    
    ds1307_set_time(&dev, &init);

    struct tm time;
    ds1307_get_time(&dev, &time);
    print_time("RTC time : ", &time);

    return 0;
}


static const shell_command_t shell_commands[] = {
    { "set_rtc", "Set the DS1307 RTC time", set_rtc },
    { "get_rtc", "Get the DS1307 RTC time", get_rtc },
    { NULL, NULL, NULL }
};

int main(void)
{
    xtimer_sleep(2);
    puts("RIOT DS1307 RTC Shell");

    /* initialize the device */
    int res = ds1307_init(&dev, (&ds1307_params[0]));
    if (res != 0) {
        puts("error: unable to initialize RTC [I2C initialization error]");
        return 1;
    }


    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
}
