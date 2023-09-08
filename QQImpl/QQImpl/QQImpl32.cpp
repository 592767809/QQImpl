#ifndef _WIN64

#include "QQImpl32.h"
#include <format>
#include <filesystem>
#include <iostream>
#include <format>

qqimpl::QQIpcParentWrapper::QQIpcParentWrapper()
{
	m_child_pid = 0;
	m_ipc_dll = 0;
	m_ptr_IMojoIpc = NULL;
	_last_err = L"ERR_OK";
}

qqimpl::QQIpcParentWrapper::~QQIpcParentWrapper()
{
}

qqimpl::QQIpcChildWrapper::QQIpcChildWrapper()
{
	m_ipc_dll = 0;
	m_ptr_IMojoIpc = NULL;
	_last_err = L"ERR_OK";
}

qqimpl::QQIpcChildWrapper::~QQIpcChildWrapper()
{
}

void __stdcall qqimpl::QQIpcParentWrapper::OnDefaultReceiveMsg(DWORD* pArg, char* msg, DWORD arg3, DWORD arg4, char* addition_msg, int addition_msg_size)
{
	std::cout << std::endl << "OnReceiveParentMsg: [ " << msg << " ]\n";

	char addition_msg_zero[MAX_PATH] = { 0 };//����Ӧ�ö�̬����, ��Ϊaddition_msg��һ����·��, ������protobuf��С�ᳬ��MAX_PATH
	if (addition_msg_size != 0)
	{
		memmove(addition_msg_zero, addition_msg, addition_msg_size);
		std::cout << "The Addition Msg: " << addition_msg_zero << std::endl;
	}

	std::string msg_str = msg;
	if (msg_str == "onScreenShotRequestOCR")
	{
#ifdef NEED_WECHATOCR
		if (!qqimpl::qqocrwrapper::DoOCRTask(addition_msg_zero))
			std::wcout << std::format(L"[!] DoOCRTask Err:{}\n", qqimpl::qqocrwrapper::GetLastErrWStr());
#endif // NEED_WECHATOCR
	}
}

/*
HRESULT DllGetClassObject(
	[in]  REFCLSID rclsid,
	[in]  REFIID   riid,
	[out] LPVOID* ppv);
*/
typedef HRESULT(*LPFN_DLLGETCLASSOBJECT)(REFCLSID, REFIID, LPVOID*);

bool qqimpl::QQIpcParentWrapper::Init(std::wstring exe_path, std::wstring dll_path)
{
	m_ipc_dll = LoadLibraryW(dll_path.c_str());
	if (m_ipc_dll == NULL)
	{
		_last_err = std::format(L"ParentIpc LoadDll {} Err:[{}]", dll_path, GetLastError());
		return false;
	}

	if (!SetExePath(exe_path))
		return false;

	LPFN_DLLGETCLASSOBJECT lpfnDllGetClassObject = (LPFN_DLLGETCLASSOBJECT)GetProcAddress(m_ipc_dll, "DllGetClassObject");
	if (!lpfnDllGetClassObject)
	{
		_last_err = std::format(L"Can't Get \'DllGetClassObject\' Addr:[{}]", GetLastError());
		return false;
	}

	IID clsid = { 0x14D5685E, 0x0574, 0x4F1F,{ 0xB6, 0xA4, 0x9B, 0xE9, 0xC3, 0xBE, 0x0C, 0x0D } };
	//IID m_iid = { 0x00000001, 0x0000, 0x00000,{ 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };//IID_IClassFactory
	IID riid = { 0x8504298A, 0xA491, 0x4B40,{ 0xAA, 0xC4, 0xB2, 0x60, 0x8D, 0xAC, 0x40, 0x45} };

	IClassFactory* pIClassFactory = NULL;
	if (lpfnDllGetClassObject(clsid, IID_IClassFactory, (LPVOID*)(&pIClassFactory)) != S_OK)
	{
		_last_err = std::format(L"lpfnDllGetClassObject Err:[{}]", GetLastError());
		return false;
	}

	DWORD* pIMojoIpc = NULL;
	pIClassFactory->CreateInstance(NULL, riid, (void**)&pIMojoIpc);
	pIClassFactory->Release();
	if (pIMojoIpc == NULL)
	{
		_last_err = L"pIClassFactory->CreateInstance Err";
		return false;
	}

	this->m_ptr_IMojoIpc = pIMojoIpc;

	_last_err = L"ERR_OK";
	return true;
}

