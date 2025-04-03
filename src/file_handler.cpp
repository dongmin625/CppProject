#include "file_handler.h"
#include <fstream>
#include <sstream>
#include <filesystem>


// 지정된 경로의 파일을 읽어와 문자열로 반환하는 함수
std::optional<std::string> FileHandler::read_file(const std::string& path){
    std::ifstream file(path, std::ios::binary); // 바이너리 모드로 파일 열기
    if(!file){
        return std::nullopt;
    }

    std::ostringstream content;
    content << file.rdbuf(); // 파일 내용저장
    return content.str();
}

std::string FileHandler::get_mime_type(const std::string& path){
    std::filesystem::path file_path(path);
    std::string extension = file_path.extension().string(); // 확장자 확인

    if (extension == ".html" || extension == ".htm") return "text/html";
    if (extension == ".css") return "text/css";
    if (extension == ".js") return "application/javascript";
    if (extension == ".jpg" || extension == ".jpeg") return "image/jpeg";
    if (extension == ".png") return "image/png";
    if (extension == ".gif") return "image/gif";

    return "application/octet-stream";  // 기본 MIME 타입
}