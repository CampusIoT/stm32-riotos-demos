#include <stdio.h>
#include <stdlib.h>

#include "phydat.h"
#include "saul_reg.h"
#include "saul.h"
#include "xtimer.h"
#include "assert.h"

/* TODO driver lsm6ds0 not present in RIOT */

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
 * Check device exists and print his name
 */
bool check_device(saul_reg_t *dev) {
    if (dev == NULL) {
        puts("Device not found");
    }
    else {
        printf("Found device: %s\n", dev->name);
    }
    return dev != NULL;
}

void print_phydat_1D(const char *prefix, phydat_t *data)
{
    const char *unit = phydat_unit_to_str(data->unit);
    const char scale_prefix = phydat_prefix_from_scale(data->scale);

    printf("%s: ", prefix);
    printf("%d %c%s", data->val[0], scale_prefix, unit);
    printf("\n");
}

void print_phydat_3D(const char *prefix, phydat_t *data)
{
    const char* axis[] = {"X", "Y", "Z"};

    printf("%s: ", prefix);
    for (unsigned int i = 0; i < PHYDAT_DIM; i++) {
        printf("[%s: %d %s] ", axis[i], data->val[i], phydat_unit_to_str(data->unit));
    }
    printf("\n");
}

/**
 * Read data and dump data from device
 */
void saul_reg_dump(saul_reg_t *dev, const char *prefix) {
    phydat_t data;
    int dim = saul_reg_read(dev, &data);
    check_saul_reg_error(dim);
    puts(prefix);
    phydat_dump(&data, dim);
}

int main(void)
{
    puts("Demo X-NUCLEO-IKS01A1");
    puts("Read and show data for all MEMS sensors");

    /* Get all sensors */
    saul_reg_t *accel = saul_reg_find_type(SAUL_SENSE_ACCEL);
    saul_reg_t *mag = saul_reg_find_type(SAUL_SENSE_MAG);
    saul_reg_t *press = saul_reg_find_type(SAUL_SENSE_PRESS);
    saul_reg_t *hum = saul_reg_find_type(SAUL_SENSE_HUM);
    saul_reg_t *temp = saul_reg_find_type(SAUL_SENSE_TEMP);

    /* Verify and print each devices name */
    check_device(accel); 
    check_device(mag);
    check_device(press);
    check_device(hum);
    check_device(temp);

    /* Read data from each device */
    /* saul_reg_dump(accel, "Acceleration"); */
    saul_reg_dump(mag, "Magnetometer");
    saul_reg_dump(press, "Pressure");
    saul_reg_dump(hum, "Humidity");
    saul_reg_dump(temp, "Temperature");

    return 0;
}
