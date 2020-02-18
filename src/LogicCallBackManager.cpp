#include "LogicCallBackManager.h"
#include "PacketHandlers.h"
#include "System/Log/AsyncLogger.h"
#include <v8.h>
#include <node.h>
#include "Dirlogic.h"




void OnAsyncClosed(uv_handle_t* handle)
{
	Log(LOG_DEBUG, "OnAsyncClosed");
	uv_async_t* async = (uv_async_t*)handle;
	//AsyncArgs* data = (AsyncArgs*)async->data;
	//if (data->callback)
	//{
	//	data->callback->Reset();
	//}
	
	// SAFE_DELETE(data);
}

void OnEFSWEvent(uv_async_t* handle)
{
	Nan::HandleScope scope;

	Nan::AsyncResource resource("efsw:core_listener.on_efsw_event");

	AsyncArgs* asyncArgs = (AsyncArgs*)handle->data;
	// Object ���͵�����
	//Local<Object> obj = v8::Object::New(Isolate::GetCurrent());
	//obj->Set(Nan::New("DirName").ToLocalChecked(), Nan::New("aaaa").ToLocalChecked());

	Local<Value> argv[1];
	argv[0] = LogicCallBackManager::CreateJsObj(asyncArgs->dataType, asyncArgs->dataStruct);
	
	
	Log(LOG_DEBUG, "CreateJsObj Ok");
	if (asyncArgs->callback)
	{
		Log(LOG_DEBUG, "OnEFSWEvent Call Js");
		asyncArgs->callback->Call(1, argv);
		
	}
	
	uv_close((uv_handle_t*)handle, OnAsyncClosed);
}

/////////////////////////////////////////////////////////////////////////////////////////

LogicCallBackManager::LogicCallBackManager()
{
	InitHandlers();
}

LogicCallBackManager::~LogicCallBackManager()
{
}

BOOL LogicCallBackManager::RegisterCallBack(UINT32 packetID, AsyncArgs* asyncArgs)
{
	std::map<UINT32, AsyncArgs*>::iterator iter;
	iter = m_CallBacks->find(packetID);
	if (iter == m_CallBacks->end())
	{
		m_CallBacks->insert(std::pair<UINT32, AsyncArgs*>(packetID, asyncArgs));
		// ��ʼ���¼�������
		uv_async_init(uv_default_loop(), &asyncArgs->handle, OnEFSWEvent);
		return TRUE;
	}
	return FALSE;
}

v8::Local<v8::Object> LogicCallBackManager::CreateJsObj(StructType type, void * dataStruct)
{
	// Object ���͵�����
	Local<Object> obj = v8::Object::New(Isolate::GetCurrent());

	switch (type)
	{
	case TYPE_MAG_DIRITEM:
		{
			MAG_DIRITEM* dirItem = (MAG_DIRITEM*)dataStruct;
			obj->Set(0, Nan::New(dirItem->iDirNum));
			obj->Set(1, Nan::New(dirItem->szMagDirName).ToLocalChecked());
		}
		break;

	default:
		break;
	}

	obj->Set(Nan::New("Type").ToLocalChecked(), Nan::New(type));
	return obj;
}

