/* Reference: https://github.com/espressif/esp-idf/tree/master/examples/wifi/getting_started/station */

#include "WifiHandler.hh"

EventGroupHandle_t WifiHandler::event_group;
int WifiHandler::num_retries;
esp_netif_ip_info_t WifiHandler::ip_info;

WifiHandler::WifiHandler() {

    // Initialize NETIF layer and Wifi
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&init_cfg));

    // Initialize the event group
    event_group = xEventGroupCreate();

    // Set up event handlers for Wifi and IP events to monitor connection status
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &_event_handler,
        NULL,
        &instance_any_id
    ));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        IP_EVENT,
        IP_EVENT_STA_GOT_IP,
        &_event_handler,
        NULL,
        &instance_got_ip
    ));

}

WifiHandler::~WifiHandler() {
    
}


void WifiHandler::_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT) {
        if (event_id == WIFI_EVENT_STA_START) {
            // Ready to connect
            esp_wifi_connect();
        }
        else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
            // Failed, try again until retries are over
            if (num_retries < c_max_retries) {
                esp_wifi_connect();
                num_retries++;
                ESP_LOGI(tag, "Retrying to connect (%d)", num_retries);
            }
            else {
                xEventGroupSetBits(event_group, WIFI_FAILED_BIT);
            }
        }
    }
    else if (event_base == IP_EVENT) {
        if (event_id == IP_EVENT_STA_GOT_IP) {
            // Successfully connected and received an IP 
            ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
            ip_info = event->ip_info;
            ESP_LOGI(tag, "Received IP: " IPSTR, IP2STR(&ip_info.ip));
            num_retries = 0;
            xEventGroupSetBits(event_group, WIFI_CONNECTED_BIT);
        }
    }
}

void WifiHandler::Connect(const char* ssid, const char* password) {

    this->ssid = ssid;
    this->password = password;

    // Set the wifi configurations
    wifi_config_t wifi_config = {};
    memcpy(wifi_config.sta.ssid, ssid, strlen(ssid)); 
    memcpy(wifi_config.sta.password, password, strlen(ssid)); 

    // Initiate the conenction process
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(tag, "Wifi connection initiated...");

    // Wait for the result from the wifi module
    EventBits_t bits = xEventGroupWaitBits(
        event_group,
        WIFI_CONNECTED_BIT | WIFI_FAILED_BIT,
        pdFALSE,
        pdFALSE,
        portMAX_DELAY
    );

    // Process the result
    switch(bits) {
        case WIFI_CONNECTED_BIT:
            ESP_LOGI(tag, "Connection successful to Wifi with SSID: %s", ssid);
            break;
        case WIFI_FAILED_BIT:
            ESP_LOGE(tag, "Failed to connect to Wifi with SSID: %s", ssid);
            break;
        default:
            ESP_LOGE(tag, "Unexpected event bit");
            break;
    }

}