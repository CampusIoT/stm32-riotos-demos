APPLICATION=lorawan-ds75lx

BOARD ?= im880b

# Default radio driver is Semtech SX1272 (used by the IMST iM880ab boards)
DRIVER ?= sx1272

# Default region is Europe and default band is 868MHz
REGION ?= EU868

USEMODULE += xtimer
USEMODULE += ds75lx
USEMODULE += $(DRIVER)

USEPKG += semtech-loramac
USEMODULE += semtech_loramac_rx

USEPKG += cayenne-lpp

DEVELHELP ?= 1

# RIOTBASE ?= ${RIOT_BASE}
RIOTBASE ?= /Users/donsez/github/RIOT-OS/RIOT

CFLAGS += -DREGION_$(REGION)
CFLAGS += -DLORAMAC_ACTIVE_REGION=LORAMAC_REGION_$(REGION)

include $(RIOTBASE)/Makefile.include
