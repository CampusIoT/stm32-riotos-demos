APPLICATION=lorawan-ds75lx

BOARD ?= im880b

# Default radio driver is Semtech SX1272 (used by the IMST iM880ab boards)
DRIVER ?= sx1272

# Default region is Europe and default band is 868MHz
REGION ?= EU868

USEMODULE += fmt
USEMODULE += xtimer
USEMODULE += ds75lx
USEMODULE += $(DRIVER)

USEPKG += cayenne-lpp

USEPKG += semtech-loramac
USEMODULE += semtech_loramac_rx

# SECRET should be changed and kept secret
SECRET ?= cafebabe02000001cafebabe02ffffff

DEVEUI ?= cafebabe02000001
APPEUI ?= cafebabe02ffffff
APPKEY ?= cafebabe02000001cafebabe02ffffff

FEATURES_REQUIRED += periph_rtc

USEMODULE += hashes

DEVELHELP ?= 1

RIOTBASE ?= ${RIOT_BASE}
# RIOTBASE ?= ~/github/RIOT-OS/RIOT

CFLAGS += -DREGION_$(REGION)
CFLAGS += -DLORAMAC_ACTIVE_REGION=LORAMAC_REGION_$(REGION)

CFLAGS += -DFORGE_DEVEUI_APPEUI_APPKEY -DSECRET=\"$(SECRET)\"
# CFLAGS += -DDEVEUI=\"$(DEVEUI)\" -DDEVEUI=\"$(DEVEUI)\" -DAPPEUI=\"$(APPEUI)\" -DAPPKEY=\"$(APPKEY)\"


include $(RIOTBASE)/Makefile.include
