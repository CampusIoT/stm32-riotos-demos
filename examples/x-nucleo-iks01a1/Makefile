APPLICATION = x-nucleo-iks01a1-demo

BOARD ?= nucleo-f411re

RIOTBASE ?= $(CURDIR)/../RIOT/

DEVELHELP ?= 1

USEMODULE += saul saul_default xtimer

# driver for MEMS devices
USEMODULE += lis3mdl lps25hb hts221 

# Needed to enable user LED
CFLAGS += -DAUTO_INIT_LED0

include $(RIOTBASE)/Makefile.include
