APPLICATION = nucleo-f411re-demo

BOARD ?= nucleo-f411re

RIOTBASE ?= $(CURDIR)/../RIOT/

DEVELHELP ?= 1

USEMODULE += saul saul_default xtimer

# Needed to enable user LED
CFLAGS += -DAUTO_INIT_LED0

include $(RIOTBASE)/Makefile.include