bool qqimpl::QQIpcChildWrapper::Init(std::wstring dll_path)
{
	m_ipc_dll = LoadLibraryW(dll_path.c_str());
	if (m_ipc_dll == NULL)
	{
		_last_err = std::format(L"ChildIpc LoadDll {} Err:[{}]", dll_path, GetLastError());
		return false;
	}

	LPFN_DLLGETCLASSOBJECT lpfnDllGetClassObject = (LPFN_DLLGETCLASSOBJECT)GetProcAddress(m_ipc_dll, "DllGetClassObject");
	if (!lpfnDllGetClassObject)
	{
		_last_err = std::format(L"Can't Get \'DllGetClassObject\' Addr:[{}]", GetLastError());
		return false;
	}

	IID clsid = { 0x14D5685E, 0x0574, 0x4F1F,{ 0xB6, 0xA4, 0x9B, 0xE9, 0xC3, 0xBE, 0x0C, 0x0D } };
	//IID m_iid = { 0x00000001, 0x0000, 0x00000,{ 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };//IID_IClassFactory
	IID riid = { 0x2A106583, 0xA3C4, 0x43AF,{ 0x85, 0xA0, 0x57, 0x81, 0x74, 0x89, 0x36, 0x1A} };

	IClassFactory* pIClassFactory = NULL;
	if (lpfnDllGetClassObject(clsid, IID_IClassFactory, (LPVOID*)(&pIClassFactory)) != S_OK)
	{
		_last_err = std::format(L"lpfnDllGetClassObject Err:[{}]", GetLastError());
		return false;
	}

	DWORD* pIMojoIpc = NULL;
	pIClassFactory->CreateInstance(NULL, riid, (void**)&pIMojoIpc);
	pIClassFactory->Release();
	if (pIMojoIpc == NULL)
	{
		_last_err = L"pIClassFactory->CreateInstance Err";
		return false;
	}

	this->m_ptr_IMojoIpc = pIMojoIpc;

	_last_err = L"ERR_OK";
	return true;
}

bool qqimpl::QQIpcParentWrapper::SetExePath(std::wstring exe_path)
{
	if (!std::filesystem::exists(exe_path))
	{
		_last_err = L"The EXE to Launched Path is not exist!";
		return false;
	}

	if (!std::filesystem::is_regular_file(exe_path))
	{
		_last_err = L"The Path is not a file!";
		return false;
	}

	this->m_exe_path = exe_path;
	_last_err = L"ERR_OK";
	return true;
}

qqimpl::QQLAUNCHINFO* qqimpl::QQIpcParentWrapper::GetInfoPtr()
{
	DWORD* pIMojoIpc = this->m_ptr_IMojoIpc;
	if (pIMojoIpc == NULL)
	{
		_last_err = L"IMojoIpc is not init!";
		return nullptr;
	}

	return (qqimpl::QQLAUNCHINFO*)(*(DWORD*)(((DWORD*)(*(DWORD*)(*(pIMojoIpc + 29)))) + 3));
}

std::wstring qqimpl::QQIpcParentWrapper::GetLastErrStr()
{
	return _last_err;
}

std::wstring qqimpl::QQIpcChildWrapper::GetLastErrStr()
{
	return this->_last_err;
}

//����ֱ�Ӳ����ڴ�ĺ��������Ĵ���Ƭ�εı����Ż��ص�
#pragma optimize( "", off )
void qqimpl::QQIpcParentWrapper::SetLogLevel(int level)//������Բ����� Ҳ������� ����
{
	if (m_ipc_dll == NULL) return;
	if (level < 0 || level > 5) level = 5;

	DWORD* lpLogLevel_ = (DWORD*)((DWORD)m_ipc_dll + 0x150034);//����
	*lpLogLevel_ = level;
}

void qqimpl::QQIpcParentWrapper::InitLog(int level, void* callback)
{
	DWORD *pIMojoIpc = this->m_ptr_IMojoIpc, 
		dwFunc = *((DWORD*)(*pIMojoIpc) + 4),	//call [eax + 0x10] �ڶ�������Ӧ����LogCallBack wrapper.node + 0x96C7E0 
		logLevel = level;
	_asm
	{
		push logLevel
		push callback
		push pIMojoIpc
		call dwFunc
	}
	_last_err = L"ERR_OK";
}

void qqimpl::QQIpcParentWrapper::InitParentIpc()
{
	DWORD* pIMojoIpc = this->m_ptr_IMojoIpc,
		dwFunc = *((DWORD*)(*pIMojoIpc) + 3);//pIMojoIpc->InitParentIpc()
	_asm
	{
		push pIMojoIpc
		call dwFunc
	}
	_last_err = L"ERR_OK";
}

UINT qqimpl::QQIpcParentWrapper::LaunchChildProcess(void* callback)
{
	//pIMojoIpc->LaunchChildProcess(wchar_t *path, DWORD 0,DWORD 0, LPVOID callback, LPVOID Launch�����Ĳ���);//arg4: wrapper.node + 0x97D1B0
	DWORD* pIMojoIpc = this->m_ptr_IMojoIpc,
		dwFunc = *((DWORD*)(*pIMojoIpc) + 5);
	struct QQLAUNCHINFO info; memset(&info, 0, sizeof(struct QQLAUNCHINFO));
	info.ExePath = this->m_exe_path;
	DWORD* lpArg = (DWORD*)&info, lpCallback = (DWORD)callback, dwPid = 0;
	const wchar_t* wzChildPath = m_exe_path.c_str();
	if (callback == NULL)
		lpCallback = (DWORD)OnDefaultReceiveMsg;

	_asm
	{
		push lpArg
		push lpCallback
		push 0
		push 0
		push wzChildPath
		push pIMojoIpc
		call dwFunc
		mov dwPid, eax
	}//�ڴ˺����ڲ��´��һ��QQSSINFO��pIMojoIpc���� �ɹ�����PID ʧ�ܷ���0
	if (dwPid <= 0)
	{
		_last_err = L"pIMojoIpc->LaunchChildProcess Internal Error";
		return 0;
	}
	this->m_child_pid = dwPid;

	_last_err = L"ERR_OK";
	return dwPid;
}

