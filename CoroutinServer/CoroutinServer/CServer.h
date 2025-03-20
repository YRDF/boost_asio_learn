#pragma once
#include<memory>
#include<map>
#include<mutex>
#include<boost/asio.hpp>
#include"CSession.h"
#include<iostream>


class CServer{
public:
	CServer(boost::asio::io_context& io_contex,short port);
	~CServer();
	void ClearSession(std::string uuid);
private:
	void HandleAccept(std::shared_ptr<CSession> new_session,const boost::system::error_code& error);
	void StartAccept();


	boost::asio::io_context& _io_contex;
	short port;
	boost::asio::ip::tcp::acceptor _acceptor;
	std::map<std::string, std::shared_ptr<CSession>> _sessions;
	std::mutex _mutex;
};

