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
		//生成一个唯一id，绑定连接进来的ip，如果ip重复就踢
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

	//监听客户端
	void Star();
	//粘包测试
	void PrintRecvData(char* data, int length);
	//发送函数
	void Send(char* msg, int max_length);
	void Close();
private:
	//处理数据的回调函数
	void handle_read(const boost::system::error_code &error,
		size_t bytes_transferred,std::shared_ptr<Session> _self_shared);
	void handle_write(const boost::system::error_code& error,std::shared_ptr<Session> _self_shared);

	boost::asio::ip::tcp::socket _socket;
	enum {max_length = 1024*2};
	char _data[max_length];
	Server* _server;
	//uuid
	std::string _uuid;

	//队列存储要发送的数据,保证发送时序
	//我们不能保证发送接口和回调接口在一个线程
	//所以要加一个锁保证发送队列安全
	//发送队列
	std::queue<std::shared_ptr<MsgNode>> _send_que;
	//锁
	std::mutex _send_lock;
	//对收到的消息进行切包
	//收到的消息结构
	std::shared_ptr<MsgNode> _recv_msg_node;
	//表示头部是否解析完成
	bool _b_head_parse;
	//收到的头部结构
	std::shared_ptr<MsgNode> _recv_head_node;
};

//建立连接的server类
class Server {
public:
	Server(boost::asio::io_context& ioc, short port);
	//客户端断开后，把session从map移除
	void ClearSession(std::string uuid);

private:
	//启动描述符用来监听过来的连接
	void start_accept();
	//当对方链接过来会触发回调函数,创建session
	void handle_accpet(std::shared_ptr<Session> new_session,const boost::system::error_code& error);
	//asio不允许ioc拷贝，所以使用引用
	boost::asio::io_context& _ioc;
	boost::asio::ip::tcp::acceptor _acceptor;
	//智能指针实现伪闭包
	//用map管理连接，连接是用智能指针存储的
	std::map<std::string, std::shared_ptr<Session>> _sessions;
};

//记录发送，接收数据，保证全双工的时序性
class MsgNode {
	friend class Session;
public:
	//进一步优化，发送时：添加切包功能,msgnode中存储消息内容长度+消息
	MsgNode(char* msg, short max_len):_total_len(max_len+HEAD_LENGTH),_cur_len(0) {
		_data = new char[_total_len + 1]();
		//先把字节大小存储在前2字节,然后偏移存储数据
		memcpy(_data, &max_len, HEAD_LENGTH);
		memcpy(_data + HEAD_LENGTH, msg, max_len);
		_data[_total_len] = '\0';
	}

	//接收时使用的
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