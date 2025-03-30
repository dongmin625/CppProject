#include "server.h"
#include "http_request.h"
#include "http_response.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Server 클래스 생성자: 서버를 초기화하고 클라이언트 연결을 기다림
Server::Server(const std::string& address, unsigned short port)
    : acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(address), port)) 
{
    do_accept();  // 첫 번째 클라이언트 연결 대기 시작
}

// 서버 실행 함수
void Server::run()
{
    cout << "Server running on http://" 
         << acceptor_.local_endpoint().address().to_string()  // 서버의 IP 주소 출력
         << ":" << acceptor_.local_endpoint().port() << endl; // 서버의 포트 출력

    io_context_.run();  // 비동기 이벤트 루프 실행 (클라이언트 요청을 처리하는 핵심 부분)
}

//  클라이언트 연결을 비동기적으로 기다리는 함수
void Server::do_accept()
{   
    cout << "Waiting for connection..." << endl;  // 연결 대기 메시지 출력

    // 새로운 클라이언트 연결을 비동기적으로 수락
    acceptor_.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) 
        {
            if (!ec) // 오류가 발생하지 않았다면
            {
                cout << "Connection accepted" << endl;  // 연결 성공 메시지 출력
                handle_request(move(socket));  // 클라이언트 요청 처리 함수 호출
            }

            do_accept();  // 다음 클라이언트의 연결을 계속 대기 (재귀 호출)
        });
}

// 클라이언트 요청을 처리하는 함수
void Server::handle_request(boost::asio::ip::tcp::socket socket)
{
    auto self(shared_from_this());  // 서버 인스턴스를 `shared_ptr`로 보관하여 수명 관리

    // 클라이언트로부터 데이터를 비동기적으로 읽음
    socket.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self, socket = move(socket)](boost::system::error_code ec, size_t length) mutable
        {
            if (!ec)  // 오류가 없으면
            {
                HttpRequest request;  
                request.parse(string(data_, length));  // 요청을 문자열로 변환 후 파싱

                HttpResponse response;  // 응답 객체 생성

                // 요청된 URI에 따라 응답 생성 (간단한 라우팅 기능)
                if (request.uri == "/") {
                    response.set_status(200, "OK");
                    response.set_body("<html><body><h1>Welcome to our C++ HTTP Server!</h1></body></html>");
                } else if (request.uri == "/about") {
                    response.set_status(200, "OK");
                    response.set_body("<html><body><h1>About Us</h1><p>We are passionate C++ developers.</p></body></html>");
                } else {
                    response.set_status(404, "Not Found");
                    response.set_body("<html><body><h1>404 Not Found</h1></body></html>");
                }

                response.set_header("Content-Type", "text/html");  // 🔹 응답의 헤더 설정

                // 🔹 응답을 문자열로 변환
                string response_str = response.to_string();

                // 🔹 클라이언트에게 응답 데이터를 비동기적으로 전송
                boost::asio::async_write(socket, boost::asio::buffer(response_str),
                    [this, self, socket = move(socket)](boost::system::error_code ec, size_t /*length*/) mutable
                    {
                        if (!ec)
                        {
                            handle_request(move(socket));  // 다음 요청을 계속 처리 (keep-alive)
                        }
                    });
            }
        });
}
