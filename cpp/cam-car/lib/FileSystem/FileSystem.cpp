/* Reference: https://github.com/espressif/esp-idf/tree/master/examples/storage/spiffs/main/spiffs_example_main.c */

#include "FileSystem.hh"

FileSystem::FileSystem() {
    esp_err_t status;
    
    // Initialize the config
    esp_vfs_spiffs_conf_t config = {
        .base_path = root_path,
        .partition_label = "storage",
        .max_files = 5,
        .format_if_mount_failed = true
    };

    // Register and mount SPIFFS filesystem
    status = esp_vfs_spiffs_unregister("storage");
    status = esp_vfs_spiffs_register(&config);

    // Check for errors
    if (status != ESP_OK) {
        if (status == ESP_FAIL) {
            ESP_LOGE(tag, "Failed to mount or format filesystem");
        } else if (status == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(tag, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(tag, "Failed to initialize SPIFFS: %s", esp_err_to_name(status));
        }
    }
    else {
        ESP_LOGI(tag, "Successfully initialized SPIFFS");
    }
}

FileSystem::~FileSystem() {
    
}

std::string FileSystem::read_file(std::string fname) {
    
    FILE *fp;
    long fsize;
    char *content;
    
    // Open the file
    fp = fopen(fname.c_str(), "r");
    if (fp == NULL) {
        ESP_LOGE(tag, "Failed to open the file: %s", fname.c_str());
        return "";
    }

    // Get file size
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Read the file content into a string
    content = (char *) malloc(fsize + 1);
    fread(content, fsize, 1, fp);
    content[fsize] = '\0';

    // Close the file
    fclose(fp);

    return std::string(content);

}

std::string FileSystem::get_root_path() {
    return std::string(root_path);
}