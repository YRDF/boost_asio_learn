#pragma once
//定义一个session类，这个session类表示服务器处理客户端连接的管理类
//通过队列保证应用层的发送顺序。
// 我们在Session中定义一个发送队列，然后重新定义正确的异步发送函数和回调处理
#include<memory>
#include<iostream>
#include<boost/asio.hpp>
#include<queue>
const int RECVSIZE = 1024;
class Session{
public:
	Session(std::shared_ptr<boost::asio::ip::tcp::socket>socket);
	//连接
	void Connect(const boost::asio::ip::tcp::endpoint& ep);
	//异步函数 参数是：错误码，写了多少字符，传递智能指针指向MsgNode保证其不被析构
	void WriteCallBackErr(const boost::system::error_code& ec,std::size_t bytes_transferred,
	std::shared_ptr<MsgNode> msg_node);
	void WriteToSocketErr(const std::string buf);

	void WrietCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred);
	void WriteToSocket(const std::string buf);

	//再次优化 异步写使用async_send一次发完这函数内部会多次调用async_write_some函数
	//保证只调用一次回调
	void WriteAllToSocket(const std::string buf);
	void WriteAllCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred);
	
	//异步读操作
	void ReadFromSocket();
	void ReadCallBack(const boost::system::error_code&ec,std::size_t bytes_transferred);
	
	//再次优化异步读操作，使用async_receive一次读完
	void ReadAllFromSocket();
	void ReadAllCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred);

private:
	//发送队列
	std::queue<std::shared_ptr<MsgNode>> _send_queue;
	//判断队列是否为空
	bool _send_pending;
	std::shared_ptr<boost::asio::ip::tcp::socket> _socket;
	std::shared_ptr<MsgNode> _send_node;

	//读数据参数
	std::shared_ptr<MsgNode> _recv_node;
	bool _recv_pending;
};


//在写操作前，我们先封装一个Node结构，用来管理要发送和接收的数据，
// 该结构包含数据域首地址，数据的总长度，以及已经处理的长度(已读的长度或者已写的长度)
class MsgNode {
public:
	//字符串首地址，字符串长度(作为发送节点时的构造函数)
	MsgNode(const char* msg, int total_len) :_total_len(total_len),_cur_len(0){
		_msg = new char[_total_len];
		//原串拷贝到_msg串中
		memcpy(_msg, msg, total_len);
	}

	//作为接收节点时的构造函数,只知道要接收多长的数据
	MsgNode(int total_len) :_total_len(total_len), _cur_len(0) {
		_msg = new char[_total_len];
	}

	~MsgNode() {
		delete[]_msg;
	}

	int _cur_len;
	char* _msg;
	int _total_len;
};


