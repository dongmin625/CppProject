#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>
#include <map>


class HttpResponse{
    private:
        int status_code_ =200;
        std::string status_message_ ="OK";
        std::map<std::string,std::string> headers_;
        std::string body_;
    
    public:
        HttpResponse() = default;
        void set_status(int status_code, const std::string& status_message);
        void set_header(const std::string& key, const std::string& value);
        void set_body(const std::string& body);
        std::string to_string() const;
};

#endif