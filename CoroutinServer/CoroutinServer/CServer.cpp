#include "CServer.h"
#include"AsioIOServicePool.h"

CServer::CServer(boost::asio::io_context& io_contex, short port):
	_io_contex(io_contex),port(port),
	_acceptor(io_contex,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),port)){
	StartAccept();
}

CServer::~CServer(){
	std::cout<< "CServer destructed!" << std::endl;
}

void CServer::ClearSession(std::string uuid){
	std::lock_guard<std::mutex> lock(_mutex);
	_sessions.erase(uuid);
}

void CServer::HandleAccept(std::shared_ptr<CSession> new_session, const boost::system::error_code& error){
	if (!error) {
		new_session->Start();
		std::lock_guard<std::mutex> lock(_mutex);
		_sessions.insert(std::make_pair(new_session->GetUuid(), new_session));
	}
	else {
		std::cout << "session accept failed, error is " << error.what() << std::endl;
	}

	StartAccept();
}

void CServer::StartAccept(){
	auto& io_contex = AsioIOServicePool::GetInstance().GetIOService();
	std::shared_ptr<CSession> new_session = std::make_shared<CSession>(io_contex, this);
	_acceptor.async_accept(new_session->GetSocket(),std::bind(
	&CServer::HandleAccept,this,new_session,std::placeholders::_1));
}
