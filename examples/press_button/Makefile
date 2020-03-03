APPLICATION = example

BOARD ?= native

RIOTBASE ?= $(CURDIR)/../RIOT/

DEVELHELP ?= 1

USEMODULE += saul saul_default

FEATURES_REQUIRED += periph_gpio
FEATURES_REQUIRED += periph_gpio_irq

CFLAGS += -DAUTO_INIT_LED0

include $(RIOTBASE)/Makefile.include
