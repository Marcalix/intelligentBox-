/*
 * Sht3x_Driver.h
 *
 *  Created on: 25 janv. 2025
 *      Author: niami
 */

#ifndef MAIN_SHT3X_DRIVER_H_
#define MAIN_SHT3X_DRIVER_H_

// Bibliothèques standard
#include "Pin.h"

#define FAHRENHEIT(celcius)         (((celcius * 9.0) / 5.0) + 32.0)
#define KELVIN(celcius)             (celcius + 273.15)
#define SCALE_CELCIUS               ('C')
#define SCALE_FAHRENHEIT            ('F')
#define SCALE_KELVIN                ('K')

#define I2C_ACK_CHECK_DIS           (0x00)
#define I2C_ACK_CHECK_EN            (0x01)
#define I2C_ACK_VAL                 (0x00)
#define I2C_NACK_VAL                (0x01)

#define SHT3X_SENSOR_ADDR           (0x44)
#define SHT3X_READ_ERROR            (0xFFFF)
#define SHT3X_HEX_CODE_SIZE         (0x02)

#define CRC8_POLYNOMIAL             (0x31)
#define CRC8_INIT                   (0xFF)

// Data Structure
typedef struct sht3x_msb_lsb {
    uint8_t msb;
    uint8_t lsb;
} sht3x_msb_lsb_t;

typedef struct sht3x_sensor_value {
    sht3x_msb_lsb_t value;
    uint8_t crc;
} sht3x_sensor_value_t;

typedef struct sht3x_sensors_values {
    float temperature;
    float humidity;
} sht3x_sensors_values_t;

// Prototypes des fonctions
uint8_t sht3x_generate_crc(const uint8_t* data, uint16_t count);
esp_err_t sht3x_send_command(uint8_t *command);
esp_err_t sht3x_read(uint8_t *hex_code, uint8_t *measurements, uint8_t size);
esp_err_t sht3x_write(uint8_t *hex_code, uint8_t *measurements, uint8_t size);
esp_err_t sht3x_send_command_and_fetch_result(uint8_t *command, uint8_t *measurements, uint8_t size);
esp_err_t sht3x_start_periodic_measurement();
esp_err_t sht3x_start_periodic_measurement_with_art();
esp_err_t sht3x_read_measurement(sht3x_sensors_values_t *sensors_values);
esp_err_t sht3x_stop_periodic_measurement();
esp_err_t sht3x_soft_reset();
esp_err_t sht3x_general_call_reset();
esp_err_t sht3x_enable_heater();
esp_err_t sht3x_disable_heater();
esp_err_t sht3x_read_status_register(sht3x_sensor_value_t *sensors_value);
esp_err_t sht3x_clear_status_register();


#endif /* MAIN_SHT3X_DRIVER_H_ */
