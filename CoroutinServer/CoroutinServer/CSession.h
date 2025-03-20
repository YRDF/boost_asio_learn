#pragma once
#include<boost/asio.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

class CServer;
class CSession:public std::enable_shared_from_this<CSession>{
public:
	CSession(boost::asio::io_context& io_contex,CServer* server);
	boost::asio::ip::tcp::socket& GetSocket();
	std::string& GetUuid();
	void Start();
private:
	boost::asio::io_context& _io_contex;
	CServer* _server;
	boost::asio::ip::tcp::socket _socket;
	std::string _uuid;
};

