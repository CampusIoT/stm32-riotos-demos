APPLICATION = cayenne-lpp-demo

BOARD ?= nucleo-f411re

RIOTBASE ?= $(CURDIR)/../RIOT/

DEVELHELP ?= 1

USEMODULE += saul saul_default xtimer

# driver for MEMS devices
USEMODULE += lis3mdl lps25hb hts221

# allow to printf float
USEMODULE += printf_float

# use package
USEPKG += cayenne-lpp

# Needed to enable user LED
CFLAGS += -DAUTO_INIT_LED0

include $(RIOTBASE)/Makefile.include
