#include <stdio.h>
#include <string>
#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include "esp_log.h"
#include "esp_spiffs.h"

class FileSystem {

    public:
        FileSystem();
        ~FileSystem();

        std::string read_file(std::string fname);
        std::string get_root_path();

    private:
        const char *tag = "FileSystem";
        const char *root_path = "/spiffs";

};

#endif /* __FILE_SYSTEM_H__ */