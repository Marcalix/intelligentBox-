#include "filters_task.h"


// Filtre
fir_s16_t filter;

// Tache 
TaskHandle_t filter_task_handle;

// Buffers pour stockage des données
static int16_t audio_data [BUFFER_SIZE];
static int16_t audio_filtred [BUFFER_SIZE];
static float audio_normalized[BUFFER_SIZE]; 
static int16_t delay_line[FILTER_TAP_NUM] = {0};
//static float filtered_buffer[BUFFER_SIZE];
//static float fft_input[FFT_SIZE];
//static float fft_output[FFT_SIZE];

// -----------------------------------------------------------------------------------------------------------------------------------------
void normalize_to_minus_one_one(float *input, float *output, int size) {
    if (size <= 0 || !input || !output) return;

    float max_val = 0.0f;

    // Trouver la valeur maximale absolue
    for (int i = 0; i < size; i++) {
        float abs_val = fabsf(input[i]);
        if (!isfinite(abs_val)) continue; // Ignore NaN et Inf
        if (abs_val > max_val) max_val = abs_val;
    }

    // Appliquer la normalisation [-1,1]
    if (max_val > 1.1754944e-38f) { // FLT_MIN ≈ 1.1754944e-38
        float inv_max = 1.0f / max_val;
        for (int i = 0; i < size; i++) {
            if (!isfinite(input[i])) {
                output[i] = 0.0f; // Remplace NaN/Inf par 0
            } else {
                output[i] = input[i] * inv_max;
            }
        }
    } else {
        for (int i = 0; i < size; i++) {
            output[i] = 0.0f;
        }
    }
}


// -----------------------------------------------------------------------------------------------------------------------------------------

void Filter_task(void *arg){
	dsps_fird_init_s16(&filter, filter_taps, delay_line, FILTER_TAP_NUM, 1, 0, 0);
	
	while (1) {
		circular_buffer_read( &CircularBuffer, audio_data, sizeof(audio_data), 1);
        //ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        
        // Appliquer le filtrage FIR
    	dsps_fird_s16_ansi(&filter, audio_data, audio_filtred, BUFFER_SIZE);
    	
        //xTaskNotifyGive(task_handle_fft);
        
        // Appliquer la normalisation
        normalize_to_minus_one_one((float_t*) audio_filtred, audio_normalized, BUFFER_SIZE);
        
        for (int i = 0; i < BUFFER_SIZE; i++) {
                printf("%f\n", audio_normalized[i]);
            }
            vTaskDelay(pdMS_TO_TICKS(200));
    }
}
