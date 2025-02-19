#include "buzzer_ldr_task.h"

// ----------------------------------------- Buzzer Part --------------------------------------------------------------

// Handle de la tâche pour le buzzer
TaskHandle_t buzzer_task_handle;

// Paramètres par défaut pour le buzzer
buzzer_task_params_t buzzer_params = {
    .gpio_pin = BUZZER_GPIO_PIN,     // GPIO par défaut pour le buzzer
    .blink_period = 500,         // Période de clignotement (500 ms)
    .mode = BUZZER_OFF           // Mode par défaut : éteint
};

// Fonction pour gérer le buzzer
void Buzzer_Task(void *arg) {
    buzzer_task_params_t *params = (buzzer_task_params_t *)arg;

    // Configurer le GPIO du buzzer comme sortie
    gpio_set_direction(params->gpio_pin, GPIO_MODE_OUTPUT);
    
    while (1) {
        switch (params->mode) {
            case BUZZER_OFF:
                gpio_set_level(params->gpio_pin, 0); // Éteindre le buzzer
                vTaskDelay(pdMS_TO_TICKS(100));      // Attente pour éviter la surcharge
                break;

            case BUZZER_ON:
                gpio_set_level(params->gpio_pin, 1); // Allumer le buzzer
                vTaskDelay(pdMS_TO_TICKS(100));      // Attente pour éviter la surcharge
                break;
                
			case BUZZER_BLINK:
                gpio_set_level(params->gpio_pin, 1); // Allumer le buzzer
                vTaskDelay(pdMS_TO_TICKS(params->blink_period)); // Attente ON

                gpio_set_level(params->gpio_pin, 0); // Éteindre le buzzer
                vTaskDelay(pdMS_TO_TICKS(params->blink_period)); // Attente OFF
                break;
                
            default:
                break;
        }
    }
}

// Fonction d'initialisation pour le buzzer
bool Buzzer_Init(void) {
    BaseType_t buzzer_task_status;

    // Création de la tâche pour gérer le buzzer
    buzzer_task_status = xTaskCreate(
        Buzzer_Task,                // Fonction de la tâche
        "buzzer_task",              // Nom de la tâche
         buzzer_StackDepth,   // Taille de la pile (minimale pour un buzzer)
        (void *)&buzzer_params,     // Paramètres pour la tâche
        buzzer_Priority,                          // Priorité
        &buzzer_task_handle         // Handle de la tâche
    );

    // Vérifier si la tâche a été créée avec succès
    if (buzzer_task_status != pdPASS) {
        return false;
    }

    return true;
}

// ----------------------------------------- Ldr Part --------------------------------------------------------------
// Handle pour la tâche LDR
TaskHandle_t ldr_task_handle;

// Dernière valeur lue du LDR
static int ldr_value = 0;

// Configuration des paramètres LDR
ldr_task_params_t ldr_params = {
    .adc_channel = ADC_CHANNEL_7,   // Canal ADC pour GPIO_NUM_8
    .adc_width = ADC_BITWIDTH_12,  // Résolution 12 bits
    .adc_atten = ADC_ATTEN_DB_12,   // Atténuation 12 dB
    .adc_handle = NULL             // Initialisé lors de LDR_Init
};

// Tâche pour surveiller le LDR
void LDR_Task(void *arg) {
    ldr_task_params_t *params = (ldr_task_params_t *)arg;
	
    while (1) {
        // Lire la valeur brute de l'ADC
        adc_oneshot_read(params->adc_handle, params->adc_channel, &ldr_value);
        vTaskDelay(pdMS_TO_TICKS(150)); // Lecture toutes les secondes
    }
}

// Fonction d’initialisation de l’LDR
bool LDR_Init() {
    BaseType_t task_status;
    
	gpio_set_pull_mode(LDR_GPIO_PIN, GPIO_FLOATING);
	
    // Configuration de l'unité ADC
    adc_oneshot_unit_init_cfg_t adc_unit_cfg = {
        .unit_id = ADC_UNIT_2, // Utilisation de l'ADC 1 ADC_UNIT_1
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    esp_err_t ret = adc_oneshot_new_unit(&adc_unit_cfg, &ldr_params.adc_handle);
    if (ret != ESP_OK) {
        return false;
    }

    // Configuration du canal ADC
    adc_oneshot_chan_cfg_t channel_cfg = {
        .bitwidth = ldr_params.adc_width,
        .atten = ldr_params.adc_atten
    };
    ret = adc_oneshot_config_channel(ldr_params.adc_handle, ldr_params.adc_channel, &channel_cfg);
    if (ret != ESP_OK) {
        return false;
    }

    // Créer la tâche LDR
    task_status = xTaskCreate(
        LDR_Task,               // Fonction de la tâche
        "ldr_task",             // Nom de la tâche
        ldr_StackDepth, // Taille de la pile
        (void *)&ldr_params,    // Paramètres pour la tâche
        Ldr_Priority,                      // Priorité
        &ldr_task_handle        // Handle de la tâche
    );

    if (task_status != pdPASS) {
        return false;
    }

    return true;
}

// Fonction pour obtenir la dernière valeur lue du LDR
int get_ldr(void) {
    return ldr_value;
}
