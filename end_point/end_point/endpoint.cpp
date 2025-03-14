#include"endpoint.h"

int client_end_point()
{
	//对端的ip地址和端口号
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;

	//asio的错误关键字
	boost::system::error_code ec;
	//通信asio要用的ip地址类型
	boost::asio::ip::address ip_address = boost::asio::ip::make_address(raw_ip_address, ec);

	if (ec.value() != 0) {
		std::cout << "error! failed to parse the IP address! error code = " << ec.value() <<
			" .message is" << ec.message() << std::endl;
		return ec.value();
	}

	//生成端口,以后客户端可以通过ep连接
	boost::asio::ip::tcp::endpoint ep(ip_address, port_num);

	return 0;
}

int server_end_point(){

	unsigned short port_num = 3333;
	//服务器的地址自己知道 any表示任何地址都可以和服务器进行通信
	boost::asio::ip::address ip_address = boost::asio::ip::address_v6::any();
	//生成端点
	boost::asio::ip::tcp::endpoint ep(ip_address,port_num);
	return 0;
}

int creat_tcp_socket(){
	//当一个socket用来通讯时，必须要有上下文.生成socket时把上下文ioc传进去，
	// 告诉asio，socket属于哪个上下文服务。
	boost::asio::io_context ioc;
	//创建一个ipv4协议,生成socket
	boost::asio::ip::tcp protocol = boost::asio::ip::tcp::v4();
	boost::asio::ip::tcp::socket sock(ioc);
	//打开生成的socket，看是否生成成功
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
	//创建上下文，把上下文绑定到acceptor，就生成了用来接收连接的acceptor
	boost::asio::io_context ios;
	//boost::asio::ip::tcp::acceptor acceptor(ios);
	//新的写法生成accpetor，指定为tcp::v4协议，所有发往我本地端口3333的连接
	//新的acceptor会自动绑定
	boost::asio::ip::tcp::acceptor a(ios, 
		boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 3333));

	return 0;
}

int bind_acceptor_socket(){
	//创建端口号，生成端点
	unsigned short port_num = 3333;
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), port_num);
	//创建服务acceptor是绑定在哪个服务
	boost::asio::io_context ios;
	boost::asio::ip::tcp::acceptor acceptor(ios,ep.protocol());
	//手动绑定
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
	//我们知道服务器地址和端口
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;

	//连接服务器
	try{
		//创建端点
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::make_address(raw_ip_address), port_num);
		boost::asio::io_context ios;
		boost::asio::ip::tcp::socket sock(ios,ep.protocol());
		//连接这个端点
		sock.connect(ep);
	}catch (boost::system::system_error&e){
		std::cout << "error! failed to parse the IP address! error code = " << e.code() <<
			" .message is" << e.what() << std::endl;
		return e.code().value();
	}

	return 0;
}

//解析域名，实现链接
int dns_connect_to_end(){
	std::string host = "llfc.club";
	std::string port = "3333";

	//boost::asio::io_context ios;
	//dns解析器
	// 旧版本
	//boost::asio::ip::tcp::resolver::query resolver_query(host,port,boost::asio::ip::tcp::resolver::query::numeric_service);
	
	//boost::asio::ip::tcp::resolver resolver(ios);

	try {
		// 创建 io_context 对象
		boost::asio::io_context io_context;
		// 创建 resolver 对象
		boost::asio::ip::tcp::resolver resolver(io_context);
		// 解析域名和端口
		boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, port);
		// 遍历解析结果
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
	//监听队列的大小30个，服务器监听连接，当来不及通过accpet返回创建新的连接去处理客户端的连接时
	// 就放在缓冲区队列
	const int BACKLOG_SIZE = 30;
	unsigned short port_num = 3333;
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), port_num);
	boost::asio::io_context ios;

	try {
		boost::asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
		//接收器绑定
		acceptor.bind(ep);
		//监听新的连接
		acceptor.listen(BACKLOG_SIZE);
		//创建一个socket,用来跟客户端通信
		boost::asio::ip::tcp::socket sock(ios);
		//acceptor接收新的连接交给sock处理
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
	//转换为buffer,第一个参数为内存首地址，第二个参数为长度
	boost::asio::const_buffer asio_buf(buf.c_str(), buf.length());
	//这种buffer不能直接传给send，需要把其存入一个vector容器
	std::vector<boost::asio::const_buffer> buffers_sequence;
	buffers_sequence.emplace_back(asio_buf);
	//调用send就可以直接传入 asio::send(buffers_sequence);
}

void use_buffer_str(){
	boost::asio::const_buffer output_buf = boost::asio::buffer("hello world");
}

void use_buffer_array(){
	const size_t BUF_SIZE_BYTES = 20;
	std::unique_ptr<char[]>buf(new char[BUF_SIZE_BYTES]);
	//uniue_ptr的buf如何转成buffer结构？使用get()取出原指针，转换成void*类型。
	auto input_buf = boost::asio::buffer(static_cast<void*>(buf.get()),BUF_SIZE_BYTES);
}

void write_to_socket(boost::asio::ip::tcp::socket& sock){
	std::string buf = "Hello world!";
	//总共写了多少数据
	std::size_t total_bytes_written = 0;
	//循环发送
	//write_some 返回每次写入的字节数
	while (total_bytes_written != buf.length()) {
		total_bytes_written += sock.write_some(boost::asio::buffer(
			buf.c_str()+total_bytes_written,buf.length()-total_bytes_written));
	}
}

int send_data_by_write(){
	//对端地址，端口号
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;
	//客户端发送数据
	try {
		//端点，上下文，socket的创建
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::make_address(raw_ip_address), port_num);
		boost::asio::io_context ios;
		boost::asio::ip::tcp::socket soc(ios,ep.protocol());

		//连接
		soc.connect(ep);
		//发送数据
		write_to_socket(soc);

	}catch (boost::system::system_error &e) {
		std::cout << "error! failed to parse the IP address! error code = " << e.code() <<
			" .message is" << e.what() << std::endl;
		return e.code().value();
	}
	return 0;
}

int send_data_by_send(){
	//对端地址，端口号
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;
	//客户端发送数据
	try {
		//端点，上下文，socket的创建
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::make_address(raw_ip_address), port_num);
		boost::asio::io_context ios;
		boost::asio::ip::tcp::socket soc(ios, ep.protocol());

		//连接
		soc.connect(ep);

		std::string buf = "Hello world!";
		//socket的send函数，让其把传递的全部发完，tcp阻塞，直到全部发完为止
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
	//对端地址，端口号
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;
	//客户端发送数据
	try {
		//端点，上下文，socket的创建
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::make_address(raw_ip_address), port_num);
		boost::asio::io_context ios;
		boost::asio::ip::tcp::socket soc(ios, ep.protocol());

		//连接
		soc.connect(ep);

		std::string buf = "Hello world!";
		//write函数
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
	//消息
	const unsigned char MESSAGE_SIZE = 7;
	char buf[MESSAGE_SIZE];
	//读取字节数
	std::size_t total_bytes_read = 0;
	while (total_bytes_read!=MESSAGE_SIZE) {
		total_bytes_read += sock.read_some(boost::asio::buffer(
			buf + total_bytes_read, MESSAGE_SIZE - total_bytes_read));
	}
	//返回读取到的数据
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
		//读数据
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
		//读数据,一次性读完
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
		//读数据,一次性读完
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
