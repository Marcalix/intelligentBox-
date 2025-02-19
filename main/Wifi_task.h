/*
 * Wifi_task.h
 *
 *  Created on: 5 févr. 2025
 *      Author: niami
 */

#ifndef MAIN_WIFI_TASK_H_
#define MAIN_WIFI_TASK_H_

// Bibliothèques standard
#include "Pin.h"

// Nombre maximal d'appareils connectés en mode AP
#define MAX_STA_CONN 4

//void wifi_init(bool as_ap, char *ssid, char *password);
void wifi_init(char *ssid, char *password);
bool wifi_is_connected();
int wifi_get_connected_devices();
void wifi_stop();

#endif /* MAIN_WIFI_TASK_H_ */
