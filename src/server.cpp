#include "server.h"
#include <iostream>
#include <string>

using namespace std;

Server::Server(const std::string& address, unsigned short port)
    : acceptor_(io_context_,boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(address),port))
{
    do_accept();
}

void Server::run()
{
    cout<<"Server running on http://"<<acceptor_.local_endpoint().address().to_string() // 서버 주소 출력
        <<":"<<acceptor_.local_endpoint().port()<<endl; // 포트 번호 출력
    io_context_.run(); // 이벤트 시작
}

void Server::do_accept()
{   
    cout << "Waiting for connection..." << endl;  // 디버그 메시지
    acceptor_.async_accept(
        [this](boost::system::error_code ec,boost::asio::ip::tcp::socket socket) // 새로운 소켓을 생성
        {
            if(!ec) // 오류가 없으면
            {
                cout << "Connection accepted" << endl;  // 연결 수락시 메시지
                handle_request(move(socket)); // 소켓 처리함수 호출
            }

            do_accept(); // 다른 클리이언트 연결 대기 (재귀)
        });
}

void Server::handle_request(boost::asio::ip::tcp::socket socket)
{
    // HTML 형식으로 응답을 생성
    string response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n" // Content-Type을 HTML로 설정
        "Content-Length: 46\r\n"
        "\r\n"
        "<html><body><h1>Hello, World!</h1></body></html>";  // HTML로 "Hello, World!" 메시지 반환
    
    boost::asio::write(socket, boost::asio::buffer(response));  // 소켓을 통해 클라이언트에게 응답
}
