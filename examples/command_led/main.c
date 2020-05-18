/*
 * Copyright (C) 2018 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdio.h>
#include <stdlib.h>

#include "shell.h"
#include "board.h"

static int cmd_handler(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    LED0_TOGGLE;
    return 0;
}

static const shell_command_t shell_commands[] = {
    { "toggle", "toggle led", cmd_handler},
    { NULL, NULL, NULL }
};

int main(void)
{
    puts("shell application that toggles on-board LEDs");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}
