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

void signal_handler(const boost::system::error_code& err,int signal){
	cout<<"Receive Sig : "<<signal<<endl;
}

int main(int argc, char **argv) {
	try {
		io_service service;
		boost::asio::signal_set sig(service,SIGINT,SIGTERM);
		sig.async_wait(signal_handler);
		//采用异步需要调用run(),而run将阻塞线程
		service.run();
	} catch (std::exception &e) {
		printf("%s", e.what());
	}
	cout<<"Run after async\n";
}
