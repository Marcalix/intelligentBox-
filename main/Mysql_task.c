#include "Mysql_task.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "esp_http_client.h"
#include "esp_log.h"
#include "cJSON.h"

#define SERVER_URL "http://192.168.141.122:5000/data" // Remplace par l'IP de ton PC http://192.168.141.122:5000

static const char *TAG = "ESP32_HTTP";

// Fonction pour envoyer des données (POST)
void send_data(float temperature, float humidity,int luminosite) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "temperature", temperature);
    cJSON_AddNumberToObject(json, "humidity", humidity);
    cJSON_AddNumberToObject(json, "luminosite", luminosite);
    
    char *post_data = cJSON_Print(json);
    cJSON_Delete(json);

    esp_http_client_config_t config = {
        .url = SERVER_URL,
        .method = HTTP_METHOD_POST
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, post_data, strlen(post_data));

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Données envoyées: %s", post_data);
    } else {
        ESP_LOGE(TAG, "Échec de l'envoi des données");
    }

    esp_http_client_cleanup(client);
    free(post_data);
}


// Fonction pour récupérer des données (GET)
/*void read_data(char *buffer, size_t buffer_size) {
    esp_http_client_config_t config = {
        .url = SERVER_URL,
        .method = HTTP_METHOD_GET
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Content-Type", "application/json");

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        int content_length = esp_http_client_get_content_length(client);
        esp_http_client_read_response(client, buffer, buffer_size - 1);
        buffer[content_length] = '\0';

        ESP_LOGI(TAG, "Réponse du serveur : %s", buffer);
    } else {
        ESP_LOGE(TAG, "Erreur lors de la récupération des données");
    }

    esp_http_client_cleanup(client);
}*/

char* read_data() {
    esp_http_client_config_t config = {
        .url = SERVER_URL,
        .method = HTTP_METHOD_GET
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Content-Type", "application/json");

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        int content_length = esp_http_client_get_content_length(client);

        if (content_length > 0) {
            char *response = (char*) malloc(content_length + 1); // Allocation mémoire
            if (response == NULL) {
                esp_http_client_cleanup(client);
                return NULL; // Échec d'allocation
            }

            int read_bytes = esp_http_client_read_response(client, response, content_length);
            response[read_bytes] = '\0';  // Assurer la fin de chaîne

            esp_http_client_cleanup(client);
            return response;  // Retourne la réponse allouée dynamiquement
        }
    }

    esp_http_client_cleanup(client);
    return NULL;  // Retourne NULL en cas d'erreur
}



