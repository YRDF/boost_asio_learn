#pragma once
#include"Singleton.h"
#include"boost/asio.hpp"
#include<vector>

class AsioIOServicePool:public Singleton<AsioIOServicePool>{
	friend Singleton<AsioIOServicePool>;
public:
	using IOService = boost::asio::io_context;
	using Work = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;
	//using Work = boost::asio::io_context::work;
	using WorkPtr = std::unique_ptr<Work>;
	
	~AsioIOServicePool();
	AsioIOServicePool(const AsioIOServicePool&) = delete;
	AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;
	//使用round-robin的方式返回一个io_contex
	boost::asio::io_context& GetIOService();
	void Stop();
private:
	AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency());
	//存储所有的iocontex
	std::vector<IOService> _ioServices;
	std::vector<WorkPtr> _works;
	std::vector<std::thread> _threads;
	std::size_t _nextIOService;
};

