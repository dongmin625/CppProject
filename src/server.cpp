#include "server.h";
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
}

void Server::do_accept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec,boost::asio::ip::tcp::socket socket) // 새로운 소켓을 생성
        {
            if(!ec) // 오류가 없으면
            {
                handle_request(move(socket)); // 소켓 처리함수 호출
            }

            do_accept(); // 다른 클리이언트 연결 대기 (재귀)
        });
}

void Server::handle_request(boost::asio::ip::tcp::socket socket)
{
    //임시 코드
    string response ="HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    boost::asio::write(socket, boost::asio::buffer(response)); // 소켓을 통해 클라이언트에게 응답함
}