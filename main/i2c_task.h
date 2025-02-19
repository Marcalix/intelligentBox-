/*
 * i2c_task.h
 *
 *  Created on: 25 janv. 2025
 *      Author: niami
 */

#ifndef MAIN_I2C_TASK_H_
#define MAIN_I2C_TASK_H_

// Bibliothèques standard
#include "Sht3x_Driver.h"

// Stack Depth
#define Sht3x_StackDepth 4600 

// Priority
#define Sht3x_Priority 4

// Définition des modes de fonctionnement pour les LEDs
typedef enum {  
    SHT3X_ON,
    SHT3X_OFF,
    SHT3X_RESET      
} sht3x_mode_t;

// Structure pour les paramètres du St3x
typedef struct {
    int reset_gpio_pin;
    int alert1_gpio_pin;         
    bool alert1_state;     
    sht3x_mode_t mode;   
} sht3x_task_params_t;

// Variables globales
extern sht3x_task_params_t st3x_params;

// Handles des tâches FreeRTOS 
extern TaskHandle_t sht3x_task_handle;

// Prototypes des fonctions i2c
bool I2C_Master_Init(void);
bool I2C_Check_Device(uint8_t address);

// Prototypes des fonctions Sht3x
void Sht3x_Task(void *arg);
bool sht3x_Init(void);
sht3x_sensors_values_t get_sht3x(void);

#endif /* MAIN_I2C_TASK_H_ */
