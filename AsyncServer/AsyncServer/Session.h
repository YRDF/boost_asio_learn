#pragma once
#include<iostream>
#include<boost/asio.hpp>
#include<map>
#include<boost/uuid/uuid_generators.hpp>
#include<boost/uuid/uuid_io.hpp>
#include <queue>
#define MAX_LENGTH 1024*2
#define HEAD_LENGTH 2
class Server;
class MsgNode;
class Session:public std::enable_shared_from_this<Session>{
public:
	Session(boost::asio::io_context& ioc,Server* server) :_socket(ioc),_server(server), _b_head_parse(false){
		//����һ��Ψһid�������ӽ�����ip�����ip�ظ�����
		boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
		_uuid = boost::uuids::to_string(a_uuid);
		_recv_head_node = std::make_shared<MsgNode>(HEAD_LENGTH);
	}

	boost::asio::ip::tcp::socket& Socket() {
		return _socket;
	}
	
	const std::string& GetUuid() const{
		return _uuid;
	}

	//�����ͻ���
	void Star();
	//ճ������
	void PrintRecvData(char* data, int length);
	//���ͺ���
	void Send(char* msg, int max_length);
	void Close();
private:
	//�������ݵĻص�����
	void handle_read(const boost::system::error_code &error,
		size_t bytes_transferred,std::shared_ptr<Session> _self_shared);
	void handle_write(const boost::system::error_code& error,std::shared_ptr<Session> _self_shared);

	boost::asio::ip::tcp::socket _socket;
	enum {max_length = 1024*2};
	char _data[max_length];
	Server* _server;
	//uuid
	std::string _uuid;

	//���д洢Ҫ���͵�����,��֤����ʱ��
	//���ǲ��ܱ�֤���ͽӿںͻص��ӿ���һ���߳�
	//����Ҫ��һ������֤���Ͷ��а�ȫ
	//���Ͷ���
	std::queue<std::shared_ptr<MsgNode>> _send_que;
	//��
	std::mutex _send_lock;
	//���յ�����Ϣ�����а�
	//�յ�����Ϣ�ṹ
	std::shared_ptr<MsgNode> _recv_msg_node;
	//��ʾͷ���Ƿ�������
	bool _b_head_parse;
	//�յ���ͷ���ṹ
	std::shared_ptr<MsgNode> _recv_head_node;
};

//�������ӵ�server��
class Server {
public:
	Server(boost::asio::io_context& ioc, short port);
	//�ͻ��˶Ͽ��󣬰�session��map�Ƴ�
	void ClearSession(std::string uuid);

private:
	//����������������������������
	void start_accept();
	//���Է����ӹ����ᴥ���ص�����,����session
	void handle_accpet(std::shared_ptr<Session> new_session,const boost::system::error_code& error);
	//asio������ioc����������ʹ������
	boost::asio::io_context& _ioc;
	boost::asio::ip::tcp::acceptor _acceptor;
	//����ָ��ʵ��α�հ�
	//��map�������ӣ�������������ָ��洢��
	std::map<std::string, std::shared_ptr<Session>> _sessions;
};

//��¼���ͣ��������ݣ���֤ȫ˫����ʱ����
class MsgNode {
	friend class Session;
public:
	//��һ���Ż�������ʱ������а�����,msgnode�д洢��Ϣ���ݳ���+��Ϣ
	MsgNode(char* msg, short max_len):_total_len(max_len+HEAD_LENGTH),_cur_len(0) {
		_data = new char[_total_len + 1]();
		//�Ȱ��ֽڴ�С�洢��ǰ2�ֽ�,Ȼ��ƫ�ƴ洢����
		memcpy(_data, &max_len, HEAD_LENGTH);
		memcpy(_data + HEAD_LENGTH, msg, max_len);
		_data[_total_len] = '\0';
	}

	//����ʱʹ�õ�
	MsgNode(short max_len):_total_len(max_len),_cur_len(0){
		_data = new char[_total_len+1]();
	}

	//MsgNode(char* msg, int max_len) {
	//	_data = new char[max_len];
	//	memcpy(_data, msg, max_len);
	//}
	~MsgNode() {
		delete[]_data;
	}

	void Clear() {
		memset(_data, 0, _total_len);
		_cur_len = 0;
	}

private:
	short _cur_len;
	short _total_len;
	char* _data;
};