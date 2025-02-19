# include "i2c_task.h"
#include "Pin.h"
#include "hal/gpio_types.h"

bool I2C_Master_Init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    // Configurer les paramètres I2C
    esp_err_t ret = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (ret != ESP_OK) {return false;}

    // Installer le pilote I2C
    ret = i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    if (ret != ESP_OK) {return false;}

    return true;
}

// Fonction pour vérifier si un périphérique I2C est présent à une adresse spécifique
bool I2C_Check_Device(uint8_t address) {
    // Créer une commande I2C
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_stop(cmd);

    // Envoyer la commande et attendre la réponse
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    // Retourner true si le périphérique répond, sinon false
    return (ret == ESP_OK);
}

// ------------------------------------------------------- Sht3x Task ------------------------------------------------------------------------
TaskHandle_t sht3x_task_handle;

char scale = SCALE_CELCIUS;

// Paramètres par défaut pour le buzzer
sht3x_task_params_t st3x_params = {
    .reset_gpio_pin = RESET_GPIO_PIN,
    .alert1_gpio_pin = ALERT1_GPIO_PIN,         
    .alert1_state = false,     
    .mode = SHT3X_ON           
};

static sht3x_sensors_values_t sensors_values = {
            .temperature = 0x00,
            .humidity = 0x00
        };
        
static void sht3x_PinMode(sht3x_task_params_t *Sht3x) {
	   gpio_set_direction(Sht3x->reset_gpio_pin, GPIO_MODE_OUTPUT);
       gpio_set_pull_mode(Sht3x->reset_gpio_pin, GPIO_PULLUP_ONLY);
      
       gpio_set_direction(Sht3x->alert1_gpio_pin, GPIO_MODE_INPUT);
       gpio_set_pull_mode(Sht3x->alert1_gpio_pin, GPIO_FLOATING);   
}

static void sht3x_Reset(sht3x_task_params_t *Sht3x) {
	   switch (Sht3x->mode) {
            case SHT3X_OFF:
   				gpio_set_level(Sht3x->reset_gpio_pin, 0);
                break;

            case SHT3X_ON:
                gpio_set_level(Sht3x->reset_gpio_pin, 1);
                break;
                
			case SHT3X_RESET:
      			gpio_set_level(Sht3x->reset_gpio_pin, 0);
                vTaskDelay(pdMS_TO_TICKS(0.5));

                gpio_set_level(Sht3x->reset_gpio_pin, 1);
                vTaskDelay(pdMS_TO_TICKS(0.5));
                break;
                
            default:
                break;
        }
    }   


void Sht3x_Task(void *arg) {
	sht3x_task_params_t *params = (sht3x_task_params_t *)arg;
    
    sht3x_PinMode(params);
    
    #if defined(SENSORS_SCALE_F)
    scale = SCALE_FAHRENHEIT;
    #elif defined(SENSORS_SCALE_K)
    scale = SCALE_KELVIN;
    #endif

    vTaskDelay(pdMS_TO_TICKS(10));

    while(1) {
		
		sht3x_Reset(params);
		
        sht3x_start_periodic_measurement();

        vTaskDelay(pdMS_TO_TICKS(1000));

        sht3x_read_measurement(&sensors_values); 
            
        vTaskDelay(pdMS_TO_TICKS(5));

        #if defined(SENSORS_SCALE_F)
        sensors_values.temperature = FAHRENHEIT(sensors_values.temperature);
        #elif defined(SENSORS_SCALE_K)
        sensors_values.temperature = KELVIN(sensors_values.temperature);
        #endif
		
		params->alert1_state = gpio_get_level(params->alert1_gpio_pin);
		
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

bool sht3x_Init(void) {
    BaseType_t sht3x_task_status;
	
    // Création de la tâche pour surveiller les Switchs
    sht3x_task_status = xTaskCreate(
         Sht3x_Task,                // Fonction de la tâche
        "Sht3x_Task",                   // Nom de la tâche
        Sht3x_StackDepth,          // Taille de la pile
        (void *)&st3x_params,                     // Paramètres pour la tâche
        Sht3x_Priority,            // Priorité
        &sht3x_task_handle         // Handle de la tâche
    );

    // Vérifier si la tâche a été créée
    if (sht3x_task_status != pdPASS) {
        return false;
    }
    return true;
}

sht3x_sensors_values_t get_sht3x(void) {
	return sensors_values;
}

