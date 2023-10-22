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
#define WIFI_SSID MY_SSID
#define WIFI_PASS MY_PASSWORD






//----------------------
#include "esp_http_server.h"

httpd_handle_t server = NULL;

esp_err_t root_handler(httpd_req_t *req)
{
    httpd_resp_send(req, "Bienvenue sur le serveur web ESP32 !", -1);
    return ESP_OK;
}

void start_webserver()
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    // Configure les routes et les gestionnaires
    httpd_uri_t root_uri = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = root_handler,
        .user_ctx = NULL
    };

    // Démarre le serveur HTTP
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &root_uri);
    }
}

void stop_webserver()
{
    httpd_stop(server);
}
//----------------------




 //vTaskDelay(pdMS_TO_TICKS(1000));
extern "C" void app_main(void)
{

    //2023-10-21 16:58:22 - Wifi needs NVS

    //2023-06-28 17:34:56 - Clear Eprom Memory
    // nvs_flash_deinit();
    // esp_err = nvs_flash_erase();
    // if( esp_err != ESP_OK ) ESP_ERROR_CHECK_WITHOUT_ABORT(esp_err);

    // Initialize the NVS partition
    esp_err_t esp_err = nvs_flash_init();
    // ESP_OK if storage was successfully initialized
    // ESP_ERR_NVS_NO_FREE_PAGES if the NVS storage contains no empty pages (which may happen if NVS partition was truncated)
    // ESP_ERR_INVALID_ARG in case partition is NULL
    // ESP_ERR_NO_MEM in case memory could not be allocated for the internal structures
    // one of the error codes from the underlying flash storage driver
    if( esp_err == ESP_OK )
    {
        puts("nvs_flash_init OK");
        // Example of nvs_get_stats() to get the number of used entries and free entries:
        nvs_stats_t nvs_stats;
        esp_err = nvs_get_stats(NULL, &nvs_stats);
        if( esp_err == ESP_OK )
        {
            puts("nvs_get_stats OK");
        }
        else
        {
            printf("Error nvs_get_stats (%x) :%s\n",esp_err, esp_err_to_name(esp_err) );
            ESP_ERROR_CHECK_WITHOUT_ABORT(esp_err);
            puts("exit program !");
            //ESP_ERROR_CHECK(esp_err);
        }
    }//ESP_ok for nvs




   // Configuration du gestionnaire de logs
    esp_log_level_set(TAG, ESP_LOG_INFO);

    // Configuration de la structure de configuration WiFi
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Démarrage de l'interface WiFi en mode station
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

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
    while (true) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        if (esp_wifi_connect() == ESP_OK) {
            wifi_mode_t mode;
            esp_wifi_get_mode(&mode);

            if (mode == WIFI_MODE_STA) {
                wifi_ap_record_t ap_info;
                esp_wifi_sta_get_ap_info(&ap_info);
                if (ap_info.rssi >= -60) {
                    ESP_LOGI(TAG, "Connecté à %s", WIFI_SSID);

                    //2023-10-22 02:28:10 - Get IP Address
                    // esp_netif_ip_info_t ip_info;
                    // esp_err_t err = esp_netif_get_ip_info(ESP_IF_WIFI_STA, &ip_info);
                    // if (err == ESP_OK) {
                    //     ESP_LOGI(TAG, "Connecté à %s", WIFI_SSID);
                    //     ESP_LOGI(TAG, "Adresse IP: %s", esp_ip4addr_ntoa(&ip_info.ip));                      
                    // }
                    break;
                }
            }
        }
    }




    


    // Démarrage du serveur web
    //start_webserver();


    vTaskDelay(pdMS_TO_TICKS(6000));
} //End main
