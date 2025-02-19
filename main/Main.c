#include "Main.h"
#include "Mysql_task.h"
#include "Wifi_task.h"
#include "buzzer_ldr_task.h"
#include "esp_wifi.h"
#include <stdbool.h>


// -------------------------------------------------- Champs non Modifiable ------------------------------------------------
CircularBuffer_t CircularBuffer;

void Loop(void){
	char response_buffer[12];
	
	while(1){
		// ------------------------------- Test True -----------------------------------------------
		send_data(get_sht3x().temperature,get_sht3x().humidity, get_ldr());  // Envoi des données avec passage de paramètres
		vTaskDelay(pdMS_TO_TICKS(100));
		//response_buffer = read_data();
		//read_data(response_buffer, sizeof(response_buffer));  // Lecture des données
		
		if (strcmp(response_buffer, "1") == 0) {led2_params.mode = LED_MODE_BLINK;}
		else if (strcmp(response_buffer, "2") == 0) {buzzer_params.mode = BUZZER_BLINK;}
		else {
			buzzer_params.mode = BUZZER_OFF;
			led2_params.mode = LED_MODE_OFF;
		}
		vTaskDelay(pdMS_TO_TICKS(100));
		
		}
	}

void app_main(void) {
	//wifi_init(true, "Vitabox", "12345678");
	circular_buffer_init(&CircularBuffer); // initalisation du buffer
  	I2C_Master_Init(); // I2c initialisation
  	I2s_Master_Init(); // I2s initialisation
  	
  	Led_Init();       // Node Led
  	Buzzer_Init();    // Node Buzzer
	Switch_Init();    // Node Switch
    LDR_Init();       // Node Ldr
    sht3x_Init();     // Node Sht3x
    Im72d128_Init(); // Node Im72d128
  	wifi_init("S25 de Marc-Alix", "283hih67p5d4euq"); // Node wifi
  	
  	// Création de la tâche de filtrage
    //xTaskCreate(Filter_task, "FilterTask", 4096, NULL, 5, NULL);
    
    Loop();
  
}


// Fonction Pin
void Switch1_Pressed(){ buzzer_params.mode = BUZZER_ON;}
void Switch1_Released(){buzzer_params.mode = BUZZER_OFF;}

void Switch2_Pressed(){esp_wifi_stop(); buzzer_params.mode = BUZZER_BLINK;}//buzzer_params.mode = BUZZER_BLINK;
void Switch2_Released(){}//buzzer_params.mode = BUZZER_OFF;

void Switch3_Pressed(){esp_wifi_start(); buzzer_params.mode = BUZZER_BLINK;}//led2_params.mode = LED_MODE_ON;
void Switch3_Released(){}

void Switch4_Pressed(){}//led2_params.mode = LED_MODE_OFF;
void Switch4_Released(){}

void Switch5_Pressed(){led2_params.mode = LED_MODE_BLINK;}
void Switch5_Released(){led2_params.mode = LED_MODE_OFF;}
