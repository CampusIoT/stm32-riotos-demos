#include <stdio.h>
#include <stdlib.h>
#include "board.h"

#include "periph/gpio.h"

int cpt = 0;

static void gpio_falling(void *arg)
{
	(void)arg;
	LED0_TOGGLE;
	printf("cpt=%d\n", ++cpt);
}

int main(void)
{
	LED0_OFF;

	/* initialize on-board LED1 */
	gpio_init(LED0_PIN, GPIO_OUT);

	/* initialize on-board user button */
#ifdef BTN_B1_PIN
	gpio_init_int(BTN_B1_PIN, GPIO_IN, GPIO_FALLING, gpio_falling, NULL);
#else
	gpio_init_int(BTN0_PIN, GPIO_IN, GPIO_FALLING, gpio_falling, NULL);
#endif

	printf("Press button 0\n");
}
