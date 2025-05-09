﻿#include <string>
#include <iostream>
#include <memory>
#include <grpcpp/grpcpp.h>
#include "demo.grpc.pb.h"
using grpc::ClientContext;
using grpc::Channel;
using grpc::Status;
using hello::HelloReply;
using hello::HelloRequest;
using hello::Greeter;

// static method : Greeter::NewStub
class FCClient {
public:
    FCClient(std::shared_ptr<Channel> channel)
        :stub_(Greeter::NewStub(channel)) {
    }

    std::string SayHello(std::string name) {
        ClientContext context;
        HelloReply reply;
        HelloRequest request;
        request.set_message(name);

        Status status = stub_->SayHello(&context, request, &reply);

        if (status.ok()) {
            return reply.message();
        }
        else {
            return "failure " + status.error_message();
        }
    }

private:
    std::unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char* argv[]) {
    auto channel = grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials());
    FCClient client(channel);
    // block until get result from RPC server
    std::string result = client.SayHello("hello , llfc.club !");
    printf("get result [%s]\n", result.c_str());
    return 0;
}