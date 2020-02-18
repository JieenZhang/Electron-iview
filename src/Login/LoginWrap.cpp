#include <v8.h>
#include <node.h>
#include "LogicManager.h"
#include "UtilsFunction.h"
#include "Random\KRandom.h"
#include "Dirlogic.h"
#include "System/Log/AsyncLogger.h"
#include "DiskLogic.h"
#include "LoginLogic.h"
#include "LoginWrap.h"
#include "LogicCallBackManager.h"
#include "Dirlogic.h"

using namespace node;
using namespace v8;
using namespace System::Random;



LoginWrap::LoginWrap()
{
	//CLoginLogic::GetInstancePtr()->Initialize();
}

LoginWrap::~LoginWrap()
{
}

void LoginWrap::CheckPwd(CHAR * szUserName, CHAR * szPwd, Nan::Callback* callback)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	CHAR	szPath[512];
	CHAR	szPubKey[512];
	if (!GetModulePathEx(NULL, szPath))
	{
		Log(LOG_DEBUG, "Initialize %s", szPubKey);
		return;
	}
	strcpy(szPubKey, szPath);
	strcat(szPubKey, "\\wfbspubkey.bin");
	Log(LOG_DEBUG, "CheckPwd: %s", szPubKey);

	status = CLoginLogic::GetInstancePtr()->CheckPwd(szUserName, szPwd);
	Local<Value> argv[1];
	//Local<Object> callBackObj = Nan::New<Object>(New);
	if (NT_SUCCESS(status))
	{
		//callBackObj->Set(Nan::New("result").ToLocalChecked(), Nan::New(true));
		argv[0] = Nan::New(true);

		LogicCallBackManager::Instance();
		CDirLogic::Instance()->Initialize();
		
	}
	else
	{
		argv[0] = Nan::New(false);
	}
	callback->Call(1, argv);
	SAFE_DELETE(callback);
}

void LoginWrap::Init(Local<Object> target) {
	Nan::HandleScope scope;

	Local<FunctionTemplate> t = Nan::New<FunctionTemplate>(New);

	t->SetClassName(Nan::New("LoginWrap").ToLocalChecked());
	t->InstanceTemplate()->SetInternalFieldCount(1);

	Nan::SetPrototypeMethod(t, "Login", Login);

	Isolate *isolate = target->GetIsolate();
    Local<Context> ctx = isolate->GetCurrentContext();
	target->Set(ctx, Nan::New("LoginWrap").ToLocalChecked(), t->GetFunction(ctx).ToLocalChecked());
}

NAN_METHOD(LoginWrap::New) {

    LoginWrap *loginComm = new LoginWrap();
    loginComm->Wrap(info.This());

    loginComm->Ref();

	info.GetReturnValue().Set(info.This());
}

NAN_METHOD(LoginWrap::Login) {
	Log(LOG_DEBUG, "Initialize ParamLen:%d", info.Length());
	if (info.Length() != 3) {
		return Nan::ThrowError("usage: Login(szUserName, szPwd, callback)");
	}
	String::Utf8Value username(info[0]);
	String::Utf8Value password(info[1]);
	LoginWrap* loginWrap = Nan::ObjectWrap::Unwrap<LoginWrap>(info.This());
	if (loginWrap)
	{
		loginWrap->CheckPwd(*username, *password, new Nan::Callback(info[2].As<Function>()));
	}
	else
	{
		return Nan::ThrowError("LoginWrap Not Found.");
	}
}
