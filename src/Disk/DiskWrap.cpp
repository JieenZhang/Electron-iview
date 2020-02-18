#include <v8.h>
#include <node.h>
#include "LogicManager.h"
#include "UtilsFunction.h"
#include "Random\KRandom.h"
#include "Dirlogic.h"
#include "System/Log/AsyncLogger.h"
#include "DiskLogic.h"
#include "LoginLogic.h"
#include "DiskWrap.h"

using namespace node;
using namespace v8;

DiskWrap::DiskWrap()
{
	
}

DiskWrap::~DiskWrap()
{
}

void DiskWrap::Init(Local<Object> target) {
	Nan::HandleScope scope;

	Local<FunctionTemplate> t = Nan::New<FunctionTemplate>(New);

	Isolate *isolate = target->GetIsolate();
	Local<Context> ctx = isolate->GetCurrentContext();

	t->InstanceTemplate()->SetInternalFieldCount(1);
	t->SetClassName(Nan::New("DiskWrap").ToLocalChecked());

	Nan::SetPrototypeMethod(t, "NewDisk", NewDisk);
	target->Set(ctx, Nan::New("DiskWrap").ToLocalChecked(), t->GetFunction(ctx).ToLocalChecked());
}

NAN_METHOD(DiskWrap::New) {
	CDiskLogic::Instance()->Initialize();
}

NAN_METHOD(DiskWrap::NewDisk) {

}
