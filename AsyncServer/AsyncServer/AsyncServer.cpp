// AsyncServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//使用boost::asio创建一个异步的服务器
#include"Session.h"
#include <iostream>

int main(){
	try {
		boost::asio::io_context ioc;
		Server s(ioc,10086);
		ioc.run();
	}catch (std::exception& e) {
		std::cerr << "exception: " << e.what() << "\n";
	}

	return 0;
}

