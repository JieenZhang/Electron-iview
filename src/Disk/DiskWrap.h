#ifndef NODE_DISK_WRAP_H
#define NODE_DISK_WRAP_H

#pragma once
#include <node.h>
#include <nan.h>

class DiskWrap : public Nan::ObjectWrap {

public:
	DiskWrap();
	~DiskWrap();

	static void Init(v8::Local<v8::Object> exports);
	
	static NAN_METHOD(New);
	static NAN_METHOD(NewDisk);
};

#endif