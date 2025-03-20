#include "AsioIOServicePool.h"

AsioIOServicePool::~AsioIOServicePool() {
	std::cout << "AsioIOServicePool destruct" << std::endl;
}

boost::asio::io_context& AsioIOServicePool::GetIOService(){
	auto& service = _ioServices[_nextIOService++];
	if (_nextIOService == _ioServices.size()) {
		_nextIOService = 0;
	}
	return service;
	// TODO: �ڴ˴����� return ���
}

void AsioIOServicePool::Stop(){
	for (auto& work : _works) {
		work.reset();
	}
	for (auto& t : _threads) {
		t.join();
	}
}

AsioIOServicePool& AsioIOServicePool::GetInstance(){
	static AsioIOServicePool instance(1);
	return instance;
	// TODO: �ڴ˴����� return ���
}

AsioIOServicePool::AsioIOServicePool(std::size_t size) :_ioServices(size),
_works(size), _nextIOService(0) {
	for (std::size_t i = 0; i < size; ++i) {
		_works[i] = std::make_unique<Work>(boost::asio::make_work_guard(_ioServices[i]));
	}
	//�������ioservice����������̣߳�ÿ���߳��ڲ�����ioservice
	for (std::size_t i = 0; i < _ioServices.size(); ++i) {
		_threads.emplace_back([this, i]() {
			_ioServices[i].run();
			});
	}
}