void LogicCallBackManager::InitHandlers()
{
	m_CallBacks = new std::map<UINT32, AsyncArgs*>();
	//Ŀ¼
	//�½�Ŀ¼
	PacketHandler* handlerAddMagDir = XNEW(PacketHandler)(WFCMD_ADD_MAGDIR, false);
	UTILS_REGISTER_MESSAGEHANDLER(handlerAddMagDir, this, &LogicCallBackManager::HandleAddMagDir);
	PacketHandlers::Instance()->Register(WFCMD_ADD_MAGDIR, handlerAddMagDir);
	//����Ŀ¼
	PacketHandler* handleUpdateMagDir = XNEW(PacketHandler)(WFCMD_UPDATE_MAGDIR, false);
	UTILS_REGISTER_MESSAGEHANDLER(handleUpdateMagDir, this, &LogicCallBackManager::HandleUpdateMagDir);
	PacketHandlers::Instance()->Register(WFCMD_UPDATE_MAGDIR, handleUpdateMagDir);
	//ɾ��Ŀ¼
	PacketHandler* handleDelMagDir = XNEW(PacketHandler)(WFCMD_DEL_MAGDIR, false);
	UTILS_REGISTER_MESSAGEHANDLER(handleDelMagDir, this, &LogicCallBackManager::HandleDelMagDir);
	PacketHandlers::Instance()->Register(WFCMD_DEL_MAGDIR, handleDelMagDir);
	//**********************************************************************************************
	//��Ӳ���ϵͳ����Ŀ¼
	PacketHandler* handleAddMagDirOs = XNEW(PacketHandler)(WFCMD_ADD_MAGDIROS, false);
	UTILS_REGISTER_MESSAGEHANDLER(handleAddMagDirOs, this, &LogicCallBackManager::HandleAddMagDirOs);
	PacketHandlers::Instance()->Register(WFCMD_ADD_MAGDIROS, handleAddMagDirOs);
	//���²���ϵͳ����Ŀ¼
	PacketHandler* handleUpdateMagDirOS = XNEW(PacketHandler)(WFCMD_UPDATE_MAGDIROS, false);
	UTILS_REGISTER_MESSAGEHANDLER(handleUpdateMagDirOS, this, &LogicCallBackManager::HandleUpdateMagDirOs);
	PacketHandlers::Instance()->Register(WFCMD_UPDATE_MAGDIROS, handleUpdateMagDirOS);
	//ɾ������ϵͳ����Ŀ¼
	PacketHandler* handleDelMagDirOs = XNEW(PacketHandler)(WFCMD_DEL_MAGDIROS, false);
	UTILS_REGISTER_MESSAGEHANDLER(handleDelMagDirOs, this, &LogicCallBackManager::HandleDelMagDirOs);
	PacketHandlers::Instance()->Register(WFCMD_DEL_MAGDIROS, handleDelMagDirOs);
	//***********************************************************************************************
	//��������
	//�½���������
	PacketHandler* handleBuildPartImg = XNEW(PacketHandler)(WFCMD_PIMG_BUILDIMG, false);
	UTILS_REGISTER_MESSAGEHANDLER(handleBuildPartImg, this, &LogicCallBackManager::HandleBuildPartImg);
	PacketHandlers::Instance()->Register(WFCMD_PIMG_BUILDIMG, handleBuildPartImg);
	//���·�������
	PacketHandler* handleUpdatePartImg = XNEW(PacketHandler)(WFCMD_UPDATE_PIMGINFO, false);
	UTILS_REGISTER_MESSAGEHANDLER(handleUpdatePartImg, this, &LogicCallBackManager::HandleUpdatePartImg);
	PacketHandlers::Instance()->Register(WFCMD_UPDATE_PIMGINFO, handleUpdatePartImg);
	//ɾ����������
	PacketHandler* handleDeletePartImg = XNEW(PacketHandler)(WFCMD_PIMG_DELETEPARTIMG, false);
	UTILS_REGISTER_MESSAGEHANDLER(handleDeletePartImg, this, &LogicCallBackManager::HandleDeletePartImg);
	PacketHandlers::Instance()->Register(WFCMD_PIMG_DELETEPARTIMG, handleDeletePartImg);
	//***********************************************************************************************
	//����
	//�½�����
	PacketHandler* handleBuildDiskImg = XNEW(PacketHandler)(WFCMD_DIMG_BUILDDISKIMG, false);
	UTILS_REGISTER_MESSAGEHANDLER(handleBuildDiskImg, this, &LogicCallBackManager::HandleBuildDiskImg);
	PacketHandlers::Instance()->Register(WFCMD_DIMG_BUILDDISKIMG, handleBuildDiskImg);
	//���´���
	PacketHandler* handleUpdateDiskImg = XNEW(PacketHandler)(WFCMD_UPDATE_DISKIMGINFO, false);
	UTILS_REGISTER_MESSAGEHANDLER(handleUpdateDiskImg, this, &LogicCallBackManager::HandleUpdateDiskImg);
	PacketHandlers::Instance()->Register(WFCMD_UPDATE_DISKIMGINFO, handleUpdateDiskImg);
	//ɾ������
	PacketHandler* handleDeleteDiskImg = XNEW(PacketHandler)(WFCMD_DIMG_DELETEDISKIMG, false);
	UTILS_REGISTER_MESSAGEHANDLER(handleDeleteDiskImg, this, &LogicCallBackManager::HandleDeleteDiskImg);
	PacketHandlers::Instance()->Register(WFCMD_DIMG_DELETEDISKIMG, handleDeleteDiskImg);
	//***********************************************************************************************
	//����
	PacketHandler* handlerAddGroup = XNEW(PacketHandler)(WFCMD_CFG_BUILDGROUP, false);
	UTILS_REGISTER_MESSAGEHANDLER(handlerAddGroup, this, &LogicCallBackManager::HandleBuildGroup);
	PacketHandlers::Instance()->Register(WFCMD_CFG_BUILDGROUP, handlerAddGroup);
	//���·���
	PacketHandler* handleUpdateGroupInfo = XNEW(PacketHandler)(WFCMD_CFG_UPDATEGROUPINFO, false);
	UTILS_REGISTER_MESSAGEHANDLER(handleUpdateGroupInfo, this, &LogicCallBackManager::HandleUpdateGroupInfo);
	PacketHandlers::Instance()->Register(WFCMD_CFG_UPDATEGROUPINFO, handleUpdateGroupInfo);
	//ɾ������
	PacketHandler* handleDeleteGroup = XNEW(PacketHandler)(WFCMD_CFG_DELETEGROUP, false);
	UTILS_REGISTER_MESSAGEHANDLER(handleDeleteGroup, this, &LogicCallBackManager::HandleDeleteGroup);
	PacketHandlers::Instance()->Register(WFCMD_CFG_DELETEGROUP, handleDeleteGroup);
	//����ն�
	PacketHandler* handleAddMachine = XNEW(PacketHandler)(WFCMD_NEWMACHINE_ADDTOSYSTEM, false);
	UTILS_REGISTER_MESSAGEHANDLER(handleAddMachine, this, &LogicCallBackManager::HandleAddMachine);
	PacketHandlers::Instance()->Register(WFCMD_NEWMACHINE_ADDTOSYSTEM, handleAddMachine);
	//�����ն�
	PacketHandler* handleUpdateMachineInfo = XNEW(PacketHandler)(WFCMD_CFG_UPDATEMACHINEINFO, false);
	UTILS_REGISTER_MESSAGEHANDLER(handleUpdateMachineInfo, this, &LogicCallBackManager::HandleUpdateMachineInfo);
	PacketHandlers::Instance()->Register(WFCMD_CFG_UPDATEMACHINEINFO, handleUpdateMachineInfo);
	//ɾ���ն�
	PacketHandler* handleDeleteMachine = XNEW(PacketHandler)(WFCMD_CFG_DELETEMACHINE, false);
	UTILS_REGISTER_MESSAGEHANDLER(handleDeleteMachine, this, &LogicCallBackManager::HandleDeleteMachine);
	PacketHandlers::Instance()->Register(WFCMD_CFG_DELETEMACHINE, handleDeleteMachine);
	//*************************************************************************************************
	//����ϵͳ����
	//����ϵͳ����
	PacketHandler* handleBuildOs = XNEW(PacketHandler)(WFCMD_DIMG_BUILDOS, false);
	UTILS_REGISTER_MESSAGEHANDLER(handleBuildOs, this, &LogicCallBackManager::HandleBuildOs);
	PacketHandlers::Instance()->Register(WFCMD_DIMG_BUILDOS, handleBuildOs);
	//����ϵͳ����
	PacketHandler* handleUpdateOsImage = XNEW(PacketHandler)(WFCMD_CFG_UPDATEOSIMG, false);
	UTILS_REGISTER_MESSAGEHANDLER(handleUpdateOsImage, this, &LogicCallBackManager::HandleUpdateOsImg);
	PacketHandlers::Instance()->Register(WFCMD_CFG_UPDATEOSIMG, handleUpdateOsImage);
	//ɾ��ϵͳ����
	PacketHandler* handleDeleteOsImage = XNEW(PacketHandler)(WFCMD_DELETE_OSIMG, false);
	UTILS_REGISTER_MESSAGEHANDLER(handleDeleteOsImage, this, &LogicCallBackManager::HandleDeleteOsImg);
	PacketHandlers::Instance()->Register(WFCMD_DELETE_OSIMG, handleDeleteOsImage);
}

