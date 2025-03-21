#include "CSession.h"
#include"CServer.h"
#include "LogicSystem.h"

CSession::CSession(boost::asio::io_context& io_contex, CServer* server):_io_contex(io_contex),
_server(server),_socket(io_contex),_b_close(false),_b_head_parse(false){
	
	boost::uuids::uuid  a_uuid = boost::uuids::random_generator()();
	_uuid = boost::uuids::to_string(a_uuid);
	_recv_head_node = std::make_shared<MsgNode>(HEAD_TOTAL_LEN);
}

CSession::~CSession(){
	try {
		std::cout << "~CSession destruct" << std::endl;
		Close();
		//std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	catch (std::exception& exp) {
		std::cout << "exception is " << exp.what() << std::endl;
	}
}

boost::asio::ip::tcp::socket& CSession::GetSocket(){
	return _socket;
	// TODO: 在此处插入 return 语句
}

std::string& CSession::GetUuid(){
	return _uuid;
	// TODO: 在此处插入 return 语句
}

void CSession::Start(){
	//协程方式
	auto shared_this = shared_from_this();
	//开启协程
	boost::asio::co_spawn(_io_contex, [=]()->boost::asio::awaitable<void> {
		try {
			for (; !_b_close;) {
				_recv_head_node->Clear();
				std::size_t n = co_await boost::asio::async_read(_socket,
					boost::asio::buffer(_recv_head_node->_data, HEAD_TOTAL_LEN),
					use_awaitable);

				if (n == 0) {
					std::cout << "receive peer closed" << std::endl;
					Close();
					_server->ClearSession(_uuid);
					co_return;
				}

				//获取头部MSGID数据
				short msg_id = 0;
				memcpy(&msg_id, _recv_head_node->_data, HEAD_ID_LEN);
				//网络字节序转化为本地字节序
				msg_id = boost::asio::detail::socket_ops::network_to_host_short(msg_id);
				std::cout << "msg_id is " << msg_id << std::endl;
				//id非法
				if (msg_id > MAX_LENGTH) {
					std::cout << "invalid msg_id is " << msg_id << std::endl;
					_server->ClearSession(_uuid);
					co_return;
				}
				short msg_len = 0;
				memcpy(&msg_len, _recv_head_node->_data + HEAD_ID_LEN, HEAD_DATA_LEN);
				//网络字节序转化为本地字节序
				msg_len = boost::asio::detail::socket_ops::network_to_host_short(msg_len);
				std::cout << "msg_len is " << msg_len << std::endl;
				//长度非法
				if (msg_len > MAX_LENGTH) {
					std::cout << "invalid data length is " << msg_len << std::endl;
					_server->ClearSession(_uuid);
					co_return;
				}

				_recv_msg_node = std::make_shared<RecvNode>(msg_len, msg_id);
				//读出包体
				n = co_await boost::asio::async_read(_socket,
					boost::asio::buffer(_recv_msg_node->_data, _recv_msg_node->_total_len), use_awaitable);

				if (n == 0) {
					std::cout << "receive peer closed" << std::endl;
					Close();
					_server->ClearSession(_uuid);
					co_return;
				}

				_recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';
				std::cout << "receive data is " << _recv_msg_node->_data << std::endl;
				//投递给逻辑线程
				LogicSystem::GetInstance().PostMsgToQue(make_shared<LogicNode>(shared_from_this(), _recv_msg_node));
			}
		}catch (std::exception& e) {
			std::cout << "exception is " << e.what() << std::endl;
			Close();
			_server->ClearSession(_uuid);
		}
		},detached);
}

void CSession::Close(){
	_socket.close();
	_b_close = true;
}

void CSession::Send(const char* msg, short max_length, short msgid){
	std::unique_lock<std::mutex> lock(_send_lock);
	int send_que_size = _send_que.size();
	if (send_que_size > MAX_SENDQUE) {
		std::cout << "session: " << _uuid << " send que fulled, size is " << MAX_SENDQUE << std::endl;
		return;
	}

	_send_que.push(std::make_shared<SendNode>(msg, max_length, msgid));
	if (send_que_size > 0) {
		return;
	}
	auto msgnode = _send_que.front();
	lock.unlock();
	boost::asio::async_write(_socket, boost::asio::buffer(msgnode->_data, msgnode->_total_len),
		std::bind(&CSession::HandleWrite, this, std::placeholders::_1,shared_from_this()));
}

void CSession::Send(std::string msg, short msgid){
	Send(msg.c_str(), msg.length(), msgid);
}

void CSession::HandleWrite(const boost::system::error_code& error, std::shared_ptr<CSession> shared_self){
	//增加异常处理
	try {
		if (!error) {
			std::unique_lock<std::mutex> lock(_send_lock);
			//cout << "send data " << _send_que.front()->_data+HEAD_LENGTH << endl;
			_send_que.pop();
			if (!_send_que.empty()) {
				auto msgnode = _send_que.front();
				lock.unlock();
				boost::asio::async_write(_socket, boost::asio::buffer(msgnode->_data, msgnode->_total_len),
					std::bind(&CSession::HandleWrite, this, std::placeholders::_1, shared_self));
			}
		}
		else {
			std::cout << "handle write failed, error is " << error.what() << std::endl;
			Close();
			_server->ClearSession(_uuid);
		}
	}
	catch (std::exception& e) {
		std::cerr << "Exception code : " << e.what() << std::endl;
		Close();
		_server->ClearSession(_uuid);
	}

}

LogicNode::LogicNode(std::shared_ptr<CSession> session, std::shared_ptr<RecvNode> recvnode) 
	:_session(session), _recvnode(recvnode) {
}
