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
#include "driver/gpio.h" //gpio stuffs




//Timer
#include <unistd.h>
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_sleep.h"
// end timer stuff
 


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

//const gpio_num_t relayPin = GPIO_NUM_2; //GPIO_NUM_2 is Built in LED
const gpio_num_t relayPin = GPIO_NUM_27; //Relay is connected to PIN 27
#define PIN_LOW 1
#define PIN_HIGH 0  //This is inverted for Built in LED
short s_relay_state = PIN_LOW;



//2024-04-28 16:36:45 - Auto restart declarations 

int MSEC_MULT = 1000;
int SEC_MULT = 1000 * MSEC_MULT; 
int MIN_MULT = 60 * SEC_MULT;
int HOUR_MULT = 60 * MIN_MULT;
 
static void oneshot_timer_callback(void* arg);
static void oneshot_timer_callback(void* arg)
{
    int64_t time_since_boot = esp_timer_get_time();
    ESP_LOGI(TAG, "One-shot timer called, time since boot: %lld us", time_since_boot);
    ESP_LOGI(TAG, "esp_restart() !!");     
    esp_restart();
}






//2023-08-01 01:27:04 - Pattern buzzer
typedef struct gpio_job_params
{
    bool state;
    gpio_job_params():state(0){}
}gpio_job_params;

//2023-10-22 21:18:44 - Task code
void gpio_job(void *pvParameters) 
{
    int waitTime = 600; //HIGH state time.
    ESP_LOGI(TAG, "PIN is %d!", (int) relayPin);
    ESP_LOGI(TAG, "Turning the LED ON");


    // Configuration de la broche GPIO en mode sortie
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << relayPin);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);


    // Built in LED 1 for ON and 0 for Down, this is inverted 
    s_relay_state = PIN_HIGH; //ON
    gpio_set_level(relayPin, s_relay_state);


    // Vous pouvez également éteindre la LED en utilisant gpio_set_level(relayPin, 0);


    ESP_LOGI(TAG, "waitTime = %d ", waitTime);
    vTaskDelay(waitTime / portTICK_PERIOD_MS);

    // Éteindre la LED
    s_relay_state = PIN_LOW;
    gpio_set_level(relayPin, s_relay_state);
    ESP_LOGI(TAG, "Turning the LED OFF");

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
        //ESP_LOGI(TAG, "Deconnexion !");
        ESP_LOGI(TAG, "Deconnexion + reconnexion!");
        esp_wifi_connect();
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
    // // configure la connexion Wifi du point d'accès (AP)
    // wifi_config_t wifi_config = {
    //     .sta = {
    //     .ssid = ESP_WIFI_SSID,
    //     .password = ESP_WIFI_PASSWORD,
    //     //.threshold.authmode = WIFI_AUTH_WPA2_PSK,
    //     .pmf_cfg = {
    //         .capable = true,
    //         .required = false
    //         },
    //     },
    // };    
 
    
    wifi_config_t wifi_config;
    memset(&wifi_config,0,sizeof(wifi_config_t) );

    wifi_pmf_config_t pmf_cfg;
    memset(&pmf_cfg,0,sizeof(wifi_pmf_config_t) );
    pmf_cfg.capable = true;
    pmf_cfg.required = false;

 
    memset(&wifi_config.sta,0,sizeof(wifi_sta_config_t) );

    strncpy((char*)wifi_config.sta.ssid , ESP_WIFI_SSID, sizeof(wifi_config.sta.ssid ));
    strncpy((char*) wifi_config.sta.password ,ESP_WIFI_PASSWORD, sizeof( wifi_config.sta.password));

    //wifi_config.sta..threshold.authmode = WIFI_AUTH_WPA2_PSK,
    wifi_config.sta.pmf_cfg = pmf_cfg;

    
    // //2023-10-29 11:03:44 - Must use strncpy to copy array content in byte array.
    // std::ostringstream w_ssid;
    // w_ssid<<ESP_WIFI_SSID<<"\0";
    
    // strncpy((char *)sta_cfg.ssid, w_ssid.str().c_str(), sizeof(sta_cfg.ssid));    
    // std::ostringstream w_pwd;
    // w_pwd<<ESP_WIFI_PASSWORD<<"\0";
    // strncpy((char*)sta_cfg.password,w_pwd.str().c_str() , sizeof(sta_cfg.password));
    // //sta_cfg.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    // sta_cfg.pmf_cfg = pmf_cfg;
    // wifi_config.sta = sta_cfg;




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


    //2024-04-28 16:41:34 - Start restart timer --------------------------------------------------------
    const esp_timer_create_args_t oneshot_timer_args = {
            .callback = &oneshot_timer_callback,
            /* argument specified here will be passed to timer callback function */
            //.arg = (void*) periodic_timer,
            .name = "one-shot"
    };
    esp_timer_handle_t oneshot_timer;
    ESP_ERROR_CHECK(esp_timer_create(&oneshot_timer_args, &oneshot_timer));

    /* Start the timers */
    int delay = 24;
    ESP_LOGI(TAG,"timer invoke prevu dans %d mins",  delay);
    ESP_ERROR_CHECK(esp_timer_start_once(oneshot_timer, delay *  HOUR_MULT ));
    ESP_LOGI(TAG, "Started timer, time since boot: %lld us", esp_timer_get_time());
    //-------------------------------------------  END RESTART TIMER ------------------------------------------------







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