bool qqimpl::QQIpcParentWrapper::ConnectedToChildProcess()
{
	if (this->m_child_pid <= 0)
	{
		_last_err = L"ChildPid is not valid!";
		return false;
	}

	//pIMojoIpc->ConnectedToChildProcess(DWORD child_pid);
	DWORD* pIMojoIpc = this->m_ptr_IMojoIpc,
		dwFunc = *((DWORD*)(*pIMojoIpc) + 6), dwPid = this->m_child_pid, call_result = 0;
	_asm
	{
		push dwPid
		push pIMojoIpc
		call dwFunc
		mov call_result, eax
	}
	if (call_result == E_FAIL)
	{
		_last_err = L"pIMojoIpc->ConnectedToChildProcess Internal Error";
		return false;
	}
	_last_err = L"ERR_OK";
	return true;
}

bool qqimpl::QQIpcParentWrapper::SendIpcMessage(std::string command, std::string addition_msg)
{
	//pIMojoIpc->SendMessageUsingBufferInIPCThread(DWORD child_pid, char* command, DWORD 0, DWORD 0, char* addition_msg, int addition_msg_size);
	DWORD *pIMojoIpc = this->m_ptr_IMojoIpc,
		dwFunc = *((DWORD*)(*pIMojoIpc) + 7), addition_len = 0, dwPid = this->m_child_pid, call_result = 0;
	const char* command_c = command.c_str(), *addition_c = NULL;
	if (!addition_msg.empty())
	{
		addition_c = addition_msg.c_str();
		addition_len = addition_msg.length();
	}

	_asm
	{
		push addition_len
		push addition_c
		push 0
		push 0
		push command_c
		push dwPid
		push pIMojoIpc
		call dwFunc
		mov call_result, eax
	}
	
	//ʧ�ܻ᷵��0x80004005 E_FAIL
	if (call_result == E_FAIL)
	{
		_last_err = L"pIMojoIpc->SendMessageUsingBufferInIPCThread Internal Error";
		return false;
	}
	_last_err = L"ERR_OK";
	return true;
}

bool qqimpl::QQIpcParentWrapper::TerminateChildProcess(int exit_code, bool wait_)
{
	//pIMojoIpc->TerminateChildProcess(DWORD child_pid, int exit_code, bool wait);
	DWORD* pIMojoIpc = this->m_ptr_IMojoIpc,
		dwFunc = *((DWORD*)(*pIMojoIpc) + 8), dwPid = this->m_child_pid, call_result = 0;
	DWORD arg_wait = (DWORD)wait_;
	_asm
	{
		push arg_wait
		push exit_code
		push dwPid
		push pIMojoIpc
		call dwFunc
		mov call_result, eax
	}
	if (call_result == E_FAIL)
	{
		_last_err = L"pIMojoIpc->TerminateChildProcess Internal Error";
		return false;
	}
	_last_err = L"ERR_OK";
	return true;
}
bool qqimpl::QQIpcParentWrapper::ReLaunchChildProcess()
{
	//pIMojoIpc->ReLaunchChildProcess(DWORD child_pid);
	DWORD* pIMojoIpc = this->m_ptr_IMojoIpc,
		dwFunc = *((DWORD*)(*pIMojoIpc) + 9),dwPid = this->m_child_pid, call_result = 0;
	_asm
	{
		push dwPid
		push pIMojoIpc
		call dwFunc
		mov call_result, eax
	}
	if (call_result == E_FAIL)
	{
		_last_err = L"pIMojoIpc->ReLaunchChildProcess Internal Error";
		return false;
	}
	_last_err = L"ERR_OK";
	return true;
}

//����Ϊ��ChildIpc�����ķ�װ
void qqimpl::QQIpcChildWrapper::InitChildIpc()
{
	DWORD* pIMojoIpc = this->m_ptr_IMojoIpc,
		dwFunc = *((DWORD*)(*pIMojoIpc) + 3);//pIMojoIpc->InitChildIpc()
	_asm
	{
		push pIMojoIpc
		call dwFunc
	}
	_last_err = L"ERR_OK";
}

void qqimpl::QQIpcChildWrapper::InitLog(int level, void * callback)
{
	DWORD* pIMojoIpc = this->m_ptr_IMojoIpc,
		dwFunc = *((DWORD*)(*pIMojoIpc) + 4),	//pIMojoIpc->InitChildLog() 
		logLevel = level;
	_asm
	{
		push logLevel
		push callback
		push pIMojoIpc
		call dwFunc
	}
	_last_err = L"ERR_OK";
}

void qqimpl::QQIpcChildWrapper::SetChildReceiveCallback(void* callback)
{
	DWORD* pIMojoIpc = this->m_ptr_IMojoIpc,
		dwFunc = *((DWORD*)(*pIMojoIpc) + 5);//pIMojoIpc->SetChildReceiveCallbackAndCOMPtr()
	DWORD ipc_ptr_addr = (DWORD)(&(this->m_ptr_IMojoIpc));//�������Ҫ��һ��ȫ�ֱ���, ���������Ա������
	_asm
	{
		push ipc_ptr_addr
		push callback
		push pIMojoIpc
		call dwFunc
	}
	_last_err = L"ERR_OK";
}

