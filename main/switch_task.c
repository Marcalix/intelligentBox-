#include "switch_task.h"

TaskHandle_t switch_task_handle;

// Tableau des Switchs
switch_t Switchs[] = {
    {SW1_GPIO_PIN, false, Switch1_Pressed, Switch1_Released},
    {SW2_GPIO_PIN, false, Switch2_Pressed, Switch2_Released},
    {SW3_GPIO_PIN, false, Switch3_Pressed, Switch3_Released},
    {SW4_GPIO_PIN, false, Switch4_Pressed, Switch4_Released},
    {SW5_GPIO_PIN, false, Switch5_Pressed, Switch5_Released}
};

// Taille dynamique du tableau des Switchs
size_t Switchs_Size = sizeof(Switchs) / sizeof(Switchs[0]);

void Switch_PinMode(switch_t *Switch) {
    for (size_t i = 0; i < Switchs_Size; i++) {
        gpio_set_direction(Switch[i].gpio_pin, GPIO_MODE_INPUT);
        gpio_set_pull_mode(Switch[i].gpio_pin, GPIO_PULLUP_ONLY);
        Switch[i].last_state = false;
    }
}

// Tâche pour surveiller l'état des boutons
void Switch_Task(void *arg) {
    switch_t *Switch = (switch_t *)arg;
	Switch_PinMode(Switch);
	
    while (1) {
        for (size_t i = 0; i < Switchs_Size; i++) {
            bool current_state = !gpio_get_level(Switch[i].gpio_pin); // Bouton pressé si niveau bas

            if (current_state != Switch[i].last_state) {
                vTaskDelay(pdMS_TO_TICKS(DEBOUNCE_DELAY_MS)); // Délais pour le debounce

                // Re-vérifier après debounce
                current_state = !gpio_get_level(Switch[i].gpio_pin);
                if (current_state != Switch[i].last_state) {
                    Switch[i].last_state = current_state;

                    if (current_state && Switch[i].on_press) {
                        Switch[i].on_press();
                    } else if (!current_state && Switch[i].on_release) {
                        Switch[i].on_release();
                    }
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // Petit délai pour éviter la surcharge
    }
}

// Initialisation des Switchs
bool Switch_Init(void) {
    BaseType_t switch_task_status;

    // Création de la tâche pour surveiller les Switchs
    switch_task_status = xTaskCreate(
        Switch_Task,                // Fonction de la tâche
        "Switch_Task",              // Nom de la tâche
        switch_StackDepth,          // Taille de la pile
        (void *)Switchs,            // Paramètres pour la tâche
        switch_Priority,                          // Priorité
        &switch_task_handle         // Handle de la tâche
    );

    // Vérifier si la tâche a été créée
    if (switch_task_status != pdPASS) {
        return false;
    }
    return true;
}