INT LogicCallBackManager::HandleAddMagDir(REQUEST_EX * pRequestEx)
{
	REQUEST* pRequest = (REQUEST*)&pRequestEx->request;
	MAG_DIRITEM* pItem = (MAG_DIRITEM*)((CHAR*)pRequest + sizeof(REQUEST));
	MAG_DIRITEM Temp = *pItem;
	
	Log(LOG_DEBUG, "HandleAddMagDir usCommand:%02x", pRequestEx->request.usCommand);

	std::map<UINT32, AsyncArgs*>::iterator iter;
	iter = m_CallBacks->find(pRequestEx->request.usCommand);
	if (iter != m_CallBacks->end())
	{
		/*Local<Value> argv[1];
		argv[0] = Nan::New(Temp.szMagDirName).ToLocalChecked();
		iter->second->Call(1, argv);*/
		//args->data->
		Log(LOG_DEBUG, "HandleAddMagDir uv_async_init");
		// ���ûص�����
		AsyncArgs* asyncArgs = iter->second;
		if (asyncArgs)
		{
			//
			Log(LOG_DEBUG, "HandleAddMagDir Set");
			// �������ݶ���
			asyncArgs->dataStruct = CDirLogic::Instance()->GetDirItem(Temp.iDirNum);
			// ������������
			asyncArgs->dataType = TYPE_MAG_DIRITEM;
			
			// �¼���������
			asyncArgs->handle.data = asyncArgs;
			// �����¼�
			uv_async_send(&asyncArgs->handle);
		}
		
	}
	return 0;
}

