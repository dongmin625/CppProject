#include "http_request.h"
#include <sstream>

void HttpRequest::parse(const string& raw_request){
    istringstream request_stream(raw_request); // cin과달리 공백을 포함한 전체 문자열 저장 + 문자열을 코드에서 다룰수 있음
    string line;


    //요청 라인 파싱
    getline(request_stream, line);
    //첫 번째 줄을 읽어서 `line`에 저장 (예: `"GET /index.html HTTP/1.1"`)

    istringstream request_line(line);
    request_line >> method >> uri >> http_version; // 공백단위로 받아 request_line에 저장

    //헤더 파싱
    while(getline(request_stream,line) && line != "\r"){ // 줄을 읽으며 빈줄이 나올때 까지 반복
        auto colon_pos = line.find(':'); // : 으로 key와 value 구분
        if(colon_pos != string::npos){
            auto key = line.substr(0, colon_pos);
            auto value = line.substr(colon_pos+1);
            // 앞뒤 공백 제거
            value.erase(0,value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") +1);
            headers[key] = value;

        }
    }

    // 바디 파싱
    std::string body_str((std::istreambuf_iterator<char>(request_stream)),
                          std::istreambuf_iterator<char>());
    body = body_str;
}