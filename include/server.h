#ifndef HTTP_SERVER_H // 헤더가 포함된 적 있는지 검사 (헤더 가드)
#define HTTP_SERVER_H // 한 번만 포함되도록 설정

#include <boost/asio.hpp>     // 비동기 입출력 처리 라이브러리
#include <string>             // std::string 사용을 위한 헤더
#include <memory>             // std::shared_ptr 사용을 위한 헤더

class Server : public std::enable_shared_from_this<Server> { // std:: 추가: 자기 자신의 shared_ptr 생성에 필요
private:
    static constexpr size_t max_length = 8192; // 데이터 버퍼의 최대 크기 정의

    // 클라이언트로부터 받은 데이터를 저장할 버퍼
    // handle_request 함수 내에서 동적으로 할당하여 사용하므로 여기서는 선언만 합니다.
    // char data_[max_length]; // 더 이상 클래스 멤버 변수로 사용하지 않음

    void do_accept();                                                                 // 새로운 클라이언트 연결을 수락하는 함수
    void handle_request(std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr);       // 클라이언트의 요청을 처리하는 함수
    void read_request(std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr);         // 클라이언트 요청을 읽는 함수
    void write_response(std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr, const std::string& response); // 클라이언트에게 응답을 보내는 함수

    boost::asio::io_context io_context_;      // I/O 작업을 관리하는 객체
    boost::asio::ip::tcp::acceptor acceptor_; // 클라이언트 연결을 수락하는 객체

public:
    // 생성자: 서버의 주소와 포트 번호를 인자로 받아 초기화
    Server(const std::string& address, unsigned short port);

    // 서버를 실행하는 함수
    void run();
};

#endif // HTTP_SERVER_H // 헤더 가드 종료