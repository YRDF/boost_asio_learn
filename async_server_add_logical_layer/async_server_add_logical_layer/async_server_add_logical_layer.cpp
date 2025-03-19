// AsyncServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//使用boost::asio创建一个异步的服务器
#include <iostream>
#include "CServer.h"
#include<csignal>
#include<thread>
#include<mutex>
//c风格处理服务器优雅退出
bool bstop = false;
std::condition_variable cond_quit;
std::mutex mutex_quit;

void sig_handler(int sig) {
	//如果收到ctrl+c或者linux的kill命令
	if (sig == SIGINT||sig == SIGTERM) {
		std::unique_lock<std::mutex> lock_quit(mutex_quit);
		bstop = true;
		cond_quit.notify_one();
	}
}

using namespace std;
int main()
{
	try {
		//boost::asio::io_context  io_context;
		////启动线程，让服务器在其中运行
		//std::thread net_work_thread([&io_context]() {
		//	CServer s(io_context, 10086);
		//	io_context.run();
		//	});
		////捕获信号，回调sig_handler函数
		//signal(SIGINT, sig_handler);
		//signal(SIGTERM, sig_handler);
		//while (!bstop) {
		//	std::unique_lock<std::mutex> lock_quit(mutex_quit);
		//	cond_quit.wait(lock_quit);
		//}
		//io_context.stop();
		//net_work_thread.join();

		////CServer s(io_context, 10086);
		////io_context.run();


	//asio提供的方式:
		boost::asio::io_context io_contex;
		boost::asio::signal_set signals(io_contex,SIGINT,SIGTERM);
		signals.async_wait([&io_contex](auto,auto) {
			io_contex.stop();
			});

		CServer s(io_contex, 10086);
		io_contex.run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << endl;
	}
	//boost::asio::io_context io_context;
}

