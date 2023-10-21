/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"
#include "private.cpp"

#include <iostream>
using namespace std;



//2023-10-20 23:32:29 - C++ ESP32 Blink 

static const char *TAG = "Wireless Relay";

//2023-10-21 14:18:30 - Wifi stuff
#include "wifi_station.h"




//const gpio_num_t ledPin = GPIO_NUM_2; //2023-10-20 23:29:33 - Built in LED for ESP32 
const gpio_num_t ledPin = GPIO_NUM_5; //2023-10-20 23:51:09 -  Relay 5v branché sur D5 (pin 5)


// Remplacez ces valeurs par les informations de votre réseau Wi-Fi
#define WIFI_SSID "NomDuReseau"
#define WIFI_PASS "MotDePasseDuReseau"


extern "C" void app_main(void)
{
// Configuration du gestionnaire de logs
    esp_log_level_set(TAG, ESP_LOG_INFO);

    // Configuration de l'événement de boucle par défaut pour gérer les événements WiFi
    esp_event_loop_init(NULL, NULL);

    // Configuration de la structure de configuration WiFi
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    // Démarrage de l'interface WiFi en mode station
    esp_wifi_set_mode(WIFI_MODE_STA);

    // Configuration des informations d'authentification du réseau WiFi
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Connexion au réseau WiFi...");

    // Attente de la connexion au réseau WiFi
    ESP_ERROR_CHECK(esp_wifi_connect());

    // Boucle principale
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
} //End main
