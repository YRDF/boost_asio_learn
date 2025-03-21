#include<iostream>
#include<boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>
#include <cstdio>
#include<thread>
#include<mutex>
#include<csignal>
#include"CServer.h"
#include"AsioIOServicePool.h"


int main(){
	try {
		auto& pool = AsioIOServicePool::GetInstance();
		boost::asio::io_context ioc;
		boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
		signals.async_wait([&ioc,&pool](auto,auto) {
			ioc.stop();
			pool.Stop();
			});

		CServer s(ioc, 10086);
		ioc.run();

	}catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}