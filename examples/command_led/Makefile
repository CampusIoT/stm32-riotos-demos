APPLICATION=command_led

BOARD ?= native

RIOTBASE ?= $(CURDIR)/../RIOT

# Driver for the Riot shell
USEMODULE += shell

# Needed to enable user LED
CFLAGS += -DAUTO_INIT_LED0

include $(RIOTBASE)/Makefile.include
