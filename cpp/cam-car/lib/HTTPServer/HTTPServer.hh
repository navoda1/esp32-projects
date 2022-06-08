#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include <string>
#include "esp_log.h"
#include "esp_vfs.h"
#include "esp_http_server.h"
#include "cJSON.h"

#include "defines.hh"
#include "FileSystem.hh"

#define SCRATCH_BUFSIZE 10240

typedef struct rest_server_context {
    char base_path[ESP_VFS_PATH_MAX + 1];
    char scratch[SCRATCH_BUFSIZE];
} user_context_t;

class HTTPServer {

    public:
        HTTPServer();
        ~HTTPServer();

        esp_err_t start();

    private:
        static constexpr const char *tag = "HTTPServer";

        httpd_handle_t server;
        httpd_config_t config;

        static FileSystem file_system;
        
        static httpd_uri_t common_get_uri;
        static httpd_uri_t dc_motor_uri;
        static httpd_uri_t servo_motor_uri;

        static esp_err_t get_common_get_handler(httpd_req_t *req);
        static esp_err_t get_motor_uri_handler(httpd_req_t *req);
    
};

#endif /* __HTTP_SERVER_H__ */