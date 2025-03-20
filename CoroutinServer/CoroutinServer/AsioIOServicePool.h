#pragma once
#include<boost/asio.hpp>
#include<iostream>
#include<vector>

class AsioIOServicePool{
public;
	using IOService = boost::asio::io_context;
	using Work = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;
	//using Work = boost::asio::io_context::work;
	using WorkPtr = std::unique_ptr<Work>;
	AsioIOServicePool(const AsioIOServicePool&) = delete;
	AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;

	~AsioIOServicePool();

	//返回用到的iocontex
	boost::asio::io_context& GetIOService();
	void Stop();
	//返回单例实例
	static AsioIOServicePool& GetInstance();
	
private:
	AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency());
	//管理开辟的服务
	std::vector<IOService> _ioServices;
	std::vector<WorkPtr> _works;
	//每个iocontex都跑在独立的线程
	std::vector<std::thread> _threads;
	std::size_t _nextIOService;
};

