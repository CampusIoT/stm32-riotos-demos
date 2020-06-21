APPLICATION=lorawan-nucleo

BOARD ?= nucleo

# Default radio driver is Semtech SX1272 (sx1276 is the other driver). 
DRIVER ?= sx1272

# Default region is Europe and default band is 868MHz.
# sx1272 supports regions such as EU868, US915, CN779, AU915, AS923, KR920, IN865, RU864
# sx1276 supports regions such as EU433, CN470, EU868, US915, CN779, AU915, AS923, KR920, IN865, RU864
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
