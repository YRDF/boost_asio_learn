//前面我们学习了boost::asio同步读写的api函数，
// 现在将前面的api串联起来，做一个能跑起来的客户端和服务器。 
// 客户端和服务器采用阻塞的同步读写方式完成通信

//客户端设计基本思路是根据服务器对端的ip和端口创建一个endpoint，
// 然后创建socket连接这个endpoint，之后就可以用同步读写的方式发送和接收数据了。
// 2025年3月16日
//再次改写同步客户端，让其按照服务器切包的规则发送数据
#include <iostream>
#include<boost/asio.hpp>
#include<thread>

//最大发送和接收长度
const int MAX_LENGTH = 1024;
const int HEAD_LENGTH = 2;
int main(){

	try {
		//创建上下文服务
		boost::asio::io_context ioc;
		//构造endpoint
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::make_address("127.0.0.1"), 10086);
		//创建socket
		boost::asio::ip::tcp::socket sock(ioc);
		//错误
		boost::system::error_code error = boost::asio::error::host_not_found;
		//连接对端
		sock.connect(ep,error);
		if (error) {
			std::cout << "connect failed,code is " << error.value() << "error msg is: " << error.message()
				<< std::endl;
		}
		std::cout << "Enter message: ";
		//接收要发送的信息,通过一行一行的读取，输入回都会把数据写人数组
		char request[MAX_LENGTH];
		std::cin.getline(request, MAX_LENGTH);
		////读取输入的数据有多长
		//size_t request_length = strlen(request);
		////同步写，一次性发送完
		//boost::asio::write(sock, boost::asio::buffer(request, request_length));
		//
		////接收服务器发送过来的的信息
		//char reply[MAX_LENGTH];
		//size_t reply_length = boost::asio::read(sock, boost::asio::buffer(reply,request_length));
		//std::cout << "Reply is: ";
		//std::cout.write(reply, reply_length);
		//std::cout << "\n";
		
		
		//切包优化
		//size_t request_length = strlen(request);
		//char send_data[MAX_LENGTH] = { 0 };
		//memcpy(send_data, &request_length, 2);
		//memcpy(send_data + 2, request, request_length);
		//boost::asio::write(sock, boost::asio::buffer(send_data, request_length + 2));

		//char reply_head[HEAD_LENGTH];
		//size_t reply_length = boost::asio::read(sock, boost::asio::buffer(reply_head, HEAD_LENGTH));
		//short msglen = 0;
		//memcpy(&msglen, reply_head, HEAD_LENGTH);
		//char msg[MAX_LENGTH] = { 0 };
		//size_t  msg_length = boost::asio::read(sock, boost::asio::buffer(msg, msglen));

		//std::cout << "Reply is: ";
		//std::cout.write(msg, msglen) << std::endl;
		//std::cout << "Reply len is " << msglen;
		//std::cout << "\n";

		//收发分离测试粘包
		std::thread send_thread([&sock] {
			for (;;) {
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
				const char* request = "hello world!";
				size_t request_length = strlen(request);
				char send_data[MAX_LENGTH] = { 0 };
				memcpy(send_data, &request_length, 2);
				memcpy(send_data + 2, request, request_length);
				boost::asio::write(sock, boost::asio::buffer(send_data, request_length + 2));
			}
			});

		std::thread recv_thread([&sock] {
			for (;;) {
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
				std::cout << "begin to receive..." << std::endl;
				char reply_head[HEAD_LENGTH];
				size_t reply_length = boost::asio::read(sock, boost::asio::buffer(reply_head, HEAD_LENGTH));
				short msglen = 0;
				memcpy(&msglen, reply_head, HEAD_LENGTH);
				char msg[MAX_LENGTH] = { 0 };
				size_t  msg_length = boost::asio::read(sock, boost::asio::buffer(msg, msglen));

				std::cout << "Reply is: ";
				std::cout.write(msg, msglen) << std::endl;
				std::cout << "Reply len is " << msglen;
				std::cout << "\n";
			}
			});

		send_thread.join();
		recv_thread.join();
	}catch(std::exception& e){
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	return 0;
}

