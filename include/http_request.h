#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>
#include <map>

// using namespace std;  헤더파일에서 이거 쓰면 안됨

class HttpRequest {
    public:
        HttpRequest() = default; 
        void parse(const std::string& raw_request);

        std::string method;
        std::string uri;
        std::string http_version;
        std::map<std::string,std::string> headers;
        std::string body;

};

#endif