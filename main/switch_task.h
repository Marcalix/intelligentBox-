/*
 * switch_task.h
 *
 *  Created on: 24 janv. 2025
 *      Author: niami
 */

#ifndef MAIN_SWITCH_TASK_H_
#define MAIN_SWITCH_TASK_H_

// Bibliothèques standard
#include "Pin.h"

// Temps pour éviter les rebonds
#define DEBOUNCE_DELAY_MS 50 

// Stack Depth
#define switch_StackDepth 850 

// Priority
#define switch_Priority 2

// Structure pour définir un bouton
typedef struct {
    gpio_num_t gpio_pin;     // GPIO du bouton
    bool last_state;         // Dernier état lu (évite les rebonds)
    void (*on_press)(void);  // Callback pour action lors d'une pression
    void (*on_release)(void);// Callback pour action lors du relâchement
} switch_t;

// Variables globales 
extern switch_t Switchs[];

// Handles des tâches FreeRTOS 
extern TaskHandle_t switch_task_handle; 

// prototypes activation Switchs
void Switch1_Pressed(void);
void Switch1_Released(void);
void Switch2_Pressed(void);
void Switch2_Released(void);
void Switch3_Pressed(void); 
void Switch3_Released(void);
void Switch4_Pressed(void); 
void Switch4_Released(void);
void Switch5_Pressed(void); 
void Switch5_Released(void);

// Prototypes des fonctions
void Switch_PinMode(switch_t *switchs);
bool Switch_Init(void);
void Switch_Task(void *arg);
#endif /* MAIN_SWITCH_TASK_H_ */
