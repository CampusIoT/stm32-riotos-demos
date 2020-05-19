#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "phydat.h"
#include "saul_reg.h"
#include "saul.h"
#include "xtimer.h"
#include "assert.h"

#include "cayenne_lpp.h"


/* Each channel (1 byte) uniquely identify a sensor in the device */
#define CHANNEL_MAGNETIC_FIELD_X    0x10
#define CHANNEL_MAGNETIC_FIELD_Y    0x11
#define CHANNEL_MAGNETIC_FIELD_Z    0x12
#define CHANNEL_PRESSURE            0x20
#define CHANNEL_HUMIDITY            0x30
#define CHANNEL_TEMPERATURE         0x40

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

void lpp_dump(cayenne_lpp_t *lpp) {
    for (unsigned int i = 0; i < lpp->cursor; ++i) {
        printf("%02X ", lpp->buffer[i]);
    }
    puts("");
}

void saul_reg_read_as_float(saul_reg_t *dev, float *val) {
    phydat_t data;
    int dim = saul_reg_read(dev, &data);
    check_saul_reg_error(dim);
    for (int i = 0; i < dim; i++) {
        val[i] = data.val[i] * pow(10, data.scale);

        // DEBUG only
        printf("Value: %f = %d * 10^%d\n", val[i], data.val[i], data.scale);
    }
}

int main(void)
{
    puts("Demo package Cayenne Low Power Payload");
    puts("Read from sensors and generate payload");

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

    cayenne_lpp_t lpp = { 0 };

    float mag_values[3];
    float press_value;
    float hum_value;
    float temp_value;
    saul_reg_read_as_float(mag, mag_values);
    saul_reg_read_as_float(press, &press_value);
    saul_reg_read_as_float(hum, &hum_value);
    saul_reg_read_as_float(temp, &temp_value);


    /*
     * Magnetometer data is not yet supported by Cayenne protocol
     * Here we use 3 digital output to store the data
     */
    cayenne_lpp_add_analog_input(&lpp, CHANNEL_MAGNETIC_FIELD_X, mag_values[0]);
    cayenne_lpp_add_analog_input(&lpp, CHANNEL_MAGNETIC_FIELD_Y, mag_values[1]);
    cayenne_lpp_add_analog_input(&lpp, CHANNEL_MAGNETIC_FIELD_Z, mag_values[2]);

    cayenne_lpp_add_barometric_pressure(&lpp, CHANNEL_PRESSURE, press_value);
    cayenne_lpp_add_relative_humidity(&lpp, CHANNEL_HUMIDITY, hum_value);
    cayenne_lpp_add_temperature(&lpp, CHANNEL_TEMPERATURE, temp_value);
    

    lpp_dump(&lpp);

    return 0;
}
