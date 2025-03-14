#pragma once
#include<boost/asio.hpp>
#include<iostream>

//客户端创建一个连接对端的端点，包含ip，端口
extern int client_end_point();
//如果是服务端，则只需根据本地地址绑定就可以生成endpoint
extern int server_end_point();

//客户端创建socket用来创建连接
extern int creat_tcp_socket();
//服务器生成socket用来accpet
extern int creat_acceptor_socket();
//老版本 绑定socket端口和ip
extern int bind_acceptor_socket();
//客户端连接服务器的accpetor
extern int connect_to_end();
//如果连接的是域名
extern int dns_connect_to_end();
//服务器接收连接
extern int accpet_new_connection();
//给boost::asio传递消息需要构造一个buffer
extern void use_const_buffer();
/*上面复杂的结构交给用户使用并不合适，所以asio提出了buffer()函数，
该函数接收多种形式的字节流，该函数返回asio::mutable_buffers_1 或者asio::const_buffers_1结构的对象。
如果传递给buffer()的参数是一个只读类型，则函数返回asio::const_buffers_1 类型对象。 
如果传递给buffer()的参数是一个可写类型，则返回asio::mutable_buffers_1 类型对象。 
asio::const_buffers_1和asio::mutable_buffers_1是asio::mutable_buffer和asio::const_buffer的适配器，
提供了符合MutableBufferSequence和ConstBufferSequence概念的接口，
所以他们可以作为boost::asio的api函数的参数使用。 
简单概括一下，我们可以用buffer()函数生成我们要用的缓存存储数据。
比如boost的发送接口send要求的参数为ConstBufferSequence类型*/
//简单的传递buff方法
extern void use_buffer_str();
//如果有数组呢？
extern void use_buffer_array();

//boost::asio同步读写的方式 TCP 的读写,使用tcp的socket作为参数,使用write_some
extern void write_to_socket(boost::asio::ip::tcp::socket& sock);

//如何连接到端口进行发送？同步方式：
extern int send_data_by_write();

//同步方式使用socket的send保证一次性发送完
extern int send_data_by_send();

//同步向服务器发送数据
extern int send_data_write();

//读数据
extern std::string read_from_socket(boost::asio::ip::tcp::socket& sock);

//
extern int read_data_bye_read_some();

//全部读完再返回
extern int read_data_bye_receive();

//read
extern int read_data_bye_read();