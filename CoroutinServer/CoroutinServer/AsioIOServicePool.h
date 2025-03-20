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

	//�����õ���iocontex
	boost::asio::io_context& GetIOService();
	void Stop();
	//���ص���ʵ��
	static AsioIOServicePool& GetInstance();
	
private:
	AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency());
	//�����ٵķ���
	std::vector<IOService> _ioServices;
	std::vector<WorkPtr> _works;
	//ÿ��iocontex�����ڶ������߳�
	std::vector<std::thread> _threads;
	std::size_t _nextIOService;
};

