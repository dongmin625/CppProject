#include "http_response.h"
#include <sstream>
#include <iomanip> // std::setw 사용 (선택 사항)

using namespace std;

void HttpResponse::set_status(int status_code, const string& status_message) {
    status_code_ = status_code;
    status_message_ = status_message;
}

void HttpResponse::set_header(const string& key, const string& value) {
    headers_[key] = value;
}

void HttpResponse::set_body(const string& body) {
    body_ = body;
    // Content-Length는 to_string에서 설정하는 것이 더 안전할 수 있습니다.
    // stringstream ss;
    // ss << body_.length();
    // headers_["Content-Length"] = ss.str();
}

string HttpResponse::to_string() const {
    ostringstream response;

    // 상태 라인 작성
    response << "HTTP/1.1 " << status_code_ << " " << status_message_ << "\r\n";

    // Content-Length 헤더를 여기서 설정 (본문 길이를 최종적으로 반영)
    response << "Content-Length: " << body_.length() << "\r\n";

    // 다른 헤더 작성
    for (const auto& header : headers_) {
        if (header.first != "Content-Length") { // Content-Length는 이미 설정했으므로 제외
            response << header.first << ": " << header.second << "\r\n";
        }
    }

    // 헤더와 본문 사이의 빈 줄
    response << "\r\n";

    // 본문 작성
    response << body_;

    return response.str();
}