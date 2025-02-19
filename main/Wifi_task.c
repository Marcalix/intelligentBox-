#include "Wifi_task.h"

/*static const char *TAG = "WiFi_Manager";

static bool is_connected = false;   // État de connexion Wi-Fi
static int connected_devices = 0;   // Nombre de périphériques connectés en mode AP
static esp_netif_t *netif = NULL;

void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_START:
                esp_wifi_connect();
                break;
                
            case WIFI_EVENT_STA_CONNECTED:
                is_connected = true;
                break;
                
            case WIFI_EVENT_STA_DISCONNECTED:
                is_connected = false;
                //esp_wifi_connect();
                break;

            case WIFI_EVENT_AP_STACONNECTED: {
                connected_devices++;
                break;
            }

            case WIFI_EVENT_AP_STADISCONNECTED: {
                connected_devices = (connected_devices > 0) ? connected_devices - 1 : 0;
                break;
            }

            default:
                break;
        }
    }
}

void wifi_init(bool as_ap, char *ssid, char *password) {
    ESP_LOGI(TAG, "Initialisation du Wi-Fi...");
    nvs_flash_init();
    
    esp_netif_init();
    
    esp_event_loop_create_default();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL);
	
	esp_wifi_stop();
	
	wifi_config_t wifi_config = {};

    if (as_ap) {
        ESP_LOGI(TAG, "Démarrage en mode Point d'Accès...");
        strncpy((char*)wifi_config.ap.ssid, ssid, sizeof(wifi_config.ap.ssid) - 1);
        wifi_config.ap.ssid[sizeof(wifi_config.ap.ssid) - 1] = '\0';
        strncpy((char*)wifi_config.ap.password, password, sizeof(wifi_config.ap.password) - 1);
        wifi_config.ap.password[sizeof(wifi_config.ap.password) - 1] = '\0';
        wifi_config.ap.ssid_len = strlen(ssid);
        wifi_config.ap.max_connection = MAX_STA_CONN;
        wifi_config.ap.authmode = (strlen(password) == 0) ? WIFI_AUTH_OPEN : WIFI_AUTH_WPA_WPA2_PSK;

        esp_wifi_set_mode(WIFI_MODE_AP);
        esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
        
    } else {
        ESP_LOGI(TAG, "Démarrage en mode Station...");
        strncpy((char*)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid) - 1);
        wifi_config.sta.ssid[sizeof(wifi_config.sta.ssid) - 1] = '\0';
        strncpy((char*)wifi_config.sta.password, password, sizeof(wifi_config.sta.password) - 1);
        wifi_config.sta.password[sizeof(wifi_config.sta.password) - 1] = '\0';
        wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK, 

        esp_wifi_set_mode(WIFI_MODE_STA);
        esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    }
	
    esp_wifi_start();
}

bool wifi_is_connected() {
    return is_connected;
}

int wifi_get_connected_devices() {
    return connected_devices;
}

void wifi_stop() {
    esp_wifi_stop();
    esp_wifi_deinit();
    esp_event_loop_delete_default();
    esp_netif_deinit();
    nvs_flash_erase();
    
    if (netif) {
        esp_netif_destroy(netif);
        netif = NULL;
    }
}*/

//------------------------------------------------------------------------------------------------------------------------------
#include "Wifi_task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"

static const char *TAG = "WiFi_Manager";
static bool is_connected = false;
static esp_netif_t *netif = NULL;

// Gestionnaire d'événements Wi-Fi et IP
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_START:
                ESP_LOGI(TAG, "Tentative de connexion au Wi-Fi...");
                esp_wifi_connect();
                break;
                
            case WIFI_EVENT_STA_CONNECTED:
                ESP_LOGI(TAG, "Wi-Fi connecté !");
                is_connected = true;
                break;
                
            case WIFI_EVENT_STA_DISCONNECTED:
                ESP_LOGW(TAG, "Déconnexion du Wi-Fi, tentative de reconnexion...");
                is_connected = false;
                esp_wifi_connect();  // Tentative de reconnexion automatique
                break;

            default:
                break;
        }
    }

    // Événement : L'ESP32 a reçu une adresse IP
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Adresse IP obtenue: " IPSTR, IP2STR(&event->ip_info.ip));
    }
}

// Initialisation du Wi-Fi en mode Station (STA)
void wifi_init(char *ssid, char *password) {
    ESP_LOGI(TAG, "Initialisation du Wi-Fi en mode Station...");
    
    // Initialisation NVS pour stocker les données Wi-Fi
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();

    // Création de l'interface Wi-Fi en mode Station
    netif = esp_netif_create_default_wifi_sta();
    
    // Initialisation du Wi-Fi
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    
    // Enregistrement des gestionnaires d'événements Wi-Fi et IP
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL);

    // Configuration du Wi-Fi en mode Station
    wifi_config_t wifi_config = {};
    strncpy((char*)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid) - 1);
    wifi_config.sta.ssid[sizeof(wifi_config.sta.ssid) - 1] = '\0';
    strncpy((char*)wifi_config.sta.password, password, sizeof(wifi_config.sta.password) - 1);
    wifi_config.sta.password[sizeof(wifi_config.sta.password) - 1] = '\0';
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);

    // Démarrage du Wi-Fi
    esp_wifi_start();
}

// Vérifier si l'ESP32 est connecté au WiFi
bool wifi_is_connected() {
    return is_connected;
}

// Arrêter et nettoyer le Wi-Fi
void wifi_stop() {
    esp_wifi_stop();
    esp_wifi_deinit();
    esp_event_loop_delete_default();
    esp_netif_deinit();
    nvs_flash_erase();
    
    if (netif) {
        esp_netif_destroy(netif);
        netif = NULL;
    }
    ESP_LOGI(TAG, "Wi-Fi arrêté.");
}

