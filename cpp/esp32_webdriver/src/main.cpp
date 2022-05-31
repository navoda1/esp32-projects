#include "esp_log.h"

#include "WifiHandler.hh"
#include "HTTPServer.hh"

extern "C" {
    void app_main(void);
}

const char *tag = "main";

void app_init() {
    // Initialize the Non-Volatile Storage for storing configurations
    ESP_ERROR_CHECK(nvs_flash_init());

    // Initialize NETIF layer
    ESP_ERROR_CHECK(esp_netif_init());

    // Initialize the default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());
}

void app_main() {

    // Init the common components in the app
    app_init();

    // Initialize the WifiHandler and connect to Wifi
    WifiHandler wifi_handler = WifiHandler();
    wifi_handler.Connect("Not a 2G Wifi!", "connectanyway");

    HTTPServer server = HTTPServer();
    ESP_ERROR_CHECK(server.start());

}