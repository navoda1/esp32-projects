#ifndef __WIFI_HANDLER_H__
#define __WIFI_HANDLER_H__

#include <string>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"

#include "defines.hh"

#define WIFI_CONNECTED_BIT      BIT0
#define WIFI_FAILED_BIT         BIT1

class WifiHandler {

    public:
        WifiHandler();
        ~WifiHandler();

        status_t Connect(const char* ssid, const char* password);

    private:
        static void _event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
        void _initialize();

        static EventGroupHandle_t event_group;
        static int num_retries;

        const char *ssid;
        const char *password;
        static esp_netif_ip_info_t ip_info;
    
        esp_event_handler_instance_t instance_any_id;
        esp_event_handler_instance_t instance_got_ip;

        static constexpr const char *tag = "WifiHandler";
        static const int c_max_retries = 5;

};

#endif /* __WIFI_HANDLER_H__ */