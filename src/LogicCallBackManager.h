#ifndef __LogicCallBackManager_H
#define __LogicCallBackManager_H
#pragma once

#include "System/Patterns/KSingleton.h"
#include "SessionBase.h"
#include <v8.h>
#include <node.h>
#include <nan.h>
#include <map>
#include <string>

using namespace node;
using namespace v8;

enum StructType{
	TYPE_MAG_DIRITEM
};

// 通用的异步回调结构
struct AsyncArgs {
	Nan::Callback*			callback;
	StructType				dataType;
	void*					dataStruct;
	uv_async_t				handle;
};

class LogicCallBackManager
	: public System::Patterns::KSingleton<LogicCallBackManager>
{
public:
	LogicCallBackManager();
	~LogicCallBackManager();

	BOOL RegisterCallBack(UINT32 packetID, AsyncArgs* asyncArgs);

	static v8::Local<v8::Object> CreateJsObj(StructType type, void* dataStruct);

private:
	void				InitHandlers();

	INT					HandleAddMagDir(REQUEST_EX * pRequestEx);
	INT					HandleUpdateMagDir(REQUEST_EX * pRequestEx);
	INT					HandleDelMagDir(REQUEST_EX * pRequestEx);
	INT					HandleAddMagDirOs(REQUEST_EX* pRequestEx);
	INT					HandleUpdateMagDirOs(REQUEST_EX* pRequestEx);
	INT					HandleDelMagDirOs(REQUEST_EX* pRequestEx);
	INT					HandleBuildPartImg(REQUEST_EX * pRequestEx);
	INT					HandleUpdatePartImg(REQUEST_EX * pRequestEx);
	INT					HandleDeletePartImg(REQUEST_EX * pRequestEx);
	INT					HandleBuildDiskImg(REQUEST_EX * pRequestEx);
	INT					HandleUpdateDiskImg(REQUEST_EX * pRequestEx);
	INT					HandleDeleteDiskImg(REQUEST_EX * pRequestEx);
	INT					HandleBuildGroup(REQUEST_EX* pRequestEx);
	INT					HandleUpdateGroupInfo(REQUEST_EX * pRequestEx);
	INT					HandleDeleteGroup(REQUEST_EX * pRequestEx);
	INT					HandleBuildOs(REQUEST_EX* pRequestEx);
	INT					HandleUpdateOsImg(REQUEST_EX * pRequestEx);
	INT					HandleDeleteOsImg(REQUEST_EX * pRequestEx);
	INT					HandleAddMachine(REQUEST_EX* pRequestEx);
	INT					HandleUpdateMachineInfo(REQUEST_EX * pRequestEx);
	INT					HandleDeleteMachine(REQUEST_EX * pRequestEx);

private:
	std::map<UINT32, AsyncArgs*>* m_CallBacks;
};




#endif