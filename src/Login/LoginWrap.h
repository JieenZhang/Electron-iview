#ifndef NODE_LOGIN_WRAP_H
#define NODE_LOGIN_WRAP_H

#pragma once
#include <node.h>
#include <nan.h>

class LoginWrap : public Nan::ObjectWrap {

public:
	LoginWrap();
	~LoginWrap();
public:
    void CheckPwd(CHAR* szUserName, CHAR* szPwd, Nan::Callback* callback);

	static void Init(v8::Local<v8::Object> exports);
	static NAN_METHOD(New);
	static NAN_METHOD(Login);

private:
	Nan::Callback* callback;
};

#endif
