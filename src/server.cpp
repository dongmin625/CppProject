#include "server.h"
#include "http_request.h"
#include "http_response.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory> // shared_ptr 사용을 위해 포함

using namespace std;

// Server 클래스 생성자: 서버를 초기화하고 클라이언트 연결을 기다림
Server::Server(const std::string& address, unsigned short port)
    // acceptor_ 초기화: 주어진 주소와 포트로 연결을 수락할 준비를 함
    : acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(address), port))
{
    do_accept(); // 첫 번째 클라이언트 연결을 비동기적으로 기다리기 시작
}

// 서버 실행 함수
void Server::run()
{
    // 서버가 시작되었음을 알리는 메시지 출력
    cout << "Server running on http://"
         << acceptor_.local_endpoint().address().to_string() // 서버 IP 주소
         << ":" << acceptor_.local_endpoint().port() << endl; // 서버 포트

    io_context_.run(); // Boost.Asio 이벤트 루프 실행: 비동기 작업을 처리하는 핵심
}

// 클라이언트 연결을 비동기적으로 수락하는 함수
void Server::do_accept()
{
    cout << "Waiting for connection..." << endl; // 연결 대기 메시지 출력

    // 비동기적으로 클라이언트 연결을 수락
    acceptor_.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) // 연결 수락 완료 시 호출되는 람다 함수
        {
            if (!ec) // 오류 없이 연결 수락 성공
            {
                cout << "Connection accepted" << endl; // 연결 성공 메시지 출력
                // 소켓을 shared_ptr로 관리하여 수명을 자동 관리
                auto socket_ptr = make_shared<boost::asio::ip::tcp::socket>(move(socket));
                handle_request(socket_ptr); // 클라이언트 요청 처리 함수 호출
            }
            else // 연결 수락 실패
            {
                cerr << "Accept error: " << ec.message() << endl; // 오류 메시지 출력
            }

            do_accept(); // 다음 클라이언트 연결을 계속 기다림 (재귀 호출)
        });
}

// 클라이언트 요청을 처리하는 함수 (소켓 포인터를 shared_ptr로 받음)
void Server::handle_request(shared_ptr<boost::asio::ip::tcp::socket> socket_ptr)
{
    auto self(shared_from_this()); // Server 객체의 shared_ptr을 생성하여 콜백 내에서 안전하게 사용
    read_request(socket_ptr);      // 클라이언트 요청을 읽는 함수 호출
}

// 클라이언트 요청을 비동기적으로 읽는 함수 (소켓 포인터를 shared_ptr로 받음)
void Server::read_request(shared_ptr<boost::asio::ip::tcp::socket> socket_ptr)
{
    auto self(shared_from_this()); // Server 객체의 shared_ptr을 생성하여 콜백 내에서 안전하게 사용
    // 요청 데이터를 저장할 버퍼를 unique_ptr로 생성하여 자동 메모리 관리
    auto data = make_unique<char[]>(max_length);

    // 클라이언트로부터 데이터를 비동기적으로 읽음
    socket_ptr->async_read_some(boost::asio::buffer(data.get(), max_length),
        [this, self, socket_ptr, buffer = move(data)](boost::system::error_code ec, size_t length) mutable
        {
            if (!ec) // 오류 없이 데이터 읽기 성공
            {
                cout << "Received data: " << length << " bytes" << endl; // 받은 데이터 크기 출력
                HttpRequest request;
                try
                {
                    request.parse(string(buffer.get(), length)); // 받은 데이터를 문자열로 변환하여 HTTP 요청 파싱
                    cout << "Parsed Request URI: " << request.uri << endl; // 파싱된 URI 출력
                }
                catch (const exception& e)
                {
                    cerr << "Request parsing error: " << e.what() << endl; // 파싱 오류 메시지 출력
                    HttpResponse response;
                    response.set_status(400, "Bad Request"); // 400 Bad Request 응답 설정
                    response.set_body("<html><body><h1>400 Bad Request</h1><p>Invalid request format.</p></body></html>");
                    response.set_header("Content-Type", "text/html");
                    string response_str = response.to_string();
                    write_response(socket_ptr, response_str); // 오류 응답 전송
                    return;
                }

                HttpResponse response;
                if (request.uri == "/")
                {
                    response.set_status(200, "OK");
                    response.set_body("<html><body><h1>Welcome to our C++ HTTP Server!</h1></body></html>");
                }
                else if (request.uri == "/about")
                {
                    response.set_status(200, "OK");
                    response.set_body("<html><body><h1>About Us</h1><p>We are passionate C++ developers.</p></body></html>");
                }
                else
                {
                    response.set_status(404, "Not Found");
                    response.set_body("<html><body><h1>404 Not Found</h1></body></html>");
                }

                response.set_header("Content-Type", "text/html");
                string response_str = response.to_string();

                write_response(socket_ptr, response_str); // 응답 전송
            }
            else if (ec == boost::asio::error::eof) // 클라이언트가 연결을 정상적으로 종료
            {
                cout << "Client disconnected." << endl; // 클라이언트 연결 종료 메시지 출력
                // 클라이언트 연결 종료 처리 (필요한 경우)
            }
            else // 다른 읽기 오류 발생
            {
                cerr << "Read error: " << ec.message() << endl; // 오류 메시지 출력
            }
        });
}

// 클라이언트에게 응답을 비동기적으로 전송하는 함수 (소켓 포인터를 shared_ptr로 받음)
void Server::write_response(shared_ptr<boost::asio::ip::tcp::socket> socket_ptr, const string& response_str)
{
    auto self(shared_from_this()); // Server 객체의 shared_ptr을 생성하여 콜백 내에서 안전하게 사용
    // 응답 데이터를 클라이언트에게 비동기적으로 전송
    boost::asio::async_write(*socket_ptr, boost::asio::buffer(response_str),
        [this, self, socket_ptr](boost::system::error_code ec, size_t /*length*/) mutable
        {
            if (!ec) // 오류 없이 응답 전송 성공
            {
                cout << "Response sent successfully" << endl;
                read_request(socket_ptr); // 다음 요청을 읽기 위해 다시 호출 (Keep-Alive)
            }
            else // 응답 전송 실패
            {
                cerr << "Write error: " << ec.message() << endl; // 오류 메시지 출력
                // 오류 발생 시 소켓 정리 등의 처리 필요 (현재는 생략)
            }
        });
}