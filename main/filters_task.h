/*
 * filters_task.h
 *
 *  Created on: 30 janv. 2025
 *      Author: niami
 */

#ifndef MAIN_FILTERS_TASK_H_
#define MAIN_FILTERS_TASK_H_

// Bibliothèques standard
#include "Pin.h"


// Stack Depth
#define filter_StackDepth 8090 // 23000

// Priority
#define filter_Priority 5


// Variables globales 


// Handles des tâches FreeRTOS 
extern TaskHandle_t filter_task_handle; 


// Prototypes des fonctions 
void Filter_task(void *arg);


#endif /* MAIN_FILTERS_TASK_H_ */
