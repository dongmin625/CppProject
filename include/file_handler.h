#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <optional>

class FileHandler {
    public:
        static std::optional<std::string> read_file(const std::string& path);
        static std::string get_mime_type(const std:: string& path);
};


#endif