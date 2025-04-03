#include "http_request.h"
#include <sstream>
#include <iostream> // 디버깅 용

void HttpRequest::parse(const std::string& raw_request) {
    std::istringstream request_stream(raw_request);
    std::string line;

    // 요청 라인 파싱
    getline(request_stream, line);
    // 첫 번째 줄에서 캐리지 리턴(\r) 제거
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }
    std::istringstream request_line(line);
    request_line >> method >> uri >> http_version;
    std::cout << "Method: " << method << ", URI: " << uri << ", HTTP Version: " << http_version << std::endl; // 디버깅

    // 헤더 파싱
    while (getline(request_stream, line)) {
        // 각 줄에서 캐리지 리턴(\r) 제거
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) { // 빈 줄은 헤더의 끝을 의미
            break;
        }
        auto colon_pos = line.find(':');
        if (colon_pos != std::string::npos) {
            auto key = line.substr(0, colon_pos);
            auto value = line.substr(colon_pos + 1);
            // 앞뒤 공백 제거
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            headers[key] = value;
            std::cout << "Header: " << key << ": " << value << std::endl; // 디버깅
        }
    }

    // 바디 파싱 (Content-Length 기반으로 수정)
    if (headers.count("Content-Length")) {
        try {
            size_t content_length = stoul(headers["Content-Length"]);
            body.resize(content_length);
            request_stream.read(body.data(), content_length);
            std::cout << "Body: " << body << std::endl; // 디버깅
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error parsing Content-Length: " << e.what() << std::endl;
            // Content-Length 파싱 실패 시 처리 (예: 바디를 읽지 않음)
        }
    } else {
        // Content-Length 헤더가 없는 경우, 바디가 없을 수도 있음
        std::cout << "No Content-Length header found." << std::endl; // 디버깅
    }
}