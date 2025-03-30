#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>
#include <map>

using namespace std;

class HttpResponse{
    private:
        int status_code_ =200;
        string status_message_ ="OK";
        map<string,string> headers_;
        string body_;
    
    public:
        HttpResponse() = default;
        void set_status(int status_code, const string& status_message);
        void set_header(const string& key, const string& value);
        void set_body(const string& body);
        string to_string() const;
};

#endif