void qqimpl::QQIpcChildWrapper::ChildSendIpcMessage(std::string command, std::string addition_msg)
{
	//pIMojoIpc->ChildSendMessageUsingBuffer(char* command, DWORD 0, DWORD 0, char* addition_msg, int addition_msg_size);
	DWORD* pIMojoIpc = this->m_ptr_IMojoIpc,
		dwFunc = *((DWORD*)(*pIMojoIpc) + 7), addition_len = 0;
	const char* command_c = command.c_str(), * addition_c = NULL;
	if (!addition_msg.empty())
	{
		addition_c = addition_msg.c_str();
		addition_len = addition_msg.length();
	}

	_asm
	{
		push addition_len
		push addition_c
		push 0
		push 0
		push command_c
		push pIMojoIpc
		call dwFunc
	}
	_last_err = L"ERR_OK";
}
#pragma optimize( "", on )

#ifdef NEED_WECHATOCR
//����Ϊqqocrwrapper��ʵ��
namespace qqimpl::qqocrwrapper
{
	int OnReadPushTransfer(DWORD arg1, DWORD arg2, DWORD arg3);
	int OnReserved2Transfer(DWORD arg1, DWORD arg2, DWORD arg3);
	int OnReserved3Transfer(DWORD arg1, DWORD arg2, DWORD arg3);
	int OnRemoteConnectTransfer(DWORD arg1, DWORD arg2);
	int OnRemoteDisconnectTransfer(DWORD arg1);
	int OnRemoteProcessLaunchedTransfer(DWORD arg1);
	int OnRemoteProcessLaunchFailedTransfer(DWORD arg1, DWORD arg2);
	int OnRemoteMojoErrorTransfer(DWORD arg1, DWORD arg2, DWORD  arg3);
	DWORD* __stdcall OnThrowException(DWORD arg1);
	int __stdcall OnReadPush(DWORD arg1, DWORD* arg2);
	void __stdcall OnReserved_(DWORD arg1, DWORD arg2);
	int __stdcall OnRemoteConnect(DWORD arg1);
	int __stdcall OnRemoteDisconnect();
	int __stdcall OnRemoteProcessLaunched();
	void __stdcall OnRemoteProcessLaunchFailed(DWORD arg1);
	int __stdcall OnRemoteMojoError(DWORD arg1, DWORD arg2);


	std::wstring _last_err;
	DWORD* g_callbacks[8] = { (DWORD*)OnReadPushTransfer, (DWORD*)OnReserved2Transfer, (DWORD*)OnReserved3Transfer, (DWORD*)OnRemoteConnectTransfer,
						(DWORD*)OnRemoteDisconnectTransfer, (DWORD*)OnRemoteProcessLaunchedTransfer, (DWORD*)OnRemoteProcessLaunchFailedTransfer,
						(DWORD*)OnRemoteMojoErrorTransfer };

	qqimpl::qqocrwrapper::QQOCRManager	g_ocr_manager;//��Ϊ����ȥ����, ���Ƿ�д, ��������ṹ�����Լ�����
	qqimpl::qqocrwrapper::MMMojoDLL		g_mmmojo;

	#define MAX_TASK_ID 8
	BYTE g_task_id[MAX_TASK_ID] = { 0 };//���ͬʱ��MAX_TASK_ID��task (1 - MAX_TASK_ID), 0������� 1����ռ��

	LPFN_ONUSRREADPUSH g_usr_readpush = NULL;
	std::wstring g_mmmojo_dll_path, g_usr_lib_path;

	bool is_disconnect_signal = false;//�Ƿ����disconnect�ص�����

	//��ȡ���е�Task ID
	int GetIdleTaskId()
	{
		for (size_t i = 0; i < MAX_TASK_ID; i++)
		{
			if (g_task_id[i] == 0)
			{
				g_task_id[i] = 1;
				return (i + 1);
			}
		}
		return 0;//����0˵��û�п��е�
	}

	bool SetTaskIdIdle(int num)
	{
		if (num < 1 || num > MAX_TASK_ID) return false;
		g_task_id[num - 1] = 0;
		return true;
	}


	void InitManager(std::wstring exe_path, std::wstring dll_path, bool need_timer)
	{
		g_ocr_manager.vTable = (OCRVTABLE*)calloc(sizeof(OCRVTABLE), sizeof(BYTE));

		g_ocr_manager.vTable->ThrowException = (DWORD*)OnThrowException;
		g_ocr_manager.vTable->OnReadPush = (DWORD*)OnReadPush;
		g_ocr_manager.vTable->Reserved_2 = (DWORD*)OnReserved_;
		g_ocr_manager.vTable->Reserved_3 = (DWORD*)OnReserved_;
		g_ocr_manager.vTable->OnRemoteConnect = (DWORD*)OnRemoteConnect;
		g_ocr_manager.vTable->OnRemoteDisconnect = (DWORD*)OnRemoteDisconnect;
		g_ocr_manager.vTable->OnRemoteMojoError = (DWORD*)OnRemoteMojoError;
		g_ocr_manager.vTable->OnRemoteProcessLaunched = (DWORD*)OnRemoteProcessLaunched;
		g_ocr_manager.vTable->OnRemoteProcessLaunchFailed = (DWORD*)OnRemoteProcessLaunchFailed;

		g_ocr_manager.isEnvInit = 0;
		g_ocr_manager.MMMojoEnvPtr = NULL;

		//�ж��Ƿ�Ϊ64λϵͳ
		SYSTEM_INFO si; GetNativeSystemInfo(&si);
		if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 || si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
			g_ocr_manager.isArch64 = true;
		else
			g_ocr_manager.isArch64 = false;
		
		std::cout << std::format("[*] isArch64?: {}\n", g_ocr_manager.isArch64);

		if (!exe_path.empty()) SetOcrExePath(exe_path);
		g_mmmojo_dll_path = dll_path;
		_last_err = L"ERR_OK";
	}

