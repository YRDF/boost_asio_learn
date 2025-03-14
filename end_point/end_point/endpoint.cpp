#include"endpoint.h"

int client_end_point()
{
	//�Զ˵�ip��ַ�Ͷ˿ں�
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;

	//asio�Ĵ���ؼ���
	boost::system::error_code ec;
	//ͨ��asioҪ�õ�ip��ַ����
	boost::asio::ip::address ip_address = boost::asio::ip::make_address(raw_ip_address, ec);

	if (ec.value() != 0) {
		std::cout << "error! failed to parse the IP address! error code = " << ec.value() <<
			" .message is" << ec.message() << std::endl;
		return ec.value();
	}

	//���ɶ˿�,�Ժ�ͻ��˿���ͨ��ep����
	boost::asio::ip::tcp::endpoint ep(ip_address, port_num);

	return 0;
}

int server_end_point(){

	unsigned short port_num = 3333;
	//�������ĵ�ַ�Լ�֪�� any��ʾ�κε�ַ�����Ժͷ���������ͨ��
	boost::asio::ip::address ip_address = boost::asio::ip::address_v6::any();
	//���ɶ˵�
	boost::asio::ip::tcp::endpoint ep(ip_address,port_num);
	return 0;
}

int creat_tcp_socket(){
	//��һ��socket����ͨѶʱ������Ҫ��������.����socketʱ��������ioc����ȥ��
	// ����asio��socket�����ĸ������ķ���
	boost::asio::io_context ioc;
	//����һ��ipv4Э��,����socket
	boost::asio::ip::tcp protocol = boost::asio::ip::tcp::v4();
	boost::asio::ip::tcp::socket sock(ioc);
	//�����ɵ�socket�����Ƿ����ɳɹ�
	boost::system::error_code ec;
	sock.open(protocol, ec);
	if (ec.value() != 0) {
		std::cout << "error! failed to parse the IP address! error code = " << ec.value() <<
			" .message is" << ec.message() << std::endl;
		return ec.value();
	}

	return 0;
}

int creat_acceptor_socket(){
	//���������ģ��������İ󶨵�acceptor���������������������ӵ�acceptor
	boost::asio::io_context ios;
	//boost::asio::ip::tcp::acceptor acceptor(ios);
	//�µ�д������accpetor��ָ��Ϊtcp::v4Э�飬���з����ұ��ض˿�3333������
	//�µ�acceptor���Զ���
	boost::asio::ip::tcp::acceptor a(ios, 
		boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 3333));

	return 0;
}

int bind_acceptor_socket(){
	//�����˿ںţ����ɶ˵�
	unsigned short port_num = 3333;
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), port_num);
	//��������acceptor�ǰ����ĸ�����
	boost::asio::io_context ios;
	boost::asio::ip::tcp::acceptor acceptor(ios,ep.protocol());
	//�ֶ���
	boost::system::error_code ec;
	acceptor.bind(ep, ec);

	if (ec.value() != 0) {
		std::cout << "error! failed to parse the IP address! error code = " << ec.value() <<
			" .message is" << ec.message() << std::endl;
		return ec.value();
	}

	return 0;
}

int connect_to_end(){
	//����֪����������ַ�Ͷ˿�
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;

	//���ӷ�����
	try{
		//�����˵�
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::make_address(raw_ip_address), port_num);
		boost::asio::io_context ios;
		boost::asio::ip::tcp::socket sock(ios,ep.protocol());
		//��������˵�
		sock.connect(ep);
	}catch (boost::system::system_error&e){
		std::cout << "error! failed to parse the IP address! error code = " << e.code() <<
			" .message is" << e.what() << std::endl;
		return e.code().value();
	}

	return 0;
}

