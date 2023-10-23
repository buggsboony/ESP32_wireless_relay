#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/event_groups.h"
#include <string.h>
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_h/esp_iotools.h" //2023-08-16 17:25:12 - to use functions : replaceStr()





#include "private.cpp"
#define ESP_WIFI_SSID      MY_SSID
#define ESP_WIFI_PASSWORD  MY_PASSWORD
#define NB_RESEAUX_MAX      16

/*
    Exemples :
    - https://github.com/espressif/esp-idf/blob/8bc19ba893e5544d571a753d82b44a84799b94b1/examples/wifi/getting_started/station/main/station_example_main.c
    - https://github.com/espressif/esp-idf/blob/8bc19ba893e5544d571a753d82b44a84799b94b1/examples/wifi/scan/main/scan.c
*/

static const char *TAG = "Test Wifi";
static bool demandeConnexion = false;



//2023-08-01 01:27:04 - Pattern buzzer
typedef struct gpio_job_params
{
    bool state;
    gpio_job_params():state(0){}
}gpio_job_params;

//2023-10-22 21:18:44 - Task code
void gpio_job(void *pvParameters) 
{
puts("task_example waits...\n");
vTaskDelay(5000 / portTICK_PERIOD_MS);
puts("Task exit");
vTaskDelete(NULL);
}//gpio_job

  
//2023-10-22 19:32:19 - RelayTask, GPIO JOB
void start_gpio_job(string on_off, short duration=1000)
{
    gpio_job_params params;
    params.state=0;
    if(on_off=="ON"){ params.state = 1; 
    }
    xTaskCreate(gpio_job, "gpio_job", 2048, &params, 5, NULL);
}//start_gpio_job



//---------------------- Web server Vars 
#include "web_http_post.h"
static httpd_handle_t server = NULL;



// Gestionnaire d'évènements
static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) 
    {
        if(demandeConnexion)
        {
            ESP_LOGI(TAG, "Connexion ...");
            esp_wifi_connect();
        }
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) 
    {
        // reconnexion ?
        ESP_LOGI(TAG, "Deconnexion !");
    } 
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Adresse IP : " IPSTR, IP2STR(&event->ip_info.ip));
    }
}//end event_handler

static esp_err_t init_wifi()
{
    // initialise la pile TCP/IP
    ESP_ERROR_CHECK(esp_netif_init());
    // crée une boucle d'évènements
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    // crée 
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    // initialise la configuration Wifi par défaut 
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    // initialise le WiFi avec la configuration par défaut et démarre également la tâche WiFi.
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
   
    // installe le gestionnaire d'évènements Wifi
    /*
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip));
    */
    // version obsolète !
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    //ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL)); Works Perfect
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
    

    // définit le mode de fonctionnement station pour le WiFi
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // démarre le WiFi selon la configuration
    esp_err_t ret = esp_wifi_start();
    ESP_ERROR_CHECK(ret);

    return ret;
}

static bool scan_wifi()
{
    bool present = false;
    uint16_t number = NB_RESEAUX_MAX;
    wifi_ap_record_t ap_info[NB_RESEAUX_MAX];
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));

    // démarre le scan
    ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));

    ESP_LOGI(TAG, "Nb reseaux trouves = %u", ap_count);
    for (int i = 0; (i < NB_RESEAUX_MAX) && (i < ap_count); i++)
    {
        // cf. https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_wifi.html?highlight=wifi_ap_record_t#_CPPv416wifi_ap_record_t
        ESP_LOGI(TAG, "SSID \t\t%s", ap_info[i].ssid);
        ESP_LOGI(TAG, "RSSI \t\t%d", ap_info[i].rssi);
        ESP_LOGI(TAG, "Channel \t\t%d\n", ap_info[i].primary);
        if(strcmp((char *)ap_info[i].ssid, ESP_WIFI_SSID) == 0)
            present = true;
    }

    return present;
}

static void restart_wifi()
{
    ESP_ERROR_CHECK(esp_wifi_stop());
    ESP_ERROR_CHECK(esp_wifi_start());
}

static void connect_wifi()
{
    // configure la connexion Wifi du point d'accès (AP)
    wifi_config_t wifi_config = {
        .sta = {
        .ssid = ESP_WIFI_SSID,
        .password = ESP_WIFI_PASSWORD,
        //.threshold.authmode = WIFI_AUTH_WPA2_PSK,
        .pmf_cfg = {
            .capable = true,
            .required = false
            },
        },
    };    
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config)); //ESP_IF_WIFI_STA not working
    demandeConnexion = true;
    restart_wifi();
}





//Launch Web Service HTTP
TaskHandle_t webTaskHandle;
void webTask(void *pvParameters)
{    
    ESP_LOGI(TAG, "webTask() !");
 

    //// Send notification to waiting task    
    //xTaskNotifyGive(mainTaskHandle);
    //delete task
    //vTaskDelete(NULL);  
}//config task

extern "C" void app_main()
{
    // initialize NVS (nécessaire pour le Wifi)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
    {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    if(init_wifi() == ESP_OK)
    {
        if(scan_wifi())
        {
            connect_wifi(); //EventHandler Will Start Web Serveur ON IP allocated
        }
    }
















    vTaskDelay(pdMS_TO_TICKS(8000));
}