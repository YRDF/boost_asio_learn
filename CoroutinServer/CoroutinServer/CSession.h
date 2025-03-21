#pragma once
#include<boost/asio.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include<boost/asio/co_spawn.hpp>
#include<boost/asio/detached.hpp>
#include"const.h"
#include<queue>
#include<mutex>
#include<memory>
#include"MsgNode.h"

using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
using boost::asio::strand;
using boost::asio::io_context;

namespace this_coro = boost::asio::this_coro;

class CServer;
class CSession:public std::enable_shared_from_this<CSession>{
public:
	CSession(boost::asio::io_context& io_contex,CServer* server);
	~CSession();
	boost::asio::ip::tcp::socket& GetSocket();
	std::string& GetUuid();
	void Start();
	void Close();
	void Send(const char* msg, short max_length, short msgid);
	void Send(std::string msg, short msgid);
private:
	void HandleWrite(const boost::system::error_code& error, std::shared_ptr<CSession> shared_self);
	boost::asio::io_context& _io_contex;
	CServer* _server;
	boost::asio::ip::tcp::socket _socket;
	std::string _uuid;
	bool _b_close;
	std::queue<std::shared_ptr<SendNode> > _send_que;
	//发送互斥量
	std::mutex _send_lock;
	//收到的消息结构
	std::shared_ptr<RecvNode> _recv_msg_node;
	bool _b_head_parse;
	//收到的头部结构
	std::shared_ptr<MsgNode> _recv_head_node;
};

class LogicNode {
	friend class LogicSystem;
public:
	LogicNode(std::shared_ptr<CSession> session, std::shared_ptr<RecvNode> recvnode);
private:
	std::shared_ptr<CSession> _session;
	std::shared_ptr<RecvNode> _recvnode;
};