	void UnInitManager()
	{
		if (g_ocr_manager.vTable != NULL)
		{
			free(g_ocr_manager.vTable);
		}
		if (g_ocr_manager.MMMojoEnvPtr != NULL)
		{
			OCRdoUnInit();
		}
		is_disconnect_signal = false;
		g_usr_readpush = NULL;
		g_usr_lib_path = L"";
		_last_err = L"ERR_OK";
	}

	bool SetOcrExePath(std::wstring exe_path)
	{
		if (!std::filesystem::exists(exe_path))
		{
			_last_err = L"TencentOCR.exe Path is not exist!";
			return false;
		}

		if (!std::filesystem::is_regular_file(exe_path))
		{
			_last_err = L"The Path is a file!";
			return false;
		}

		g_ocr_manager.ExePath = exe_path;
		g_ocr_manager.isEnvInit = 1;
		_last_err = L"ERR_OK";
		return true;
	}

	bool SetOcrUsrLibPath(std::wstring usr_lib_path)
	{
		if (!std::filesystem::exists(usr_lib_path))
		{
			_last_err = L"UsrLib Path is not exist!";
			return false;
		}

		if (!std::filesystem::is_directory(usr_lib_path))
		{
			_last_err = L"The Path is a Directory!";
			return false;
		}
		g_usr_lib_path = usr_lib_path;
		return true;
	}

	std::wstring GetLastErrWStr()
	{
		return _last_err;
	}

	bool DoOCRTask(std::string pic_path)
	{
		if (g_ocr_manager.isArch64 == false)
		{
			_last_err = L"TencentOCR can't run on x86 32bit!";
			return false;
		}

		g_ocr_manager.PicPath = pic_path;

		if (g_mmmojo.hMMMojo == NULL || g_ocr_manager.MMMojoEnvPtr == NULL)
		{
			if (!OCRdoInit())//��OnRemoteConnect��ᷢ��SendOCRTask
				return false;

			_last_err = L"ERR_OK";
			return true;
		}


		int id = GetIdleTaskId();
		if (!SendOCRTask(id, pic_path))
		{
			SetTaskIdIdle(id);
			_last_err = L"[!] SendOCRTask Err\n";
			return false;
		}

		_last_err = L"ERR_OK";
		return true;
	}

