#include "Session.h"
#include<iomanip>
void Session::Star(){
	memset(_data, 0, max_length);
	_socket.async_read_some(boost::asio::buffer(_data, max_length),
		std::bind(&Session::handle_read,this,std::placeholders::_1,
			std::placeholders::_2,shared_from_this()));
	//在读取时，可以使用async_read读取指定头部字节数，减少切包步骤
}

void Session::PrintRecvData(char* data, int length){
	std::stringstream ss;
	std::string result = "0x";
	for (int i = 0; i < length; i++) {
		std::string hexstr;
		ss << std::hex << std::setw(2) << std::setfill('0') << int(data[i]) << std::endl;
		ss >> hexstr;
		result += hexstr;
	}
	std::cout << "receive raw data is : " << result << std::endl;;
}

void Session::Send(char* msg, int max_length){
	//加锁
	std::lock_guard<std::mutex> lock(_send_lock);
	//判断队列数据,false表示队列没数据
	int send_que_size = _send_que.size();

	bool pending = false;
	if (_send_que.size() > 0) {
		pending = true;
	}

	_send_que.push(std::make_shared<MsgNode>(msg, max_length));
	if (pending) {
		return;
	}
	std::cout << "sending..." << std::endl;
	auto& msgnode = _send_que.front();
	boost::asio::async_write(_socket, boost::asio::buffer(msgnode->_data, msgnode->_total_len),
		std::bind(&Session::handle_write,this,std::placeholders::_1,shared_from_this()));
}

void Session::Close(){
	_socket.close();
	//_b_close = true;
}

void Session::handle_read(const boost::system::error_code& error, 
	size_t bytes_transferred, std::shared_ptr<Session> _self_shared){
	//if (!error) {
	//	std::cout << "server receive data is " << _data << std::endl;
	//	//收到的数据发回给对方
	//	Send(_data, bytes_transferred);
	//	//发完清空data,继续读
	//	memset(_data, 0, max_length);
	//	_socket.async_read_some(boost::asio::buffer(_data, max_length),
	//		std::bind(&Session::handle_read, this, std::placeholders::_1,std::placeholders::_2,_self_shared));
	//}
	//else{
	//	std::cout << "Read Error" << std::endl;
	//	//delete this;
	//	_server->ClearSession(_uuid);
	//}
	
	//进一步优化，是使用切包
	
	//已经移动的(处理完就加上)字符数。bytes_trasnferred表示未处理的数据
	if (!error) {
	//粘包测试
		PrintRecvData(_data, bytes_transferred);
		std::chrono::milliseconds dura(2000);
		std::this_thread::sleep_for(dura);
	int copy_len = 0;
	while (bytes_transferred>0) {
		//判断头部是否被解析
		if (!_b_head_parse) {
			//收到的数据不足头部大小
			if (bytes_transferred + _recv_head_node->_cur_len < HEAD_LENGTH) {
				memcpy(_recv_head_node->_data + _recv_head_node->_cur_len, _data + copy_len, bytes_transferred);
				_recv_head_node->_cur_len += bytes_transferred;
				memset(_data, 0, MAX_LENGTH);
				_socket.async_read_some(boost::asio::buffer(_data, max_length),
				std::bind(&Session::handle_read, this, std::placeholders::_1,std::placeholders::_2,_self_shared));
				return;
			}
			//收到的数据比头部多
			//头部剩余未复制的长度
			int head_remain = HEAD_LENGTH - _recv_head_node->_cur_len;
			memcpy(_recv_head_node->_data + _recv_head_node->_cur_len, _data + copy_len, head_remain);
			//更新已处理的data长度和剩余未处理的长度
			copy_len += head_remain;
			bytes_transferred -= head_remain;
			//获取头部数据
			short data_len = 0;
			memcpy(&data_len, _recv_head_node->_data, HEAD_LENGTH);
			std::cout << "data_len is " << data_len << std::endl;
			//头部长度非法
			if (data_len > MAX_LENGTH) {
				std::cout << "invalid data length is " << data_len << std::endl;
				_server->ClearSession(_uuid);
				return;
			}
			_recv_msg_node = std::make_shared<MsgNode>(data_len);

			//消息的长度小于头部规定的长度，说明数据未收全，则先将部分消息放到接收节点里
			if (bytes_transferred < data_len) {
				memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, bytes_transferred);
				_recv_msg_node->_cur_len += bytes_transferred;
				::memset(_data, 0, MAX_LENGTH);
				_socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
					std::bind(&Session::handle_read, this, std::placeholders::_1, std::placeholders::_2, _self_shared));
				//头部已经处理完成
				_b_head_parse = true;
				return;
			}

			memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, data_len);
			_recv_msg_node->_cur_len += data_len;
			copy_len += data_len;
			bytes_transferred -= data_len;
			_recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';
			std::cout << "receive data is " << _recv_msg_node->_data << std::endl;
			//此处可以调用Send发送测试
			Send(_recv_msg_node->_data, _recv_msg_node->_total_len);
			//继续轮询剩余未处理数据
			_b_head_parse = false;
			_recv_head_node->Clear();
			if (bytes_transferred <= 0) {
				::memset(_data, 0, MAX_LENGTH);
				_socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
					std::bind(&Session::handle_read, this, std::placeholders::_1, std::placeholders::_2, _self_shared));
				return;
			}
			continue;
		}

		//已经处理完头部，处理上次未接受完的消息数据
		//接收的数据仍不足剩余未处理的
		int remain_msg = _recv_msg_node->_total_len - _recv_msg_node->_cur_len;
		if (bytes_transferred < remain_msg) {
			memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, bytes_transferred);
			_recv_msg_node->_cur_len += bytes_transferred;
			::memset(_data, 0, MAX_LENGTH);
			_socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
				std::bind(&Session::handle_read, this, std::placeholders::_1, std::placeholders::_2, _self_shared));
			return;
		}
		memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, remain_msg);
		_recv_msg_node->_cur_len += remain_msg;
		bytes_transferred -= remain_msg;
		copy_len += remain_msg;
		_recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';
		std::cout << "receive data is " << _recv_msg_node->_data << std::endl;
		//此处可以调用Send发送测试
		Send(_recv_msg_node->_data, _recv_msg_node->_total_len);
		//继续轮询剩余未处理数据
		_b_head_parse = false;
		_recv_head_node->Clear();
		if (bytes_transferred <= 0) {
			::memset(_data, 0, MAX_LENGTH);
			_socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
				std::bind(&Session::handle_read, this, std::placeholders::_1, std::placeholders::_2, _self_shared));
			return;
			}
		continue;
		}
	}
	else {
	std::cout << "handle read failed, error is " << error.what() << std::endl;
	Close();
	_server->ClearSession(_uuid);
	}
}


