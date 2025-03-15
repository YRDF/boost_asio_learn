#pragma once
//����һ��session�࣬���session���ʾ����������ͻ������ӵĹ�����
//ͨ�����б�֤Ӧ�ò�ķ���˳��
// ������Session�ж���һ�����Ͷ��У�Ȼ�����¶�����ȷ���첽���ͺ����ͻص�����
#include<memory>
#include<iostream>
#include<boost/asio.hpp>
#include<queue>
const int RECVSIZE = 1024;
class Session{
public:
	Session(std::shared_ptr<boost::asio::ip::tcp::socket>socket);
	//����
	void Connect(const boost::asio::ip::tcp::endpoint& ep);
	//�첽���� �����ǣ������룬д�˶����ַ�����������ָ��ָ��MsgNode��֤�䲻������
	void WriteCallBackErr(const boost::system::error_code& ec,std::size_t bytes_transferred,
	std::shared_ptr<MsgNode> msg_node);
	void WriteToSocketErr(const std::string buf);

	void WrietCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred);
	void WriteToSocket(const std::string buf);

	//�ٴ��Ż� �첽дʹ��async_sendһ�η����⺯���ڲ����ε���async_write_some����
	//��ֻ֤����һ�λص�
	void WriteAllToSocket(const std::string buf);
	void WriteAllCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred);
	
	//�첽������
	void ReadFromSocket();
	void ReadCallBack(const boost::system::error_code&ec,std::size_t bytes_transferred);
	
	//�ٴ��Ż��첽��������ʹ��async_receiveһ�ζ���
	void ReadAllFromSocket();
	void ReadAllCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred);

private:
	//���Ͷ���
	std::queue<std::shared_ptr<MsgNode>> _send_queue;
	//�ж϶����Ƿ�Ϊ��
	bool _send_pending;
	std::shared_ptr<boost::asio::ip::tcp::socket> _socket;
	std::shared_ptr<MsgNode> _send_node;

	//�����ݲ���
	std::shared_ptr<MsgNode> _recv_node;
	bool _recv_pending;
};


//��д����ǰ�������ȷ�װһ��Node�ṹ����������Ҫ���ͺͽ��յ����ݣ�
// �ýṹ�����������׵�ַ�����ݵ��ܳ��ȣ��Լ��Ѿ�����ĳ���(�Ѷ��ĳ��Ȼ�����д�ĳ���)
class MsgNode {
public:
	//�ַ����׵�ַ���ַ�������(��Ϊ���ͽڵ�ʱ�Ĺ��캯��)
	MsgNode(const char* msg, int total_len) :_total_len(total_len),_cur_len(0){
		_msg = new char[_total_len];
		//ԭ��������_msg����
		memcpy(_msg, msg, total_len);
	}

	//��Ϊ���սڵ�ʱ�Ĺ��캯��,ֻ֪��Ҫ���ն೤������
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


