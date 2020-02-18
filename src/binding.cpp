#include <nan.h>
#include "LogicManager.h"
#include "UtilsFunction.h"
#include "Random\KRandom.h"
#include "Dirlogic.h"
#include "System/Log/AsyncLogger.h"
#include "DiskLogic.h"
#include "LoginLogic.h"

AsyncDailyFileLogger g_logger("c:\\Log\\binding.log");



using namespace System::Random;


NAN_METHOD(Login) {
    
	CHAR	szPath[512];
	CHAR	szPubKey[512];
	if (!GetModulePathEx(NULL, szPath))
	{
		info.GetReturnValue().Set(Nan::New("GetModulePathEx FAILED").ToLocalChecked());
		return;
	}
	strcpy(szPubKey, szPath);
	strcat(szPubKey, "\\wfbspubkey.bin");

	KRandom_2*	random = new KRandom_2();
	random->Initialize(30, 50);
	DWORD randomdw = random->GetRandom();
	CHAR msgp[512];
	memset(msgp, 0, 512);

	Log(LOG_DEBUG, "Initialize %s", szPubKey);
	
	// ¼ÓÃÜÃÜÂë
	//CHAR*	textEncrypt = new CHAR[4096];
	//CHAR	textPwd[512];
	//memset(textEncrypt, 0, 4096);
	//memset(textPwd, 0, 512);
	//sprintf(textPwd, "%s", "123456");
	//int ii;
	//if (strlen(textPwd))
	//{
	//	Log(LOG_DEBUG, "start pubkey:%s,textPwd:%s", szPubKey, textPwd);
	//	WFRSAEncryptString(szPubKey, textPwd, textEncrypt);//¼ÓÃÜ
	//	Log(LOG_DEBUG, "end pubkey:%s,encryptpwd:%s", szPubKey, textEncrypt);
	//	info.GetReturnValue().Set(Nan::New(textPwd).ToLocalChecked());
	//}

	//BOOL result = CDirLogic::Instance()->Initialize();
	//if (result)
	//{
	//	sprintf(msgp, "true hello_%d_%s", randomdw, szPubKey);
	//	info.GetReturnValue().Set(Nan::New(msgp).ToLocalChecked());
	//}
	//else
	//{
	//	sprintf(msgp, "false hello_%d_%s", randomdw, szPubKey);
	//	info.GetReturnValue().Set(Nan::New(msgp).ToLocalChecked());
	//}

	

	CLogicManager::Instance()->Initialize((CHAR*)"192.168.10.5", 5526, (CHAR*)"0.0.0.0", 5520);


	NTSTATUS status = CLoginLogic::Instance()->CheckPwd("admin", "123456789a");
	if (NT_SUCCESS(status))
	{
		sprintf(msgp, "true login");
		CDiskLogic::Instance()->Initialize();
		info.GetReturnValue().Set(Nan::New(true));
	}
	else
	{
		sprintf(msgp, "false login");
		info.GetReturnValue().Set(Nan::New(false));
	}
}

NAN_METHOD(NewDisk) {
	DISKIMG diskImg;
	memset(&diskImg, 0, sizeof(DISKIMG));
	sprintf(diskImg.szDiskImgName, "nodejs_disk");
	diskImg.ulDiskImgSize = 2000;
	diskImg.usMagDirIdx = BOOT_DISKIMGS_DIR;
	NTSTATUS status = CDiskLogic::Instance()->BuildDiskImg(&diskImg);
	if (NT_SUCCESS(status))
	{
		info.GetReturnValue().Set(Nan::New(true));
	}
	else
	{
		info.GetReturnValue().Set(Nan::New(false));
	}
}


extern "C" {
    static void start(v8::Handle<v8::Object> target) {
		g_logger.SetLogLevel(0xffffffff);
		GetGlobalLogger().SetLogger(&g_logger);

        Nan::SetMethod(target, "Login", Login);

		Nan::SetMethod(target, "NewDisk", NewDisk);
    }
}

NODE_MODULE(node_addon_example, start)

