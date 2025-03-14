#pragma once
#include<boost/asio.hpp>
#include<iostream>

//�ͻ��˴���һ�����ӶԶ˵Ķ˵㣬����ip���˿�
extern int client_end_point();
//����Ƿ���ˣ���ֻ����ݱ��ص�ַ�󶨾Ϳ�������endpoint
extern int server_end_point();

//�ͻ��˴���socket������������
extern int creat_tcp_socket();
//����������socket����accpet
extern int creat_acceptor_socket();
//�ϰ汾 ��socket�˿ں�ip
extern int bind_acceptor_socket();
//�ͻ������ӷ�������accpetor
extern int connect_to_end();
//������ӵ�������
extern int dns_connect_to_end();
//��������������
extern int accpet_new_connection();
//��boost::asio������Ϣ��Ҫ����һ��buffer
extern void use_const_buffer();
/*���渴�ӵĽṹ�����û�ʹ�ò������ʣ�����asio�����buffer()������
�ú������ն�����ʽ���ֽ������ú�������asio::mutable_buffers_1 ����asio::const_buffers_1�ṹ�Ķ���
������ݸ�buffer()�Ĳ�����һ��ֻ�����ͣ���������asio::const_buffers_1 ���Ͷ��� 
������ݸ�buffer()�Ĳ�����һ����д���ͣ��򷵻�asio::mutable_buffers_1 ���Ͷ��� 
asio::const_buffers_1��asio::mutable_buffers_1��asio::mutable_buffer��asio::const_buffer����������
�ṩ�˷���MutableBufferSequence��ConstBufferSequence����Ľӿڣ�
�������ǿ�����Ϊboost::asio��api�����Ĳ���ʹ�á� 
�򵥸���һ�£����ǿ�����buffer()������������Ҫ�õĻ���洢���ݡ�
����boost�ķ��ͽӿ�sendҪ��Ĳ���ΪConstBufferSequence����*/
//�򵥵Ĵ���buff����
extern void use_buffer_str();
//����������أ�
extern void use_buffer_array();

//boost::asioͬ����д�ķ�ʽ TCP �Ķ�д,ʹ��tcp��socket��Ϊ����,ʹ��write_some
extern void write_to_socket(boost::asio::ip::tcp::socket& sock);

//������ӵ��˿ڽ��з��ͣ�ͬ����ʽ��
extern int send_data_by_write();

//ͬ����ʽʹ��socket��send��֤һ���Է�����
extern int send_data_by_send();

//ͬ�����������������
extern int send_data_write();

//������
extern std::string read_from_socket(boost::asio::ip::tcp::socket& sock);

//
extern int read_data_bye_read_some();

//ȫ�������ٷ���
extern int read_data_bye_receive();

//read
extern int read_data_bye_read();