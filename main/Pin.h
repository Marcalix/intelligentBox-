/*
 * Pin.h
 *
 *  Created on: 24 janv. 2025
 *      Author: niami
 */

#ifndef MAIN_PIN_H_
#define MAIN_PIN_H_

// Bibliothèques standard
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "soc/gpio_num.h"
#include "esp_adc/adc_oneshot.h"
#include "hal/adc_types.h"
#include "driver/i2c.h"
#include "math.h"
#include "esp_log.h"
#include "driver/i2s_common.h"
#include "hal/i2s_types.h"
#include "driver/i2s_pdm.h"
#include "freertos/ringbuf.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_wifi_types_generic.h"

// Bibliothèques 
#include "CircularBuffer.h"
#include "esp_dsp.h"
#include "dsps_fir.h"


// BUFFER GLOBAL
extern CircularBuffer_t CircularBuffer;

// Définition des GPIO
	// PROTOCOLE I2C 0
#define I2C_MASTER_SCL GPIO_NUM_10      // GPIO pour SCL
#define I2C_MASTER_SDA GPIO_NUM_9       // GPIO pour SDA
#define I2C_MASTER_NUM I2C_NUM_0        // Numéro du port I2C
#define I2C_MASTER_FREQ_HZ 100000       // Fréquence I2C
#define I2C_MASTER_TX_BUF_DISABLE 0    // Désactiver le buffer TX
#define I2C_MASTER_RX_BUF_DISABLE 0    // Désactiver le buffer RX
#define I2C_MASTER_TIMEOUT_MS 1000    // Timeout I2C

	// PROTOCOLE I2S
#define I2S_WS_PIN GPIO_NUM_6   // Word Select (WS)
#define I2S_SD_PIN GPIO_NUM_17  // Serial Data (SD)
#define I2S_SCK_PIN GPIO_NUM_7 // Serial Clock (SCK)
#define I2S_SAMPLE_RATE 48000  // Fréquence d'échantillonnage optimale (48 kHz)
#define I2S_PDM_CLK_FREQ 3072000  // Horloge PDM optimale pour SNR maximum (72 dB)
#define I2S_DATA_BIT_WIDTH I2S_DATA_BIT_WIDTH_16BIT

	// PROTOCOLE SPI 
	// LDR PIN  
#define LDR_GPIO_PIN GPIO_NUM_18            
	// BUZZER PIN
#define BUZZER_GPIO_PIN GPIO_NUM_8	
	// LED PIN 
#define LED1_GPIO_PIN GPIO_NUM_15
#define LED2_GPIO_PIN GPIO_NUM_16
	// SWITCH BUTTON PIN
#define SW1_GPIO_PIN GPIO_NUM_13
#define SW2_GPIO_PIN GPIO_NUM_14
#define SW3_GPIO_PIN GPIO_NUM_21
#define SW4_GPIO_PIN GPIO_NUM_47
#define SW5_GPIO_PIN GPIO_NUM_1
	// ALERT PIN
#define ALERT1_GPIO_PIN GPIO_NUM_4
#define RESET_GPIO_PIN GPIO_NUM_5



#endif /* MAIN_PIN_H_ */
