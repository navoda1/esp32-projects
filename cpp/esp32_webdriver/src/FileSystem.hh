#include <stdio.h>
#include <string.h>
#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include "esp_log.h"
#include "esp_spiffs.h"

class FileSystem {

    public:
        FileSystem();
        ~FileSystem();

        char* read_file(const char *fname);

    private:
        static constexpr const char *tag = "FileSystem";

};

#endif /* __FILE_SYSTEM_H__ */