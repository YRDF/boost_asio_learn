// SyncServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//服务器
#include<boost/asio.hpp>
#include <iostream>
#include<memory>
#include<set>
const int max_length = 1024;
//typedef，把一个已经知道的类型定义成一个名字，以后用这个名字表示前面那一串
typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
//定义线程集合，存储线程
std::set<std::shared_ptr<std::thread>> thread_set;

//服务器处理客户端的读写
// 因为是阻塞的读写方式，所以如果放在主线程就会阻塞
//所以，该函数跑在线程中，每有一个连接就开辟一个线程
//该函数跑在线程
//处理连接客户端的数据
void session(socket_ptr sock) {
	try {
		//循环处理客户端读和写
		for (;;) {
			char data[max_length];
			//清空char数组
			memset(data, '\0', max_length);
			boost::system::error_code error;
			//read读取到max_length这么长才返回,一直等待
			//size_t length = boost::asio::read(sock, boost::asio::buffer(data, max_length), error);
			//使用read_some部分读取
			size_t length = sock->read_some(boost::asio::buffer(data,max_length),error);
			//eof表示对端关闭
			if (error == boost::asio::error::eof) {
				std::cout << "connection closed by peer" << std::endl;
				break;
			}
			else if (error) {
				throw boost::system::system_error(error);
			}
			//sock->remote_endpoint函数返回boost::asio::ip::tcp::endpoint对象
			std::cout << "receive from" << sock->remote_endpoint().address().to_string() << std::endl;
			std::cout << "receive message is " << data << std::endl;

			//收到的数据回传给对客户端
			boost::asio::write(*sock, boost::asio::buffer(data, length));
		}
	}catch (std::exception& e) {
		std::cerr << "Exception in thread: " << e.what() << "\n" << std::endl;
	}
}

//接收客户端连接
void server(boost::asio::io_context& ioc, unsigned short port) {
	//定义一个accpetor用来接收,接收一个ioc和一个端点
	boost::asio::ip::tcp::acceptor a(ioc, boost::asio::ip::tcp::endpoint(
		boost::asio::ip::tcp::v4(), port));
	//循环接收，只要来客户端就接收
	for (;;) {
		//生成socket请求
		socket_ptr socket(new boost::asio::ip::tcp::socket(ioc));
		//对端来连接了，就触发
		a.accept(*socket);
		//在线程中处理对端的连接
		auto t = std::make_shared<std::thread>(session, socket);
		//但是，线程可能还没来得及跑，循环就进入下一轮了
		// 局部智能指针t就会回收，线程可能就来不及做session工作
		//所以我们把线程加入到集合中，使其引用计数加1，保证session不会被回收
		thread_set.insert(t);
	}
}

int main(){
	try {
		boost::asio::io_context ioc;
		server(ioc, 10086);
		//如果acceptor意外退出了，在主线程要等待所有子线程都退出再退出
		for (auto& t : thread_set) {
			t->join();
		}
	}catch (std::exception&e) {
		std::cerr << "error " << e.what() << std::endl;
	}
	return 0;
}
