/** 
 * @file         main.cpp
 * @brief
 * @details
 * @author       Pengxiang Jin
 * @date     	2018年1月23日
 * @version  	Version
 * @par Copyright (c):
 *       桂林优利特医疗电子有限公司
 * @par History:
 *   version: author, date, desc\n
 */

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>

using namespace boost::asio;
using std::cout;
using std::endl;

typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;

//同步客户端
void Client() {

	io_service service;
	ip::tcp::socket sock(service); //C里的建立socket
	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8080);
	sock.connect(ep); //把地址赋给socket然后发起连接
}

//同步服务器
void client_session(socket_ptr sock) {
	while (true) {
		char data[512];
		size_t len = sock->read_some(buffer(data));
		printf("%s", data);
		if (len > 0) {
			write(*sock, buffer("ok", 2));
		}
	}
}
void Server() {
	io_service service;
	ip::tcp::endpoint ep(ip::tcp::v4(), 8080);
	ip::tcp::acceptor acc(service, ep);

	while (true) {
		printf("Into while\n");
		socket_ptr sock(new ip::tcp::socket(service));
		printf("1\n");
		acc.accept(*sock);
		printf("2\n");
		boost::thread(boost::bind(client_session, sock));
		printf("leave while\n");
	}
}

//异步客户端
void connect_handler(const boost::system::error_code &ec) {
	if (!ec) {

	} else {
		printf("%s", ec.message().c_str());

	}

}
void async_Client() {
	io_service service;
	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8080);
	ip::tcp::socket sock(service);
	sock.async_connect(ep, connect_handler);
	service.run();
}

//异步服务器
void start_accept(socket_ptr sock);
io_service service;
ip::tcp::endpoint ep(ip::tcp::v4(), 8080);
ip::tcp::acceptor acc(service, ep);
void handle_accept(socket_ptr sock,
		const boost::system::error_code& ec) {
	if (!ec) {
		char data[1024];
		memset(data, 0, 1024);
		size_t len = sock->read_some(buffer(data,1024));
		cout<<"len : "<<len<<endl;
		printf("%s\n", data);
		write(*sock, buffer("ok", 2));
	} else {
		printf("%s\n", ec.message().c_str());
	}
	socket_ptr sock2(new ip::tcp::socket(service));
	start_accept(sock2);
}
void start_accept(socket_ptr sock) {
	cout<<"start_accept"<<endl;
	acc.async_accept(*sock, boost::bind(handle_accept, sock,_1));
}
void async_Server() {

	socket_ptr sock(new ip::tcp::socket(service));
 	start_accept(sock);
	service.run();

}

int main(int argc, char **argv) {
	try {
		async_Server();
	} catch (std::exception &e) {
		printf("%s", e.what());
	}
}
