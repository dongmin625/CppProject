#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>
#include <map>

using namespace std;

class HttpRequest {
    public:
        HttpRequest() = default; 
        void parse(const string& raw_request);

        string method;
        string uri;
        string http_version;
        map<string,string> headers;
        string body;

};

#endif