#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include "esp_log.h"
#include "esp_http_server.h"

#include "FileSystem.hh"

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
        static httpd_uri_t base_uri;
        static httpd_uri_t javascript_uri;
        static httpd_uri_t styles_uri;

        static esp_err_t get_base_uri_handler(httpd_req_t *req);
        static esp_err_t get_javascript_uri_handler(httpd_req_t *req);
        static esp_err_t get_styles_uri_handler(httpd_req_t *req);
    
};

#endif /* __HTTP_SERVER_H__ */