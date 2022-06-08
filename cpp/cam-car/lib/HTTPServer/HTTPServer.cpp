/* Reference: https://github.com/espressif/esp-idf/tree/master/examples/protocols/http_server/simple */

#include "HTTPServer.hh"

/* Declare static variables */
FileSystem HTTPServer::file_system;
user_context_t *req_content;

/* URI handler for the base URI */
httpd_uri_t HTTPServer::common_get_uri = {
    .uri = "/*",
    .method = HTTP_GET,
    .handler = get_common_get_handler,
    .user_ctx = NULL
};

/* URI handler for the DC Motor API */
httpd_uri_t HTTPServer::dc_motor_uri = {
    .uri = "/api/dc-motor",
    .method = HTTP_POST,
    .handler = get_motor_uri_handler,
    .user_ctx = NULL
};

/* URI handler for the Servo Motor API */
httpd_uri_t HTTPServer::servo_motor_uri = {
    .uri = "/api/servo-motor",
    .method = HTTP_POST,
    .handler = get_motor_uri_handler,
    .user_ctx = NULL
};

HTTPServer::HTTPServer() {
    /* Initialize HTTPD config */
    server = NULL;
    config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;
    config.uri_match_fn = httpd_uri_match_wildcard;

    /* Initialize the file system */
    file_system = FileSystem();
}

HTTPServer::~HTTPServer() {

}

esp_err_t HTTPServer::start() {
    /* Start the server */
    ESP_LOGI(tag, "Starting server on port: %d", config.server_port);
    
    esp_err_t status = httpd_start(&server, &config);

    if (status == ESP_OK) {
        /* Register URI handlers */
        ESP_LOGI(tag, "Registering URI handlers");

        httpd_register_uri_handler(server, &common_get_uri);
        httpd_register_uri_handler(server, &dc_motor_uri);
        httpd_register_uri_handler(server, &servo_motor_uri);

        ESP_LOGI(tag, "URI handler registration succesfull. Server started...");
    }
    else {
        ESP_LOGE(tag, "Could not start the server");
    }

    return status;
}

esp_err_t HTTPServer::get_common_get_handler(httpd_req_t *req) {

    std::string file_ext;
    std::string response_type;
    std::string uri;
    size_t ext_start;

    /* Get the URI */
    uri = req->uri;

    /* If the URI is for the base path, append index.html */
    if (uri.compare("/") == 0) {
        uri += "index.html";
    }

    /* Find the start position of the file extension */
    ext_start = uri.find(".");

    /* Get the file extension and set the response type based on that */
    if (ext_start != std::string::npos) {
        file_ext = uri.substr(ext_start, uri.length());

        if (file_ext.compare(".html") == 0) {
            response_type = "text/html";
        }
        else if (file_ext.compare(".js") == 0) {
            response_type = "application/javascript";
        }
        else if (file_ext.compare(".css") == 0) {
            response_type = "text/css";
        }
    }

    std::string file_path = file_system.get_root_path() + uri;
    std::string response = file_system.read_file(file_path);

    /* If the file doesn't exist, return 404 */
    if (response.empty()) {
        httpd_resp_send_404(req);
        return ESP_ERR_HTTPD_INVALID_REQ;
    }

    /* Respond with the file and file type */
    httpd_resp_set_type(req, response_type.c_str());
    httpd_resp_sendstr_chunk(req, response.c_str());
    httpd_resp_sendstr_chunk(req, NULL);

    return ESP_OK;
}

esp_err_t HTTPServer::get_motor_uri_handler(httpd_req_t *req) {

    int total_len = req->content_len;
    int curr_len = 0;
    int received = 0;
    char buf[200];
    std::string uri;
    motor_type_t motor_type;

    /* Get the URI */
    uri = req->uri;

    ESP_LOGI(tag, "uri: %s | %d", uri.c_str(), uri.find("dc"));

    /* Get motor type */
    motor_type = (uri.find("dc") != std::string::npos) ? dc : servo;

    /* Receive the direction data */
    while (curr_len < total_len) {
        received = httpd_req_recv(req, buf + curr_len, total_len);
        if (received <= 0) {
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to post control value");
            return ESP_FAIL;
        }
        curr_len += received;
    }

    /* Terminate the data with a null byte */
    buf[total_len] = '\0';

    /* Extract json data */
    cJSON *data = cJSON_Parse(buf);
    std::string direction = cJSON_GetObjectItem(data, "direction")->valuestring;

    ESP_LOGI(tag, "Received %d direction: %s", motor_type, direction.c_str());

    /* Return success */
    httpd_resp_sendstr(req, "success");

    return ESP_OK;
}

