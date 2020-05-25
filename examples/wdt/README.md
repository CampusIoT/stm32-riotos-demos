# RIOT OS :: Watchdog timer

## Watchdog timer
https://riot-os.org/api/group__drivers__periph__wdt.html

A watchdog timer (WDT) is an electronic or software timer that is used to detect and recover from unusual or suspect behaviour as well as software malfunctions. During NORMAL operation the application will reset wdt_kick() the timer counter preventing it from hitting 0. If due to software or hardware error the WDT counter isn't reset, it will trigger corrective actions, i.e. a system reboot or callback (not supported by all platforms).

This interface defines two operation modes NORMAL and WINDOW. In NORMAL operation if the WDT counter isn't reset before max_time then a reboot/callback is triggered. In WINDOW operation if a reset isn't triggered or if it's triggered outside of the time window = [min_time, max_time] a reboot is triggered.

> Attention: les STM32 ne supportent pas le mode WINDOW (min_time=0) : voir `$RIOT_BASE/cpu/stm32_common/periph/wdt.c`

## First test

Flash the test program
```bash
cd $RIOT_BASE/tests/periph_wdt
# specify your STM32 board model
BOARD=im880b
gmake BOARD=im880b flash
```

> Attention: les STM32 ne supportent pas le mode WINDOW (min_time=0) : voir `$RIOT_BASE/cpu/stm32_common/periph/wdt.c`

```
help
range
setup 0 5000
start
# wait one second and enter command :
kick
# wait one second and enter command :
kick
# wait one second and enter command :
kick
# wait one second and enter command :
kick
# wait one second and enter command :
kick
# wait one second and enter command :
kick

startloop
```

The watchdog reboots the board after 5 seconds


## Next test: reboot after OS crash
```
setup 0 5000
start
# wait one second and enter command :
kick
# wait one second and enter command :
kick
# wait one second and enter command :
kick
# wait one second and enter command :
kick
# wait one second and enter command :
kick
# wait one second and enter command :
kick

# Crash RIOT OS (assert violation) with unsupported WINDOW mode (min_time=5000)
setup 5000 10000

```

The watchdog reboots the board after 5 seconds