	bool OCRdoInit()
	{
		if (g_ocr_manager.isArch64 == false)
		{
			_last_err = L"TencentOCR can't run on x86 32bit!";
			return false;
		}

		if (g_ocr_manager.ExePath.empty() || g_mmmojo_dll_path.empty())
		{
			_last_err = L"OCRdoInit Err: Exe or DLL Path is Empty!";
			return false;
		}


		if (g_mmmojo.hMMMojo == NULL)
		{
			g_mmmojo.hMMMojo = LoadLibraryW(g_mmmojo_dll_path.c_str());
			HMODULE hMMMojo = g_mmmojo.hMMMojo;
			if (hMMMojo == NULL)
			{
				_last_err = std::format(L"LoadMMMojoDll Err:[{}]\n", GetLastError());
				return false;
			}

			g_mmmojo.lpInitializeMMMojo = (LPFN_INITIALIZEMMMOJO)GetProcAddress(hMMMojo, "InitializeMMMojo");
			g_mmmojo.lpShutdownMMMojo = (LPFN_SHUTDOWNMMMOJO)GetProcAddress(hMMMojo, "ShutdownMMMojo");
			g_mmmojo.lpCreateMMMojoEnvironment = (LPFN_CREATEMMMOJOENVIRONMENT)GetProcAddress(hMMMojo, "CreateMMMojoEnvironment");
			g_mmmojo.lpSetMMMojoEnvironmentCallbacks = (LPFN_SETMMMOJOENVIRONMENTCALLBACKS)GetProcAddress(hMMMojo, "SetMMMojoEnvironmentCallbacks");
			g_mmmojo.lpSetMMMojoEnvironmentInitParams = (LPFN_SETMMMOJOENVIRONMENTINITPARAMS)GetProcAddress(hMMMojo, "SetMMMojoEnvironmentInitParams");
			g_mmmojo.lpAppendMMSubProcessSwitchNative = (LPFN_APPENDMMSUBPROCESSSWITCHNATIVE)GetProcAddress(hMMMojo, "AppendMMSubProcessSwitchNative");
			g_mmmojo.lpStartMMMojoEnvironment = (LPFN_STARTMMMOJOENVIRONMENT)GetProcAddress(hMMMojo, "StartMMMojoEnvironment");
			g_mmmojo.lpStopMMMojoEnvironment = (LPFN_STOPMMMOJOENVIRONMENT)GetProcAddress(hMMMojo, "StopMMMojoEnvironment");
			g_mmmojo.lpRemoveMMMojoEnvironment = (LPFN_REMOVEMMMOJOENVIRONMENT)GetProcAddress(hMMMojo, "RemoveMMMojoEnvironment");
			g_mmmojo.lpGetMMMojoReadInfoRequest = (LPFN_GETMMMOJOREADINFOREQUEST)GetProcAddress(hMMMojo, "GetMMMojoReadInfoRequest");
			g_mmmojo.lpGetMMMojoReadInfoAttach = (LPFN_GETMMMOJOREADINFOATTACH)GetProcAddress(hMMMojo, "GetMMMojoReadInfoAttach");
			g_mmmojo.lpRemoveMMMojoReadInfo = (LPFN_REMOVEMMMOJOREADINFO)GetProcAddress(hMMMojo, "RemoveMMMojoReadInfo");
			g_mmmojo.lpGetMMMojoReadInfoMethod = (LPFN_GETMMMOJOREADINFOMETHOD)GetProcAddress(hMMMojo, "GetMMMojoReadInfoMethod");
			g_mmmojo.lpGetMMMojoReadInfoSync = (LPFN_GETMMMOJOREADINFOSYNC)GetProcAddress(hMMMojo, "GetMMMojoReadInfoSync");
			g_mmmojo.lpCreateMMMojoWriteInfo = (LPFN_CREATEMMMOJOWRITEINFO)GetProcAddress(hMMMojo, "CreateMMMojoWriteInfo");
			g_mmmojo.lpGetMMMojoWriteInfoRequest = (LPFN_GETMMMOJOWRITEINFOREQUEST)GetProcAddress(hMMMojo, "GetMMMojoWriteInfoRequest");
			g_mmmojo.lpRemoveMMMojoWriteInfo = (LPFN_REMOVEMMMOJOWRITEINFO)GetProcAddress(hMMMojo, "RemoveMMMojoWriteInfo");
			g_mmmojo.lpGetMMMojoWriteInfoAttach = (LPFN_GETMMMOJOWRITEINFOATTACH)GetProcAddress(hMMMojo, "GetMMMojoWriteInfoAttach");
			g_mmmojo.lpSetMMMojoWriteInfoMessagePipe = (LPFN_SETMMMOJOWRITEINFOMESSAGEPIPE)GetProcAddress(hMMMojo, "SetMMMojoWriteInfoMessagePipe");
			g_mmmojo.lpSetMMMojoWriteInfoResponseSync = (LPFN_SETMMMOJOWRITEINFORESPONSESYNC)GetProcAddress(hMMMojo, "SetMMMojoWriteInfoResponseSync");
			g_mmmojo.lpSendMMMojoWriteInfo = (LPFN_SENDMMMOJOWRITEINFO)GetProcAddress(hMMMojo, "SendMMMojoWriteInfo");
			g_mmmojo.lpSwapMMMojoWriteInfoCallback = (LPFN_SWAPMMMOJOWRITEINFOCALLBACK)GetProcAddress(hMMMojo, "SwapMMMojoWriteInfoCallback");
			g_mmmojo.lpSwapMMMojoWriteInfoMessage = (LPFN_SWAPMMMOJOWRITEINFOMESSAGE)GetProcAddress(hMMMojo, "SwapMMMojoWriteInfoMessage");

			if (g_mmmojo.lpInitializeMMMojo == NULL)
			{
				_last_err = std::format(L"MMMojoDll GetProcAddress Err:[{}]\n", GetLastError());
				return false;
			}
		}

		if (g_ocr_manager.MMMojoEnvPtr == NULL)
		{
			g_mmmojo.lpInitializeMMMojo(0, 0);

			g_ocr_manager.MMMojoEnvPtr = g_mmmojo.lpCreateMMMojoEnvironment();
			if (g_ocr_manager.MMMojoEnvPtr == NULL)
			{
				_last_err = L"CreateMMMojoEnvironment Err\n";
				return false;
			}

			if (g_ocr_manager.vTable->OnReadPush == NULL)
			{
				_last_err = L"vTable->OnReadPush is invaild!\n";
				return false;
			}
			//����OCR�Ļص�����
			g_mmmojo.lpSetMMMojoEnvironmentCallbacks(g_ocr_manager.MMMojoEnvPtr, 0, (DWORD*)&g_ocr_manager);

			for (size_t i = 1; i <= 8; i++)
			{
				g_mmmojo.lpSetMMMojoEnvironmentCallbacks(g_ocr_manager.MMMojoEnvPtr, i, g_callbacks[i - 1]);
			}

			g_mmmojo.lpSetMMMojoEnvironmentInitParams(g_ocr_manager.MMMojoEnvPtr, 0, 1);
			g_mmmojo.lpSetMMMojoEnvironmentInitParams(g_ocr_manager.MMMojoEnvPtr, 2, (DWORD)g_ocr_manager.ExePath.c_str());

			std::filesystem::path exe_path = g_ocr_manager.ExePath;
			std::filesystem::path usr_lib_path = g_usr_lib_path;
			if (g_usr_lib_path.empty()) usr_lib_path = exe_path.parent_path().parent_path().parent_path();
			//�ж�usr_lib_path�治����mmmojo_64.dll�������򱨴�
			if (!std::filesystem::exists(usr_lib_path / "mmmojo_64.dll"))
			{
				_last_err = L"Missing mmmojo_64.dll required to start OCR";
				return false;
			}

			g_mmmojo.lpAppendMMSubProcessSwitchNative(g_ocr_manager.MMMojoEnvPtr, "user-lib-dir", (wchar_t*)usr_lib_path.wstring().c_str());

			g_mmmojo.lpStartMMMojoEnvironment(g_ocr_manager.MMMojoEnvPtr);
		}

		_last_err = L"ERR_OK";
		return true;
	}

