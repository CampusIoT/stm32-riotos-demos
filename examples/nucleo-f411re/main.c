#include <stdio.h>
#include <stdlib.h>

#include "phydat.h"
#include "saul_reg.h"
#include "saul.h"
#include "xtimer.h"
#include "assert.h"

/**
 * Check error code returned from saul_reg read/write
 */
void check_saul_reg_error(int res)
{
    if (res == -ENODEV) {
        puts("Device is invalid");
    }
    else if (res == -ENOTSUP) {
        puts("Read operation is not supported by device");
    }
    else if (res == -ECANCELED) {
        puts("Device error");
    }
}

/**
 * Read boolean value from a device
 */
bool saul_reg_read_bool(saul_reg_t *dev)
{
    phydat_t data;
    int dim = saul_reg_read(dev, &data);
    check_saul_reg_error(dim);

    /* data should be only 1 dimension and unit boolean */
    assert(dim == 1);
    assert(data.unit == UNIT_BOOL);

    return (bool) data.val[0];
}

/**
 * Write a boolean value to a device
 */
void saul_reg_write_bool(saul_reg_t *dev, bool value) {
    /* Create comprehensible data format */
    phydat_t data;
    const int32_t values[] = { value };
    phydat_fit(&data, values, 1);

    /* Write data to device */
    int dim = saul_reg_write(dev, &data);
    check_saul_reg_error(dim);
    
    /* Only on element should be processed */
    assert(dim == 1);
} 

/**
 * Check if button is pressed
 */
bool is_pressed(saul_reg_t *btn)
{
    return saul_reg_read_bool(btn);
}


/**
 * Toggle LED
 */
void toggle_led(saul_reg_t *led)
{
    bool value = saul_reg_read_bool(led);
    saul_reg_write_bool(led, !value);
}

int main(void)
{
    puts("Demo NUCLEO-F411RE");
    puts("Push user button to toggle LED");

    /* User LED is registered as ON/OFF switch */
    saul_reg_t *led = saul_reg_find_type(SAUL_ACT_SWITCH);
    printf("Found device: %s\n", led->name);

    /* User button */
    saul_reg_t *button = saul_reg_find_type(SAUL_SENSE_BTN);
    printf("Found device: %s\n", button->name);

    while (1) {

        /* Toggle LED if button is pressed */
        if (is_pressed(button)) {
            toggle_led(led);
        }

        /* Sleep for 0.1 sec */
        xtimer_usleep(100000);
    }

    return 0;
}
