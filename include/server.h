#ifndef HTTP_SERVER_H // 헤더가 포함된 적 있는지 검사
#define HTTP_SERVER_H // 한 번만 포함되도록 설정

#include <boost/asio.hpp> // 비동기 입출력 처리 라이브러리
#include <string>
#include <memory> // 스마트 포인터 라이브러리(메모리 관리 자동화 도움)

class Server : public std::enable_shared_from_this<Server> {  // std:: 추가
private:
    static constexpr size_t max_length = 8192;  // 버퍼 크기 정의
    char data_[max_length];  // 데이터 버퍼

    void do_accept(); // 새로운 연결 수락
    void handle_request(boost::asio::ip::tcp::socket socket); // 클라이언트의 요청 처리

    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_; // 객체 생성

public:
    Server(const std::string& address, unsigned short port); // 서버 주소,포트 받아 초기화
    void run(); // 서버시작
};

#endif // 헤더 가드 종료
