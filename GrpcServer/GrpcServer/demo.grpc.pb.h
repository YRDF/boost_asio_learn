// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: demo.proto
#ifndef GRPC_demo_2eproto__INCLUDED
#define GRPC_demo_2eproto__INCLUDED

#include "demo.pb.h"

#include <functional>
#include <grpc/impl/codegen/port_platform.h>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace hello {

class Greeter final {
 public:
  static constexpr char const* service_full_name() {
    return "hello.Greeter";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status SayHello(::grpc::ClientContext* context, const ::hello::HelloRequest& request, ::hello::HelloReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::hello::HelloReply>> AsyncSayHello(::grpc::ClientContext* context, const ::hello::HelloRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::hello::HelloReply>>(AsyncSayHelloRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::hello::HelloReply>> PrepareAsyncSayHello(::grpc::ClientContext* context, const ::hello::HelloRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::hello::HelloReply>>(PrepareAsyncSayHelloRaw(context, request, cq));
    }
    class experimental_async_interface {
     public:
      virtual ~experimental_async_interface() {}
      virtual void SayHello(::grpc::ClientContext* context, const ::hello::HelloRequest* request, ::hello::HelloReply* response, std::function<void(::grpc::Status)>) = 0;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void SayHello(::grpc::ClientContext* context, const ::hello::HelloRequest* request, ::hello::HelloReply* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void SayHello(::grpc::ClientContext* context, const ::hello::HelloRequest* request, ::hello::HelloReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
    };
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    typedef class experimental_async_interface async_interface;
    #endif
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    async_interface* async() { return experimental_async(); }
    #endif
    virtual class experimental_async_interface* experimental_async() { return nullptr; }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::hello::HelloReply>* AsyncSayHelloRaw(::grpc::ClientContext* context, const ::hello::HelloRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::hello::HelloReply>* PrepareAsyncSayHelloRaw(::grpc::ClientContext* context, const ::hello::HelloRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status SayHello(::grpc::ClientContext* context, const ::hello::HelloRequest& request, ::hello::HelloReply* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hello::HelloReply>> AsyncSayHello(::grpc::ClientContext* context, const ::hello::HelloRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hello::HelloReply>>(AsyncSayHelloRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hello::HelloReply>> PrepareAsyncSayHello(::grpc::ClientContext* context, const ::hello::HelloRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::hello::HelloReply>>(PrepareAsyncSayHelloRaw(context, request, cq));
    }
    class experimental_async final :
      public StubInterface::experimental_async_interface {
     public:
      void SayHello(::grpc::ClientContext* context, const ::hello::HelloRequest* request, ::hello::HelloReply* response, std::function<void(::grpc::Status)>) override;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void SayHello(::grpc::ClientContext* context, const ::hello::HelloRequest* request, ::hello::HelloReply* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void SayHello(::grpc::ClientContext* context, const ::hello::HelloRequest* request, ::hello::HelloReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
     private:
      friend class Stub;
      explicit experimental_async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class experimental_async_interface* experimental_async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class experimental_async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::hello::HelloReply>* AsyncSayHelloRaw(::grpc::ClientContext* context, const ::hello::HelloRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::hello::HelloReply>* PrepareAsyncSayHelloRaw(::grpc::ClientContext* context, const ::hello::HelloRequest& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_SayHello_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status SayHello(::grpc::ServerContext* context, const ::hello::HelloRequest* request, ::hello::HelloReply* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_SayHello : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_SayHello() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_SayHello() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SayHello(::grpc::ServerContext* /*context*/, const ::hello::HelloRequest* /*request*/, ::hello::HelloReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestSayHello(::grpc::ServerContext* context, ::hello::HelloRequest* request, ::grpc::ServerAsyncResponseWriter< ::hello::HelloReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_SayHello<Service > AsyncService;
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_SayHello : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_SayHello() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::hello::HelloRequest, ::hello::HelloReply>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::hello::HelloRequest* request, ::hello::HelloReply* response) { return this->SayHello(context, request, response); }));}
    void SetMessageAllocatorFor_SayHello(
        ::grpc::experimental::MessageAllocator< ::hello::HelloRequest, ::hello::HelloReply>* allocator) {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
    #else
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::experimental().GetHandler(0);
    #endif
      static_cast<::grpc::internal::CallbackUnaryHandler< ::hello::HelloRequest, ::hello::HelloReply>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~ExperimentalWithCallbackMethod_SayHello() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SayHello(::grpc::ServerContext* /*context*/, const ::hello::HelloRequest* /*request*/, ::hello::HelloReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* SayHello(
      ::grpc::CallbackServerContext* /*context*/, const ::hello::HelloRequest* /*request*/, ::hello::HelloReply* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* SayHello(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::hello::HelloRequest* /*request*/, ::hello::HelloReply* /*response*/)
    #endif
      { return nullptr; }
  };
  #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
  typedef ExperimentalWithCallbackMethod_SayHello<Service > CallbackService;
  #endif

  typedef ExperimentalWithCallbackMethod_SayHello<Service > ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_SayHello : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_SayHello() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_SayHello() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SayHello(::grpc::ServerContext* /*context*/, const ::hello::HelloRequest* /*request*/, ::hello::HelloReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_SayHello : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_SayHello() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_SayHello() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SayHello(::grpc::ServerContext* /*context*/, const ::hello::HelloRequest* /*request*/, ::hello::HelloReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestSayHello(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_SayHello : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_SayHello() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->SayHello(context, request, response); }));
    }
    ~ExperimentalWithRawCallbackMethod_SayHello() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SayHello(::grpc::ServerContext* /*context*/, const ::hello::HelloRequest* /*request*/, ::hello::HelloReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* SayHello(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* SayHello(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_SayHello : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_SayHello() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::hello::HelloRequest, ::hello::HelloReply>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::hello::HelloRequest, ::hello::HelloReply>* streamer) {
                       return this->StreamedSayHello(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_SayHello() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status SayHello(::grpc::ServerContext* /*context*/, const ::hello::HelloRequest* /*request*/, ::hello::HelloReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedSayHello(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::hello::HelloRequest,::hello::HelloReply>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_SayHello<Service > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_SayHello<Service > StreamedService;
};

}  // namespace hello


#endif  // GRPC_demo_2eproto__INCLUDED
