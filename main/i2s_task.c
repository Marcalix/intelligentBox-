#include "i2s_task.h"

TaskHandle_t im72d128_task_handle;

i2s_chan_handle_t rx_handle;


// Initialisation du microphone
esp_err_t I2s_Master_Init(void) {
	i2s_chan_config_t chan_rx_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    esp_err_t ret;

    // Configuration PDM pour le microphone
    i2s_pdm_rx_config_t pdm_rx_cfg = {
		
  		//.clk_cfg = I2S_PDM_RX_CLK_DEFAULT_CONFIG(I2S_SAMPLE_RATE),
        .clk_cfg = {
		    .sample_rate_hz = I2S_SAMPLE_RATE,  // Fréquence d'échantillonnage (48 kHz)
		    .clk_src = I2S_CLK_SRC_DEFAULT,  // Source d'horloge par défaut (PLL interne ESP32)
		    .mclk_multiple = 64,
		    .bclk_div = 2,
		    .dn_sample_mode = I2S_PDM_DSR_8S   // Mode de décimation avancé
			},
        .slot_cfg = I2S_PDM_RX_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH, I2S_SLOT_MODE_MONO), 
		.gpio_cfg = {
        	.clk = I2S_SCK_PIN,
        	.din = I2S_SD_PIN,
        	.invert_flags = {
            	.clk_inv = false,
        	},
    	},	
    };
			
	// Initialisation du canal PDM pour la réception
	ret = i2s_new_channel(&chan_rx_cfg, NULL, &rx_handle);
	if (ret != ESP_OK) { return ret;}
	
	ret = i2s_channel_init_pdm_rx_mode(rx_handle, &pdm_rx_cfg);
	if (ret != ESP_OK) { return ret; }
    		
    // Activer le canal I2S
    ret = i2s_channel_enable(rx_handle);
    if (ret != ESP_OK) { return ret; }

    return ESP_OK;
 }


// Lecture des données audio
esp_err_t I2s_Read(int16_t *buffer, size_t buffer_size) {
    size_t bytes_read = 0;

    esp_err_t ret = i2s_channel_read(rx_handle, buffer, buffer_size * sizeof(int16_t), &bytes_read, portMAX_DELAY);
    if (ret != ESP_OK) { return ret; }
    return ESP_OK;
}

// Désactivation et libération des ressources I2S
esp_err_t I2s_Clean(void) {
    esp_err_t ret = i2s_channel_disable(rx_handle);
    if (ret != ESP_OK) { return ret; }

    ret = i2s_del_channel(rx_handle);
    if (ret != ESP_OK) { return ret; }

    return ESP_OK;
}

void Im72d128_Task(void *arg){
	gpio_set_direction(I2S_WS_PIN, GPIO_MODE_OUTPUT);
	gpio_set_level(I2S_WS_PIN, 1);
	int16_t audio_buffer[BUFFER_SIZE];
	
	while (1) {
		I2s_Read(audio_buffer, BUFFER_SIZE);
		circular_buffer_write(&CircularBuffer, (int16_t *)audio_buffer, sizeof(audio_buffer), 1);
		vTaskDelay(pdMS_TO_TICKS(1)); // Petit délai pour éviter la surcharge 1
	}
}

bool Im72d128_Init(void){
	BaseType_t im72d128_task_status;

    // Création de la tâche pour surveiller les Switchs
    im72d128_task_status = xTaskCreate(
        Im72d128_Task,                // Fonction de la tâche
        "Im72d128_Task",                  // Nom de la tâche
        Im72d128_StackDepth,         // Taille de la pile
        NULL,                        // Paramètres pour la tâche
        Im72d128_Priority,             // Priorité
        &im72d128_task_handle      // Handle de la tâche
    );

    // Vérifier si la tâche a été créée
    if (im72d128_task_status != pdPASS) {return false;}
    return true;
}
