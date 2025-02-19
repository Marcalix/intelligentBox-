/*
 * Mysql_task.h
 *
 *  Created on: 6 févr. 2025
 *      Author: niami
 */

#ifndef MAIN_MYSQL_TASK_H_
#define MAIN_MYSQL_TASK_H_

// Bibliothèques standard
#include "Pin.h"

void send_data(float temperature, float humidity, int luminosite);
//void read_data(char *buffer, size_t buffer_size);
char* read_data();

#endif /* MAIN_MYSQL_TASK_H_ */
