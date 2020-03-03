#include <stdio.h>
#include <stdlib.h>
#include "board.h"

#include "phydat.h"
#include "saul_reg.h"
#include "saul.h"

#include "periph/gpio.h"

static void gpio_rising(void *arg)
{
	(void)arg;
	LED0_TOGGLE;
}

int main(void)
{
	LED0_OFF;

	/* initialize on-board LED1 */
	gpio_init(LED0_PIN, GPIO_OUT);

	/* initialize on-board user button */
	gpio_init_int(BTN0_PIN, GPIO_IN, GPIO_RISING, gpio_rising, NULL);
}
