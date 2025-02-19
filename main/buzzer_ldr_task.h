/*
 * buzzer_ldr_task.h
 *
 *  Created on: 24 janv. 2025
 *      Author: niami
 */

#ifndef MAIN_BUZZER_LDR_TASK_H_
#define MAIN_BUZZER_LDR_TASK_H_

// Bibliothèques standard
#include "Pin.h"

// Stack Depth
#define buzzer_StackDepth 780
#define ldr_StackDepth 880

// Priority
#define buzzer_Priority 2
#define Ldr_Priority 3

// Définir les états pour le buzzer
typedef enum {
    BUZZER_OFF,    // Éteindre le buzzer
    BUZZER_ON,     // Allumer le buzzer
    BUZZER_BLINK   // Faire clignoter le buzzer
} buzzer_mode_t;

// Structure pour les paramètres du buzzer
typedef struct {
    int gpio_pin;         // Numéro du GPIO pour le buzzer
    int blink_period;     // Période de clignotement (en ms)
    buzzer_mode_t mode;   // Mode de fonctionnement du buzzer
} buzzer_task_params_t;

// Structure pour les paramètres du LDR
// Définition de la structure des paramètres du LDR
typedef struct {
    adc_channel_t adc_channel;      // Canal ADC
    adc_bitwidth_t adc_width;       // Résolution (largeur des bits)
    adc_atten_t adc_atten;          // Atténuation (ADC_ATTEN_DB_0 à ADC_ATTEN_DB_11)
    adc_oneshot_unit_handle_t adc_handle; // Handle pour l'unité ADC
} ldr_task_params_t;

// Variables globales 
extern buzzer_task_params_t buzzer_params;
extern ldr_task_params_t ldr_params;

// Handles des tâches FreeRTOS 
extern TaskHandle_t buzzer_task_handle;
extern TaskHandle_t ldr_task_handle; 

// Prototypes des fonctions
void Buzzer_Task(void *arg);       // Tâche pour gérer le buzzer
bool Buzzer_Init(void);            // Initialisation de la tâche du buzzer
void LDR_Task(void *arg);
bool LDR_Init(void);
int get_ldr(void);


#endif /* MAIN_BUZZER_LDR_TASK_H_ */
