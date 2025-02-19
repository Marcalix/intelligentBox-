/*
 * CircularBuffer.h
 *
 *  Created on: 26 janv. 2025
 *      Author: niami
 */

#ifndef MAIN_CIRCULARBUFFER_H_
#define MAIN_CIRCULARBUFFER_H_

#define BUFFER_SIZE 1024 // Taille du buffer pour les échantillons audio

#define QUEUE_SIZE 2 // Taille de la file d'attente

#define FILTER_TAP_NUM  500 //150 //Taille du filtre

#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/ringbuf.h"
#include <stdbool.h>
#include <string.h>

// Structure pour le buffer circulaire
typedef struct {
    RingbufHandle_t ringbuf;      // Handle pour le RingBuffer
    QueueHandle_t queue;          // Handle pour la file d'attente
} CircularBuffer_t;

// Variable global
extern int16_t filter_taps[FILTER_TAP_NUM];

// Prototypes des fonctions
bool circular_buffer_init(CircularBuffer_t *cb);
bool circular_buffer_write(CircularBuffer_t *cb, int16_t *data, size_t length, int delay);
bool circular_buffer_read(CircularBuffer_t *cb, int16_t *data, size_t length, int delay);

#endif /* MAIN_CIRCULARBUFFER_H_ */