//����������ʵ������
int dns_connect_to_end(){
	std::string host = "llfc.club";
	std::string port = "3333";

	//boost::asio::io_context ios;
	//dns������
	// �ɰ汾
	//boost::asio::ip::tcp::resolver::query resolver_query(host,port,boost::asio::ip::tcp::resolver::query::numeric_service);
	
	//boost::asio::ip::tcp::resolver resolver(ios);

	try {
		// ���� io_context ����
		boost::asio::io_context io_context;
		// ���� resolver ����
		boost::asio::ip::tcp::resolver resolver(io_context);
		// ���������Ͷ˿�
		boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, port);
		// �����������
		for (const auto& endpoint : endpoints) {
			std::cout << "Resolved endpoint: " << endpoint.endpoint() << std::endl;
		}
	}
	catch (boost::system::system_error& e) {
		std::cout << "error! failed to parse the IP address! error code = " << e.code() <<
			" .message is" << e.what() << std::endl;
		return e.code().value();
	}


	return 0;
}

int accpet_new_connection(){
	//�������еĴ�С30�����������������ӣ���������ͨ��accpet���ش����µ�����ȥ����ͻ��˵�����ʱ
	// �ͷ��ڻ���������
	const int BACKLOG_SIZE = 30;
	unsigned short port_num = 3333;
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), port_num);
	boost::asio::io_context ios;

	try {
		boost::asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
		//��������
		acceptor.bind(ep);
		//�����µ�����
		acceptor.listen(BACKLOG_SIZE);
		//����һ��socket,�������ͻ���ͨ��
		boost::asio::ip::tcp::socket sock(ios);
		//acceptor�����µ����ӽ���sock����
		acceptor.accept(sock);
	}
	catch (boost::system::system_error& e) {
		std::cout << "error! failed to parse the IP address! error code = " << e.code() <<
			" .message is" << e.what() << std::endl;
		return e.code().value();
	}

	return 0;
}

void use_const_buffer(){
	std::string buf = "hello world";
	//ת��Ϊbuffer,��һ������Ϊ�ڴ��׵�ַ���ڶ�������Ϊ����
	boost::asio::const_buffer asio_buf(buf.c_str(), buf.length());
	//����buffer����ֱ�Ӵ���send����Ҫ�������һ��vector����
	std::vector<boost::asio::const_buffer> buffers_sequence;
	buffers_sequence.emplace_back(asio_buf);
	//����send�Ϳ���ֱ�Ӵ��� asio::send(buffers_sequence);
}

void use_buffer_str(){
	boost::asio::const_buffer output_buf = boost::asio::buffer("hello world");
}

void use_buffer_array(){
	const size_t BUF_SIZE_BYTES = 20;
	std::unique_ptr<char[]>buf(new char[BUF_SIZE_BYTES]);
	//uniue_ptr��buf���ת��buffer�ṹ��ʹ��get()ȡ��ԭָ�룬ת����void*���͡�
	auto input_buf = boost::asio::buffer(static_cast<void*>(buf.get()),BUF_SIZE_BYTES);
}

void write_to_socket(boost::asio::ip::tcp::socket& sock){
	std::string buf = "Hello world!";
	//�ܹ�д�˶�������
	std::size_t total_bytes_written = 0;
	//ѭ������
	//write_some ����ÿ��д����ֽ���
	while (total_bytes_written != buf.length()) {
		total_bytes_written += sock.write_some(boost::asio::buffer(
			buf.c_str()+total_bytes_written,buf.length()-total_bytes_written));
	}
}

int send_data_by_write(){
	//�Զ˵�ַ���˿ں�
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;
	//�ͻ��˷�������
	try {
		//�˵㣬�����ģ�socket�Ĵ���
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::make_address(raw_ip_address), port_num);
		boost::asio::io_context ios;
		boost::asio::ip::tcp::socket soc(ios,ep.protocol());

		//����
		soc.connect(ep);
		//��������
		write_to_socket(soc);

	}catch (boost::system::system_error &e) {
		std::cout << "error! failed to parse the IP address! error code = " << e.code() <<
			" .message is" << e.what() << std::endl;
		return e.code().value();
	}
	return 0;
}