	void OCRdoUnInit()
	{
		//QQ������TencentOCR.exe�������һ��Timer, ���������Զ�shutdown ocr����
		if (g_mmmojo.hMMMojo != NULL && g_ocr_manager.MMMojoEnvPtr != NULL)
		{
			g_mmmojo.lpStopMMMojoEnvironment(g_ocr_manager.MMMojoEnvPtr);
			g_mmmojo.lpRemoveMMMojoEnvironment(g_ocr_manager.MMMojoEnvPtr);
			//g_mmmojo.lpShutdownMMMojo();//���������ʵQQû���� Ҳ���ܵ�
		}
		g_ocr_manager.isSending = 0;
		g_ocr_manager.MMMojoEnvPtr = NULL;
	}

	bool SendOCRTask(long long task_id, std::string pic_path)
	{
		if (g_ocr_manager.isArch64 == false)
		{
			_last_err = L"WeChatOCR can't run on x86 32bit!";
			return false;
		}

		ocr_protobuf::OcrRequest ocr_request;
		ocr_request.set_unknow(0);
		ocr_request.set_task_id(task_id);
		ocr_protobuf::OcrRequest::PicPaths* pp = new ocr_protobuf::OcrRequest::PicPaths();
		pp->add_pic_path(pic_path);
		ocr_request.set_allocated_pic_path(pp);

		std::string data_;
		ocr_request.SerializeToString(&data_);

		DWORD* write_info = g_mmmojo.lpCreateMMMojoWriteInfo(1, 0, 1);
		if (write_info == NULL)
		{
			_last_err = L"CreateMMMojoWriteInfo Err";
			return false;
		}

		DWORD* request = g_mmmojo.lpGetMMMojoWriteInfoRequest(write_info, data_.size());//����protobufָ�� vector��ʽ

		memmove(request, data_.c_str(), data_.size());

		g_mmmojo.lpSendMMMojoWriteInfo(g_ocr_manager.MMMojoEnvPtr, write_info);

		_last_err = L"ERR_OK";
		return true;
	}

	void SetUsrReadPushCallbck(LPFN_ONUSRREADPUSH callback)
	{
		g_usr_readpush = callback;
	}

	bool HasDisconnectSignal()
	{
		return is_disconnect_signal;
	}

	//����Ϊwrapper.node + 0x2919D30 - wrapper.node + 0x2919D70
	_declspec(naked) int OnReadPushTransfer(DWORD arg1, DWORD arg2, DWORD arg3)
	{
		_asm
		{
			push ebp
			mov ebp, esp
			mov ecx, [ebp + 0x10]
			push[ebp + 0xC]
			push[ebp + 0x8]
			mov eax, [ecx]
			call dword ptr[eax + 0x4]
			pop ebp
			ret
		}
	}

	_declspec(naked) int OnReserved2Transfer(DWORD arg1, DWORD arg2, DWORD arg3)
	{
		_asm
		{
			push ebp
			mov ebp, esp
			mov ecx, [ebp + 0x10]
			push[ebp + 0xC]
			push[ebp + 0x8]
			mov eax, [ecx]
			call dword ptr[eax + 0x8]
			pop ebp
			ret
		}
	}

	_declspec(naked) int OnReserved3Transfer(DWORD arg1, DWORD arg2, DWORD arg3)
	{
		_asm
		{
			push ebp
			mov ebp, esp
			mov ecx, [ebp + 0x10]
			push[ebp + 0xC]
			push[ebp + 0x8]
			mov eax, [ecx]
			call dword ptr[eax + 0xC]
			pop ebp
			ret
		}
	}

	_declspec(naked) int OnRemoteConnectTransfer(DWORD arg1, DWORD arg2)
	{
		_asm
		{
			push ebp
			mov ebp, esp
			mov ecx, [ebp + 0xC]
			push[ebp + 0x8]
			mov eax, [ecx]
			call dword ptr[eax + 0x10]
			pop ebp
			ret
		}
	}

	_declspec(naked) int OnRemoteDisconnectTransfer(DWORD arg1)
	{
		_asm
		{
			push ebp
			mov ebp, esp
			mov ecx, [ebp + 0x8]
			mov eax, [ecx]
			pop ebp
			jmp dword ptr[eax + 0x14]
		}
	}

