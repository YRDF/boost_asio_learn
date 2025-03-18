#include "Session.h"
#include<iomanip>
void Session::Star(){
	memset(_data, 0, max_length);
	_socket.async_read_some(boost::asio::buffer(_data, max_length),
		std::bind(&Session::handle_read,this,std::placeholders::_1,
			std::placeholders::_2,shared_from_this()));
	//�ڶ�ȡʱ������ʹ��async_read��ȡָ��ͷ���ֽ����������а�����
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
	//����
	std::lock_guard<std::mutex> lock(_send_lock);
	//�ж϶�������,false��ʾ����û����
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
	//	//�յ������ݷ��ظ��Է�
	//	Send(_data, bytes_transferred);
	//	//�������data,������
	//	memset(_data, 0, max_length);
	//	_socket.async_read_some(boost::asio::buffer(_data, max_length),
	//		std::bind(&Session::handle_read, this, std::placeholders::_1,std::placeholders::_2,_self_shared));
	//}
	//else{
	//	std::cout << "Read Error" << std::endl;
	//	//delete this;
	//	_server->ClearSession(_uuid);
	//}
	
	//��һ���Ż�����ʹ���а�
	
	//�Ѿ��ƶ���(������ͼ���)�ַ�����bytes_trasnferred��ʾδ���������
	if (!error) {
	//ճ������
		PrintRecvData(_data, bytes_transferred);
		std::chrono::milliseconds dura(2000);
		std::this_thread::sleep_for(dura);
	int copy_len = 0;
	while (bytes_transferred>0) {
		//�ж�ͷ���Ƿ񱻽���
		if (!_b_head_parse) {
			//�յ������ݲ���ͷ����С
			if (bytes_transferred + _recv_head_node->_cur_len < HEAD_LENGTH) {
				memcpy(_recv_head_node->_data + _recv_head_node->_cur_len, _data + copy_len, bytes_transferred);
				_recv_head_node->_cur_len += bytes_transferred;
				memset(_data, 0, MAX_LENGTH);
				_socket.async_read_some(boost::asio::buffer(_data, max_length),
				std::bind(&Session::handle_read, this, std::placeholders::_1,std::placeholders::_2,_self_shared));
				return;
			}
			//�յ������ݱ�ͷ����
			//ͷ��ʣ��δ���Ƶĳ���
			int head_remain = HEAD_LENGTH - _recv_head_node->_cur_len;
			memcpy(_recv_head_node->_data + _recv_head_node->_cur_len, _data + copy_len, head_remain);
			//�����Ѵ����data���Ⱥ�ʣ��δ����ĳ���
			copy_len += head_remain;
			bytes_transferred -= head_remain;
			//��ȡͷ������
			short data_len = 0;
			memcpy(&data_len, _recv_head_node->_data, HEAD_LENGTH);
			std::cout << "data_len is " << data_len << std::endl;
			//ͷ�����ȷǷ�
			if (data_len > MAX_LENGTH) {
				std::cout << "invalid data length is " << data_len << std::endl;
				_server->ClearSession(_uuid);
				return;
			}
			_recv_msg_node = std::make_shared<MsgNode>(data_len);

			//��Ϣ�ĳ���С��ͷ���涨�ĳ��ȣ�˵������δ��ȫ�����Ƚ�������Ϣ�ŵ����սڵ���
			if (bytes_transferred < data_len) {
				memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, bytes_transferred);
				_recv_msg_node->_cur_len += bytes_transferred;
				::memset(_data, 0, MAX_LENGTH);
				_socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
					std::bind(&Session::handle_read, this, std::placeholders::_1, std::placeholders::_2, _self_shared));
				//ͷ���Ѿ��������
				_b_head_parse = true;
				return;
			}

			memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, data_len);
			_recv_msg_node->_cur_len += data_len;
			copy_len += data_len;
			bytes_transferred -= data_len;
			_recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';
			std::cout << "receive data is " << _recv_msg_node->_data << std::endl;
			//�˴����Ե���Send���Ͳ���
			Send(_recv_msg_node->_data, _recv_msg_node->_total_len);
			//������ѯʣ��δ��������
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

		//�Ѿ�������ͷ���������ϴ�δ���������Ϣ����
		//���յ������Բ���ʣ��δ�����
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
		//�˴����Ե���Send���Ͳ���
		Send(_recv_msg_node->_data, _recv_msg_node->_total_len);
		//������ѯʣ��δ��������
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
		// Ӧ��ʽ
		//memset(_data, 0, max_length);
		//_socket.async_read_some(boost::asio::buffer(_data, max_length),
		//	std::bind(&Session::handle_read, this, std::placeholders::_1, std::placeholders::_2,_self_shared));
		//ȫ˫����ʽ,��Ϊ���õ���async_write��
		// ��������ص����������˵���Ѿ�д����,�Ϳ���pop��
		std::lock_guard<std::mutex> lock(_send_lock);
		std::cout << "send data " << _send_que.front()->_data + HEAD_LENGTH << std::endl;
		_send_que.pop();
		//�ٴ��ж϶����Ƿ�Ϊ��,��Ϊ�ռ����Ѷ��е�����write
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

