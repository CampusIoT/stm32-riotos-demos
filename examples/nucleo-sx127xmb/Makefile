APPLICATION=lorawan-nucleo

BOARD ?= nucleo

# Default radio driver is Semtech SX1272 (used by the IMST iM880ab boards)
DRIVER ?= sx1272

# Default region is Europe and default band is 868MHz
REGION ?= EU868

USEMODULE += xtimer
USEMODULE += $(DRIVER)

USEPKG += semtech-loramac
USEMODULE += semtech_loramac_rx

USEMODULE += hashes

# USEPKG += cayenne-lpp

DEVELHELP ?= 1

RIOTBASE ?= ${RIOT_BASE}

CFLAGS += -DREGION_$(REGION)
CFLAGS += -DLORAMAC_ACTIVE_REGION=LORAMAC_REGION_$(REGION)

include $(RIOTBASE)/Makefile.include