void Session::handle_write(const boost::system::error_code& error, 
	std::shared_ptr<Session> _self_shared){
	if (!error) {
		//std::cout << "server send data is " << _data << std::endl;
		// 应答式
		//memset(_data, 0, max_length);
		//_socket.async_read_some(boost::asio::buffer(_data, max_length),
		//	std::bind(&Session::handle_read, this, std::placeholders::_1, std::placeholders::_2,_self_shared));
		//全双工方式,因为调用的是async_write，
		// 所以如果回调这个函数就说明已经写完了,就可以pop了
		std::lock_guard<std::mutex> lock(_send_lock);
		std::cout << "send data " << _send_que.front()->_data + HEAD_LENGTH << std::endl;
		_send_que.pop();
		//再次判断队列是否为空,不为空继续把队列的数据write
		if (!_send_que.empty()) {
			auto& msgnode = _send_que.front();
			boost::asio::async_write(_socket, boost::asio::buffer(msgnode->_data,msgnode->_total_len),
				std::bind(&Session::handle_write,this,
					std::placeholders::_1,shared_from_this()));
		}
	}
	else {
		std::cout << "Handle write failed,Error is "<<error.what() << std::endl;
		//delete this;
		_server->ClearSession(_uuid);
	}
}

Server::Server(boost::asio::io_context& ioc, short port):_ioc(ioc), 
_acceptor(ioc,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),port)){
	std::cout << "Server start success, on port: " << port << std::endl;
	start_accept();
}

void Server::ClearSession(std::string uuid){
	_sessions.erase(uuid);
}

void Server::start_accept(){
	std::shared_ptr<Session> new_session = std::make_shared<Session>(_ioc,this);
	//Session* new_session = new Session(_ioc);
	_acceptor.async_accept(new_session->Socket(),std::bind(&Server::handle_accpet,this,
		new_session,std::placeholders::_1));

}

void Server::handle_accpet(std::shared_ptr<Session> new_session, const boost::system::error_code& error){
	if (!error) {
		new_session->Star();
		_sessions.insert(std::make_pair(new_session->GetUuid(),new_session));
	}
	else {
		//delete new_session;
		
	}
	start_accept();
}

