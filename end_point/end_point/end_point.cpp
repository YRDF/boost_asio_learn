// end_point.cpp : 端点方面的api
//学习服务器如何绑定端口，客户端如何连接端点

//
#include <iostream>
#include"endpoint.h"

int main()
{
    //客户端操作
    client_end_point();
    //如果是服务端，则只需根据本地地址绑定就可以生成endpoint
    std::cout << "Hello World!\n";
    return 0;
}
