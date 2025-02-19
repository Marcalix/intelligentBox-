/*
 * i2s_task.h
 *
 *  Created on: 26 janv. 2025
 *      Author: niami
 */

#ifndef MAIN_I2S_TASK_H_
#define MAIN_I2S_TASK_H_

// Bibliothèques standard
#include "Pin.h"


// Stack Depth
#define Im72d128_StackDepth 14500

// Priority
#define Im72d128_Priority 6


// Variables globales 


// Handles des tâches FreeRTOS 
extern TaskHandle_t im72d128_task_handle; 


// Prototypes des fonctions i2s
esp_err_t I2s_Master_Init(void); // Initialisation du microphone
esp_err_t I2s_Read(int16_t *buffer, size_t buffer_size); // Lecture des données audio du microphone
esp_err_t I2s_Clean(void); // Désactivation et libération des ressources I2S

// Prototypes des fonctions im72d128
void Im72d128_Task( void *arg);
bool Im72d128_Init(void);

#endif /* MAIN_I2S_TASK_H_ */
