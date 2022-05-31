/* Reference: https://github.com/espressif/esp-idf/tree/master/examples/protocols/http_server/simple */

#include "HTTPServer.hh"

// Declare static variables
FileSystem HTTPServer::file_system;
httpd_uri_t HTTPServer::base_uri = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = get_base_uri_handler,
    .user_ctx = NULL
};

httpd_uri_t HTTPServer::javascript_uri = {
    .uri = "/scripts.js",
    .method = HTTP_GET,
    .handler = get_javascript_uri_handler,
    .user_ctx = NULL
};

httpd_uri_t HTTPServer::styles_uri = {
    .uri = "/styles.css",
    .method = HTTP_GET,
    .handler = get_styles_uri_handler,
    .user_ctx = NULL
};

HTTPServer::HTTPServer() {
    // Initialize HTTPD config
    server = NULL;
    config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Initialize the file system
    file_system = FileSystem();
}

HTTPServer::~HTTPServer() {

}

esp_err_t HTTPServer::start() {
    // Start the server
    ESP_LOGI(tag, "Starting server on port: %d", config.server_port);
    
    esp_err_t status = httpd_start(&server, &config);

    if (status == ESP_OK) {
        // Register URI handlers
        ESP_LOGI(tag, "Registering URI handlers");

        // Base URL
        httpd_register_uri_handler(server, &base_uri);
        httpd_register_uri_handler(server, &javascript_uri);
        httpd_register_uri_handler(server, &styles_uri);

        ESP_LOGI(tag, "URI handler registration succesfull. Server started...");
    }
    else {
        ESP_LOGE(tag, "Could not start the server");
    }

    return status;
}

esp_err_t HTTPServer::get_base_uri_handler(httpd_req_t *req) {
    char *resp_str = file_system.read_file("/spiffs/index.html");

    httpd_resp_set_type(req, "text/html");
    // httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
    httpd_resp_sendstr_chunk(req, resp_str);
    httpd_resp_sendstr_chunk(req, NULL);

    return ESP_OK;
}

esp_err_t HTTPServer::get_javascript_uri_handler(httpd_req_t *req) {
    char *resp_str = file_system.read_file("/spiffs/scripts.js");

    ESP_LOGI(tag, "js_file: %s", resp_str);

    httpd_resp_set_type(req, "application/javascript");
    httpd_resp_sendstr_chunk(req, resp_str);
    httpd_resp_sendstr_chunk(req, NULL);

    return ESP_OK;
}

esp_err_t HTTPServer::get_styles_uri_handler(httpd_req_t *req) {
    char *resp_str = file_system.read_file("/spiffs/styles.css");

    httpd_resp_set_type(req, "text/css");
    httpd_resp_sendstr_chunk(req, resp_str);
    httpd_resp_sendstr_chunk(req, NULL);

    return ESP_OK;
}