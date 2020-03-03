APPLICATION = example

BOARD ?= native

RIOTBASE ?= $(CURDIR)/../RIOT/

DEVELHELP ?= 1

USEMODULE += saul saul_default lsm6dso

CFLAGS += -DAUTO_INIT_LED0 -DAUTO_INIT_LSM6DSO

include $(RIOTBASE)/Makefile.include
