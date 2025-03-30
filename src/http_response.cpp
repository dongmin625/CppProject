#include "http_response.h"
#include <sstream>

using namespace std;

void HttpResponse::set_status(int status_code, const string& status_message){
    status_code_ = status_code;
    status_message_ =status_message;
}

void HttpResponse::set_header(const string& key, const string& value){
    headers_[key] = value;
}

void HttpResponse::set_body(const string& body){
    body_ = body;
    stringstream ss;
    ss << body_.length();  // size_t 값을 스트림에 넣기
    headers_["Content-Length"] = ss.str();  // 문자열로 변환
}

string HttpResponse::to_string() const{
    ostringstream response;
    response << "HTTP/1.1 "<< status_code_ << " " << status_message_ << "\r\n";

    for (const auto& header : headers_){
        response << header.first << ": " << header.second << "\r\n";
    }

    response << "\r\n" << body_;
    return response.str();
}