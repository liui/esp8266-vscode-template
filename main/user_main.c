/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_wifi.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_system.h"
#include "esp_wifi_osi.h"
#include "esp_event_loop.h"

static esp_err_t wifi_event_callback(void* ctx, system_event_t *event)
{
    if (event->event_id == SYSTEM_EVENT_AP_STACONNECTED)
    {
        printf("new sta client connected.\n");
    }
    else if (event->event_id == SYSTEM_EVENT_STA_START)
    {
        esp_wifi_connect();        
    }
    else if (event->event_id == SYSTEM_EVENT_STA_CONNECTED)
    {
        
    }
    else if (event->event_id == SYSTEM_EVENT_STA_GOT_IP)
    {
        printf("sta connected: %s\n", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
    }
    else if (event->event_id == SYSTEM_EVENT_STA_LOST_IP)
    {
        esp_wifi_disconnect();      
    }
    else if (event->event_id == SYSTEM_EVENT_STA_DISCONNECTED)
    {  
        esp_wifi_connect();      
    }
    return ESP_OK;
}

/******************************************************************************
 * FunctionName : app_main
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void app_main(void)
{
    UART_SetBaudrate(UART0, 115200);
    UART_SetPrintPort(UART0);

    wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&init_cfg);

    esp_event_loop_init(wifi_event_callback, NULL);

    esp_wifi_set_mode(WIFI_MODE_APSTA);

    wifi_config_t wifi_config;
 
    memset(&wifi_config, 0, sizeof(wifi_config));
    wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    wifi_config.ap.max_connection = 3;
    strcpy(wifi_config.ap.ssid, "vscode");
    esp_wifi_set_config(WIFI_IF_AP, &wifi_config);

    printf("SDK version:%s\n", esp_get_idf_version());
}
