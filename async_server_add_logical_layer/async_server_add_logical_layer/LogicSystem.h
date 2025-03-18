#pragma once
#include "Singleton.h"
#include <queue>
#include <thread>
#include "CSession.h"
#include <map>
#include <functional>
#include "const.h"
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>

typedef function<void(shared_ptr<CSession>, const short& msg_id, const string& msg_data)> FunCallBack;

class LogicSystem :public Singleton<LogicSystem> {
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem();
	//session层收到消息后，调用这个函数投递到消息队列
	void PostMsgToQue(shared_ptr<LogicNode> msg);
private:
	LogicSystem();
	//注册回调函数
	void RegisterCallBacks();
	//回调函数
	void HelloWordCallBack(shared_ptr<CSession> session, const short& msg_id, const string& mgs_data);
	//处理函数
	void DealMsg();

	std::queue<shared_ptr<LogicNode>> _msg_que;
	std::mutex _mutex;
	std::condition_variable _consume;
	std::thread _worker_thread;
	bool _b_stop;
	//存储id和id要调用的回调函数
	std::map<short, FunCallBack> _fun_callback;
};

