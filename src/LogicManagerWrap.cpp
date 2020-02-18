#include <node.h>
#include "System/Log/AsyncLogger.h"
#include "LogicManager.h"
#include "PacketHandlers.h"
#include "Login/LoginWrap.h"
#include "Disk/DiskWrap.h"
#include "LogicCallBackManager.h"
#include <iostream>

using namespace v8;

AsyncDailyFileLogger g_logger("c:\\Log\\binding.log");


NAN_METHOD(Initialize) {
    Log(LOG_DEBUG, "Initialize ParamLen:%d", info.Length());
	if (info.Length() != 3) {
		return Nan::ThrowError("usage: Initialize(serverip, serverport, errcallback)");
	}
	String::Utf8Value address(info[0]);
    int svrPort = info[1]->Int32Value(Nan::GetCurrentContext()).ToChecked();
	Nan::Callback* callback = new Nan::Callback(info[2].As<Function>());
	Handle<Value> argv[2];

	BOOL result = CLogicManager::Instance()->Initialize((CHAR*)*address, 5526, (CHAR*)"0.0.0.0", 5520);
	if (!result)
	{
		Log(LOG_ERROR, "Initialize Param0:%s Param1:%d", *address, svrPort);
		// return Nan::ThrowError("CLogicManager Initialize Failed.");
		

		argv[0] = Nan::New(false);				
		argv[1] = String::NewFromUtf8(info.GetIsolate(), "CLogicManager Initialize Failed.");	// data
		callback->Call(2, argv);
		SAFE_DELETE(callback);
	}
	else
	{
		argv[0] = Nan::New(true);					
		argv[1] = String::NewFromUtf8(info.GetIsolate(), "CLogicManager Initialize Success.");	// data
		callback->Call(2, argv);
		SAFE_DELETE(callback);
	}
}

NAN_METHOD(RegistHandlers) {
	Log(LOG_DEBUG, "Js Call RegistHandlers");
	
	if (info.Length() != 2) {
		return Nan::ThrowError("usage: RegistHandlers(pktid, callback)");
	}
	Isolate* isolate = info.GetIsolate();
	UINT32 pktID = info[0]->Int32Value(Nan::GetCurrentContext()).ToChecked();
	Nan::Callback* callback = new Nan::Callback(info[1].As<Function>());

	
	Log(LOG_DEBUG, "LogicCallBackManager RegisterCallBack");
	AsyncArgs* asyncArgs = new AsyncArgs();
	asyncArgs->callback = callback;
	BOOL registResult = LogicCallBackManager::GetInstancePtr()->RegisterCallBack(pktID, asyncArgs);
	if (registResult)
	{
		info.GetReturnValue().Set(true);
		if (pktID == WFCMD_ADD_MAGDIR)
		{
			CDirLogic::Instance()->InitAllDirItem();
		}
	}
	else
	{
		info.GetReturnValue().Set(false);
	}
}

void InitAll(Local<Object> exports) {
    // 日志初始化
    g_logger.SetLogLevel(0xffffffff);
    GetGlobalLogger().SetLogger(&g_logger);
    // 服务器初始化
    Nan::SetMethod(exports, "Initialize", Initialize);
	Nan::SetMethod(exports, "RegistHandlers", RegistHandlers);
    // 对象初始化
    LoginWrap::Init(exports);
	DiskWrap::Init(exports);
}

NODE_MODULE(WFBSManagerNodeJS, InitAll)