	_declspec(naked) int OnRemoteProcessLaunchedTransfer(DWORD arg1)
	{
		_asm
		{
			push ebp
			mov ebp, esp
			mov ecx, [ebp + 0x8]
			mov eax, [ecx]
			pop ebp
			jmp dword ptr[eax + 0x18]
		}
	}

	_declspec(naked) int OnRemoteProcessLaunchFailedTransfer(DWORD arg1, DWORD arg2)
	{
		_asm
		{
			push ebp
			mov ebp, esp
			mov ecx, [ebp + 0xC]
			push[ebp + 0x8]
			mov eax, [ecx]
			call dword ptr[eax + 0x1C]
			pop ebp
			ret
		}
	}

	_declspec(naked) int OnRemoteMojoErrorTransfer(DWORD arg1, DWORD arg2, DWORD  arg3)
	{
		_asm
		{
			push ebp
			mov ebp, esp
			mov ecx, [ebp + 0x10]
			push[ebp + 0xC]
			push[ebp + 0x8]
			mov eax, [ecx]
			call dword ptr[eax + 0x20]
			pop ebp
			ret
		}
	}

	//����Ϊ�Լ�ʵ�ֵ������
	DWORD* __stdcall OnThrowException(DWORD arg1)
	{
		return 0;
	}

	//��ʵ��__thiscall
	int __stdcall OnReadPush(DWORD arg1, DWORD* arg2)
	{
		QQOCRManager* pThis = NULL;
		__asm mov pThis, ecx

		//arg1ò����ԶΪ1
		std::cout << std::format("[*] OnReadPush: this:0x{:08X} TaskID:{}, ARG2:0x{:08X}\n", (DWORD)pThis, arg1, (DWORD)arg2);

		DWORD request_size = 0, attach_size = 0;
		DWORD* read_request = g_mmmojo.lpGetMMMojoReadInfoRequest(arg2, &request_size);
		DWORD* read_attach = g_mmmojo.lpGetMMMojoReadInfoAttach(arg2, &attach_size);

		ocr_protobuf::OcrResponse ocr_response;
		ocr_response.ParseFromArray(read_request, request_size);

		std::cout << std::format("[*] OnReadPush type:{} task id:{} errCode:{}\n", ocr_response.type(), ocr_response.task_id(), ocr_response.err_code());
		if (ocr_response.type() == 0)
		{
			SetTaskIdIdle(ocr_response.task_id());
			std::cout << "[*] OcrResult:\n[\n";
			for (int i = 0; i < ocr_response.ocr_result().single_result_size(); i++)
			{
				ocr_protobuf::OcrResponse::OcrResult::SingleResult single_result = ocr_response.ocr_result().single_result(i);
				std::cout << std::format("\tRECT:[lx:{}, ly:{}, rx:{}, ry{}]\n", single_result.lx(), single_result.ly(), single_result.rx(), single_result.ry());
				std::string utf8str = single_result.single_str_utf8();
				printf("\tUTF8STR:[");
				for (size_t j = 0; j < utf8str.size(); j++)
				{
					printf("%02X ", (BYTE)utf8str[j]);
				}
				puts("]");
			}
			puts("]");
		}

		if (g_usr_readpush != NULL) g_usr_readpush(ocr_response);//�����û�����

		g_mmmojo.lpRemoveMMMojoReadInfo(arg2);

		return 0;
	}

	void __stdcall OnReserved_(DWORD arg1, DWORD arg2)
	{
		;
	}

	//��ʵ��__thiscall
	int __stdcall OnRemoteConnect(DWORD arg1)
	{
		QQOCRManager* pThis = NULL;
		__asm mov pThis, ecx

		std::cout << std::format("[*] OnRemoteConnect: this: 0x{:08X}\n", (DWORD)pThis);

		int id = GetIdleTaskId();
		if (!SendOCRTask(id, g_ocr_manager.PicPath))
		{
			SetTaskIdIdle(id);
			std::cout << "[!] SendOCRTask Err\n";
		}
		std::cout << "[+] SendOCRTask OK! : " << id << std::endl;

		return 0;
	}

	int __stdcall OnRemoteDisconnect()
	{
		QQOCRManager* pThis = NULL;
		__asm mov pThis, ecx

		std::cout << std::format("[*] OnRemoteDisconnect: this: 0x{:08X}\n", (DWORD)pThis);
		is_disconnect_signal = true;
		return 0;
	}

	int __stdcall OnRemoteProcessLaunched()
	{
		QQOCRManager* pThis = NULL;
		__asm mov pThis, ecx

		std::cout << std::format("[*] OnRemoteProcessLaunched: this: 0x{:08X}\n", (DWORD)pThis);
		return 0;
	}

	void __stdcall OnRemoteProcessLaunchFailed(DWORD arg1)
	{
		QQOCRManager* pThis = NULL;
		__asm mov pThis, ecx

		std::cout << std::format("[*] OnRemoteProcessLaunchFailed: this: 0x{:08X}\n", (DWORD)pThis);
	}

	int __stdcall OnRemoteMojoError(DWORD arg1, DWORD arg2)
	{
		QQOCRManager* pThis = NULL;
		__asm mov pThis, ecx

		std::cout << std::format("[*] OnRemoteMojoError: this: 0x{:08X}\n", (DWORD)pThis);
		return 0;
	}
}
#endif // NEED_WECHATOCR

#endif // !_WIN64