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

static const char *TAG = "example";
//const gpio_num_t ledPin = GPIO_NUM_2; //2023-10-20 23:29:33 - Built in LED for ESP32 
const gpio_num_t ledPin = GPIO_NUM_5; //2023-10-20 23:51:09 -  Relay 5v branché sur D5 (pin 5)


extern "C" void app_main(void)
{

cout<<MY_SSID<<":"<<MY_PASSWORD<<endl;

    short s_led_state = 1;

    while (true) {
        ESP_LOGI(TAG, "PIN is %d!", (int) 8);
        ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");

    // Configuration de la broche GPIO en mode sortie
        gpio_config_t io_conf;
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.mode = GPIO_MODE_OUTPUT;
        io_conf.pin_bit_mask = (1ULL << ledPin);
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
        gpio_config(&io_conf);

        // Allume la LED
        gpio_set_level(ledPin, s_led_state);

        // Vous pouvez également éteindre la LED en utilisant gpio_set_level(ledPin, 0);


        /* Toggle the LED state */
        s_led_state = !s_led_state;
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
    }    
}
