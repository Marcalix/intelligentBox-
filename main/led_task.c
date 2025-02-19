#include "led_task.h"

TaskHandle_t led1_task_handle;
TaskHandle_t led2_task_handle;

// Paramètres pour la LED1
	led_task_params_t led1_params = {
        .gpio_pin = LED1_GPIO_PIN,     // GPIO pour la LED
        .blink_period = Led_blink_period,          // Période de clignotement (500 ms)
        .mode = LED_MODE_BLINK        // Mode par défaut : clignotement
    };

// Paramètres pour la LED2
	led_task_params_t led2_params = {
        .gpio_pin = LED2_GPIO_PIN,     // GPIO pour la LED
        .blink_period = Led_blink_period,          // Période de clignotement (500 ms)
        .mode = LED_MODE_OFF        // Mode par défaut : clignotement
    };
  
     
void Led_Task(void *arg) {
    // Récupérer les paramètres passés à la tâche
    led_task_params_t *params = (led_task_params_t *)arg;

    gpio_set_direction(params->gpio_pin, GPIO_MODE_OUTPUT);

    while (1) {
        switch (params->mode) {
            case LED_MODE_OFF:
                gpio_set_level(params->gpio_pin, 0); // Éteindre la LED
                vTaskDelay(pdMS_TO_TICKS(100));      // Petite attente
                break;

            case LED_MODE_ON:
                gpio_set_level(params->gpio_pin, 1); // Allumer la LED
                vTaskDelay(pdMS_TO_TICKS(100));      // Petite attente
                break;

            case LED_MODE_BLINK:
                gpio_set_level(params->gpio_pin, 1); // Allumer la LED
                vTaskDelay(pdMS_TO_TICKS(params->blink_period)); // Attente ON

                gpio_set_level(params->gpio_pin, 0); // Éteindre la LED
                vTaskDelay(pdMS_TO_TICKS(params->blink_period)); // Attente OFF
                break;

            default:
                break;
        }
    }
}


bool Led_Init(void) {
    BaseType_t led1_task_status, led2_task_status;

    // Création de la tâche pour la LED1
    led1_task_status = xTaskCreate(
        Led_Task,                 // Fonction de la tâche
        "led_task1",                  // Nom de la tâche
        Led_StackDepth,                   // Taille de la pile
        (void *)&led1_params,   // Paramètres pour la tâche
        2,                       // Priorité
        &led1_task_handle                   // Handle de la tâche (non utilisé ici)
    );

    // Vérifier si la tâche LED1 a été créée
    if (led1_task_status != pdPASS) {
        return false;
    }

    // Création de la tâche pour la LED2
    led2_task_status = xTaskCreate(
        Led_Task,                     // Fonction de la tâche
        "led_task2",                      // Nom de la tâche
        Led_StackDepth,                       // Taille de la pile
        (void *)&led2_params,       // Paramètres pour la tâche
        Led_Priority,                           // Priorité
        &led2_task_handle                       // Handle de la tâche (non utilisé ici)
    );

    // Vérifier si la tâche LED2 a été créée
    if (led2_task_status != pdPASS) {
       	return false;
    }
    return true;
}