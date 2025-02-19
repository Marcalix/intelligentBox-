/*
 * led_task.h
 *
 *  Created on: 24 janv. 2025
 *      Author: niami
 */

#ifndef LED_TASK_H
#define LED_TASK_H

// Bibliothèques standard
#include "Pin.h"


// Stack Depth
#define Led_StackDepth 760

// Priority
#define Led_Priority 2

// blink_period
#define Led_blink_period 500

// Définition des modes de fonctionnement pour les LEDs
typedef enum {
    LED_MODE_OFF,    // Éteindre la LED
    LED_MODE_ON,     // Allumer la LED en continu
    LED_MODE_BLINK   // Faire clignoter la LED
} led_mode_t;

// Structure pour les paramètres des LEDs
typedef struct {
    gpio_num_t gpio_pin;    // Numéro du GPIO pour la LED
    uint32_t blink_period;  // Période de clignotement en millisecondes
    led_mode_t mode;        // Mode de fonctionnement de la LED
} led_task_params_t;

// Variables globales pour les paramètres des LEDs
extern led_task_params_t led1_params;
extern led_task_params_t led2_params;

// Handles des tâches FreeRTOS pour les LEDs
extern TaskHandle_t led1_task_handle;
extern TaskHandle_t led2_task_handle;

// Déclaration des fonctions
void Led_Task(void *arg);     // Fonction pour gérer une LED
bool Led_Init(void);     // Fonction pour initialiser les LEDs

#endif // LED_TASK_H

