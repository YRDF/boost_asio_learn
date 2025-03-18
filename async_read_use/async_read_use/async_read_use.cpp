// async_read_use.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//使用async_read指定读取字节数，减少切包步骤
//至少读取指定字节数，然后才能触发回调函数

#include <iostream>
#include "CServer.h"
using namespace std;

int main()
{
    try {
        boost::asio::io_context  io_context;
        CServer s(io_context, 10086);
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << endl;
    }
    boost::asio::io_context io_context;
}