int send_data_by_send(){
	//�Զ˵�ַ���˿ں�
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;
	//�ͻ��˷�������
	try {
		//�˵㣬�����ģ�socket�Ĵ���
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::make_address(raw_ip_address), port_num);
		boost::asio::io_context ios;
		boost::asio::ip::tcp::socket soc(ios, ep.protocol());

		//����
		soc.connect(ep);

		std::string buf = "Hello world!";
		//socket��send����������Ѵ��ݵ�ȫ�����꣬tcp������ֱ��ȫ������Ϊֹ
		int send_length = soc.send(boost::asio::buffer(buf.c_str(), buf.length()));
		if (send_length <= 0) {
			return 0;
		}
	}
	catch (boost::system::system_error& e) {
		std::cout << "error! failed to parse the IP address! error code = " << e.code() <<
			" .message is" << e.what() << std::endl;
		return e.code().value();
	}
	return 0;
}


int send_data_write() {
	//�Զ˵�ַ���˿ں�
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;
	//�ͻ��˷�������
	try {
		//�˵㣬�����ģ�socket�Ĵ���
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::make_address(raw_ip_address), port_num);
		boost::asio::io_context ios;
		boost::asio::ip::tcp::socket soc(ios, ep.protocol());

		//����
		soc.connect(ep);

		std::string buf = "Hello world!";
		//write����
		int send_length = boost::asio::write(soc, boost::asio::buffer(buf.c_str(), buf.length()));
		if (send_length <= 0) {
			return 0;
		}
	}
	catch (boost::system::system_error& e) {
		std::cout << "error! failed to parse the IP address! error code = " << e.code() <<
			" .message is" << e.what() << std::endl;
		return e.code().value();
	}
	return 0;
}

std::string read_from_socket(boost::asio::ip::tcp::socket& sock){
	//��Ϣ
	const unsigned char MESSAGE_SIZE = 7;
	char buf[MESSAGE_SIZE];
	//��ȡ�ֽ���
	std::size_t total_bytes_read = 0;
	while (total_bytes_read!=MESSAGE_SIZE) {
		total_bytes_read += sock.read_some(boost::asio::buffer(
			buf + total_bytes_read, MESSAGE_SIZE - total_bytes_read));
	}
	//���ض�ȡ��������
	return std::string(buf,total_bytes_read);
}

int read_data_bye_read_some(){
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;

	try {
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::make_address(raw_ip_address), port_num);
		boost::asio::io_context ioc;
		boost::asio::ip::tcp::socket soc(ioc, ep.protocol());

		soc.connect(ep);
		//������
		read_from_socket(soc);

	}
	catch (boost::system::system_error& e) {
		std::cout << "error! failed to parse the IP address! error code = " << e.code() <<
			" .message is" << e.what() << std::endl;
		return e.code().value();
	}

	return 0;
}

int read_data_bye_receive(){
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;

	try {
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::make_address(raw_ip_address), port_num);
		boost::asio::io_context ioc;
		boost::asio::ip::tcp::socket soc(ioc, ep.protocol());

		soc.connect(ep);
		//������,һ���Զ���
		const unsigned char BUFF_SIZE = 7;
		char buffer_receive[BUFF_SIZE];
		int receive_length = soc.receive(boost::asio::buffer(buffer_receive,BUFF_SIZE));
		if (receive_length <= 0) {
			return 0;
		}
	}
	catch (boost::system::system_error& e) {
		std::cout << "error! failed to parse the IP address! error code = " << e.code() <<
			" .message is" << e.what() << std::endl;
		return e.code().value();
	}
	return 0;
}

int read_data_bye_read(){
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;

	try {
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::make_address(raw_ip_address), port_num);
		boost::asio::io_context ioc;
		boost::asio::ip::tcp::socket soc(ioc, ep.protocol());

		soc.connect(ep);
		//������,һ���Զ���
		const unsigned char BUFF_SIZE = 7;
		char buffer_receive[BUFF_SIZE];
		int receive_length = boost::asio::read(soc,boost::asio::buffer(buffer_receive,BUFF_SIZE));
		if (receive_length <= 0) {
			return 0;
		}
	}
	catch (boost::system::system_error& e) {
		std::cout << "error! failed to parse the IP address! error code = " << e.code() <<
			" .message is" << e.what() << std::endl;
		return e.code().value();
	}
	return 0;
}