INT LogicCallBackManager::HandleUpdateMagDir(REQUEST_EX * pRequestEx)
{
	return 0;
}

INT LogicCallBackManager::HandleDelMagDir(REQUEST_EX * pRequestEx)
{
	return 0;
}

INT LogicCallBackManager::HandleAddMagDirOs(REQUEST_EX * pRequestEx)
{
	return 0;
}

INT LogicCallBackManager::HandleUpdateMagDirOs(REQUEST_EX * pRequestEx)
{
	return 0;
}

INT LogicCallBackManager::HandleDelMagDirOs(REQUEST_EX * pRequestEx)
{
	return 0;
}

INT LogicCallBackManager::HandleBuildPartImg(REQUEST_EX * pRequestEx)
{
	return 0;
}

INT LogicCallBackManager::HandleUpdatePartImg(REQUEST_EX * pRequestEx)
{
	return 0;
}

INT LogicCallBackManager::HandleDeletePartImg(REQUEST_EX * pRequestEx)
{
	return 0;
}

INT LogicCallBackManager::HandleBuildDiskImg(REQUEST_EX * pRequestEx)
{
	return 0;
}

INT LogicCallBackManager::HandleUpdateDiskImg(REQUEST_EX * pRequestEx)
{
	return 0;
}

INT LogicCallBackManager::HandleDeleteDiskImg(REQUEST_EX * pRequestEx)
{
	return 0;
}

INT LogicCallBackManager::HandleBuildGroup(REQUEST_EX * pRequestEx)
{
	return 0;
}

INT LogicCallBackManager::HandleUpdateGroupInfo(REQUEST_EX * pRequestEx)
{
	return 0;
}

INT LogicCallBackManager::HandleDeleteGroup(REQUEST_EX * pRequestEx)
{
	return 0;
}

INT LogicCallBackManager::HandleBuildOs(REQUEST_EX * pRequestEx)
{
	return 0;
}

INT LogicCallBackManager::HandleUpdateOsImg(REQUEST_EX * pRequestEx)
{
	return 0;
}

INT LogicCallBackManager::HandleDeleteOsImg(REQUEST_EX * pRequestEx)
{
	return 0;
}

INT LogicCallBackManager::HandleAddMachine(REQUEST_EX * pRequestEx)
{
	return 0;
}

INT LogicCallBackManager::HandleUpdateMachineInfo(REQUEST_EX * pRequestEx)
{
	return 0;
}

INT LogicCallBackManager::HandleDeleteMachine(REQUEST_EX * pRequestEx)
{
	return 0;
}
