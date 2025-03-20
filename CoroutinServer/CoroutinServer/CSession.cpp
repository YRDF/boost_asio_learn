#include "CSession.h"
#include"CServer.h"

CSession::CSession(boost::asio::io_context& io_contex, CServer* server):_io_contex(io_contex),
_server(server),_socket(io_contex){
	
	boost::uuids::uuid  a_uuid = boost::uuids::random_generator()();
	_uuid = boost::uuids::to_string(a_uuid);
}

boost::asio::ip::tcp::socket& CSession::GetSocket(){
	return _socket;
	// TODO: �ڴ˴����� return ���
}

std::string& CSession::GetUuid(){
	return _uuid;
	// TODO: �ڴ˴����� return ���
}

void CSession::Start(){
	
}
