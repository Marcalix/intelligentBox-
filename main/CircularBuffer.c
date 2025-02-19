#include "CircularBuffer.h"

// ------------------------------------------------- Design Buffer -------------------------------------------------------------------

// Initialisation du buffer circulaire
bool circular_buffer_init(CircularBuffer_t *cb) {
    // Crée un ring buffer de type byte
    cb->ringbuf = xRingbufferCreate(BUFFER_SIZE, RINGBUF_TYPE_BYTEBUF);
    if (cb->ringbuf == NULL) {
        return false;
    }

    // Crée une file d'attente pour 3 éléments maximum
    cb->queue = xQueueCreate(QUEUE_SIZE, sizeof(int16_t) * BUFFER_SIZE);  // Taille de chaque item = BUFFER_SIZE
    if (cb->queue == NULL) {
        vRingbufferDelete(cb->ringbuf);
        return false;
    }

    return true;
}

// Écrire dans le buffer circulaire avec gestion de la file d'attente
bool circular_buffer_write(CircularBuffer_t *cb, int16_t *data, size_t length, int delay) {
    // Essayez d'écrire dans le RingBuffer
    if (xRingbufferSend(cb->ringbuf, data, length, pdMS_TO_TICKS(delay)) == pdPASS) {return true;}

    // Si le RingBuffer est plein, essayez d'ajouter les données à la file d'attente
    if (xQueueSend(cb->queue, data, pdMS_TO_TICKS(delay)) == pdPASS) { return true;}

    return false;
}


// Lire depuis le buffer circulaire ou la file d'attente
bool circular_buffer_read(CircularBuffer_t *cb, int16_t *data, size_t length, int delay) {
    size_t received_length;

    // Vérifiez d'abord si des données sont disponibles dans la file d'attente
    if (xQueueReceive(cb->queue, data, pdMS_TO_TICKS(delay)) == pdTRUE) { return true;}


    // Sinon, essayez de lire depuis le RingBuffer
uint8_t *temp_data = (uint8_t *)xRingbufferReceive(cb->ringbuf, &received_length, pdMS_TO_TICKS(delay));
    if (temp_data != NULL) {
        // Vérifiez si la taille lue correspond à ce qui est attendu
        if (received_length <= length) {
            memcpy(data, temp_data, received_length);
            vRingbufferReturnItem(cb->ringbuf, temp_data);  // Libérez la mémoire
            return true;
        }
        // En cas de taille incorrecte
        vRingbufferReturnItem(cb->ringbuf, temp_data);  // Libérez la mémoire
    }

    // Rien à lire
    return false;
}


// Nettoyage des ressources
void circular_buffer_clean(CircularBuffer_t *cb) {
    if (cb->ringbuf != NULL) {
        vRingbufferDelete(cb->ringbuf);
    }
    if (cb->queue != NULL) {
        vQueueDelete(cb->queue);
    }
}

// ------------------------------------------------------- Design Filters --------------------------------------------------------------------
// Coefficients du filtre FIR (calculés avec SciPy)
int16_t filter_taps[FILTER_TAP_NUM] = {
    //#include "filter_coeffs.h"  // Inclure les coefficients générés par Python
    #include "filter_coeffs-2.h"  // Inclure les coefficients générés par Python
};


/*#define SAMPLE_RATE   44100   // Fréquence d'échantillonnage (Hz)
#define CUTOFF_LOW    300     // Fréquence de coupure basse (Hz)
#define CUTOFF_HIGH   3000    // Fréquence de coupure haute (Hz)
#define FILTER_TAP_NUM 128    // Nombre de coefficients du filtre FIR (doit être une puissance de 2)

// Déclaration du filtre
fir_s16_t filter;
TaskHandle_t filter_task_handle;

// Buffers pour stockage des données
static int16_t audio_data[BUFFER_SIZE];
static int16_t audio_filtred[BUFFER_SIZE];
static int16_t delay_line[FILTER_TAP_NUM] = {0};
static int16_t filter_taps[FILTER_TAP_NUM];

*
 * @brief Génération des coefficients FIR Passe-Bande avec ESP-DSP
 
void generate_fir_coefficients() {
    float normalized_low = (float)CUTOFF_LOW / (SAMPLE_RATE / 2);  // Normalisation (par rapport à Fs/2)
    float normalized_high = (float)CUTOFF_HIGH / (SAMPLE_RATE / 2); // Normalisation (par rapport à Fs/2)

    dsps_fird_s16_generate(filter_taps, FILTER_TAP_NUM, normalized_low, normalized_high, 
                           DSP_FIRD_BAND_PASS, DSP_FIRD_WIN_HAMMING);
}

*
 * @brief Génération du filtre FIR avec ESP-DSP
 
void generate_fir_coefficients() {
    float normalized_low = (float)CUTOFF_LOW / SAMPLE_RATE;  // Normalisation de la fréquence basse
    float normalized_high = (float)CUTOFF_HIGH / SAMPLE_RATE; // Normalisation de la fréquence haute

    dsps_fird_s16_generate(filter_taps, FILTER_TAP_NUM, normalized_low, normalized_high, 
                           DSP_FIRD_BAND_PASS, DSP_FIRD_WIN_HAMMING);
}*/

         
