#include "AsioThreadPool.h"

boost::asio::io_context& AsioThreadPool::GetIOService(){
	return _service;
	// TODO: 在此处插入 return 语句
}

void AsioThreadPool::Stop(){
	_work.reset();
	for (auto& t : _threads) {
		t.join();
	}
}

AsioThreadPool::AsioThreadPool(int threadNum):
	_work(std::make_unique<boost::asio::executor_work_guard
		<boost::asio::io_context::executor_type>>(_service.get_executor())){
	for (int i = 0; i < threadNum; ++i) {
		_threads.emplace_back([this]() {
			_service.run();
			});
	}
}
