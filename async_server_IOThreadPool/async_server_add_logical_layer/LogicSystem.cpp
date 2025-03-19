#include "LogicSystem.h"
using namespace std;

LogicSystem::~LogicSystem(){
	_b_stop = true;
	_consume.notify_one();
	_worker_thread.join();
}

void LogicSystem::PostMsgToQue(shared_ptr<LogicNode> msg){
	std::unique_lock<std::mutex> unique_lk(mutex);
	_msg_que.push(msg);
	if (_msg_que.size() == 1) {
		_consume.notify_one();
	}
}

LogicSystem::LogicSystem():_b_stop(false){
	RegisterCallBacks();
	_worker_thread = std::thread(&LogicSystem::DealMsg, this);
}

void LogicSystem::RegisterCallBacks(){
	_fun_callback[MSG_HELLO_WORD] = std::bind(&LogicSystem::HelloWordCallBack,
		this, placeholders::_1, placeholders::_2, placeholders::_3);
}

void LogicSystem::HelloWordCallBack(shared_ptr<CSession> session, 
	const short& msg_id, const string& msg_data){
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	std::cout << "receive msg id is " << root["id"].asInt()<< "msg data is "
		<< root["data"].asString() << endl;
	root["data"] = "server has receive msg, msg data is " + root["data"].asString();

	std::string return_str = root.toStyledString();
	session->Send(return_str,root["id"].asInt());

}

void LogicSystem::DealMsg(){
	for (;;) {
		std::unique_lock<std::mutex> unique_lk(_mutex);

		//判断队列为空则用条件变量等待
		while (_msg_que.empty() && !_b_stop) {
			_consume.wait(unique_lk);
		}
		//线程激活还是变成了关闭转换
		//如果为关闭状态，取出逻辑队列所有数据及时处理并退出循环
		if (_b_stop) {
			while (!_msg_que.empty()) {
				auto msg_node = _msg_que.front();
				cout << "recv msg id is " << msg_node->_recvnode->_msg_id << endl;
				auto call_back_iter = _fun_callback.find(msg_node->_recvnode->_msg_id);
				if (call_back_iter == _fun_callback.end()) {
					_msg_que.pop();
					continue;
				}
				call_back_iter->second(msg_node->_session, msg_node->_recvnode->_msg_id,
					std::string(msg_node->_recvnode->_data,msg_node->_recvnode->_total_len));
				_msg_que.pop();
			}
			break;
		}
		//队列不为空且并未停服
		auto msg_node = _msg_que.front();
		cout << "recv msg id is " << msg_node->_recvnode->_msg_id << endl;

		auto call_back_iter = _fun_callback.find(msg_node->_recvnode->_msg_id);

		if (call_back_iter == _fun_callback.end()) {
			_msg_que.pop();
			continue;
		}
		call_back_iter->second(msg_node->_session, msg_node->_recvnode->_msg_id,
			std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_total_len));
		_msg_que.pop();
	}
}
