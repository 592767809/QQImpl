#include "QQOcr.h"

#ifdef _WIN64
#include "QQOcrAsm.h"
#endif // _WIN64

namespace qqimpl
{
namespace qqocr
{
	//����Ϊchar��wchar��ת��
	std::wstring Utf8ToUnicode(std::string utf8_str)
	{
		if (utf8_str.empty())
			return std::wstring();

		const auto size_needed = MultiByteToWideChar(CP_UTF8, 0, &utf8_str.at(0), (int)utf8_str.size(), nullptr, 0);
		if (size_needed <= 0)
		{
			throw std::runtime_error("MultiByteToWideChar() failed: " + std::to_string(size_needed));
		}

		std::wstring result(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &utf8_str.at(0), (int)utf8_str.size(), &result.at(0), size_needed);
		return result;
	}

	std::string UnicodeToUtf8(std::wstring utf16_str)
	{
		if (utf16_str.empty())
			return std::string();

		const auto size_needed = WideCharToMultiByte(CP_UTF8, 0, &utf16_str.at(0), (int)utf16_str.size(), nullptr, 0, nullptr, nullptr);
		if (size_needed <= 0)
		{
			throw std::runtime_error("WideCharToMultiByte() failed: " + std::to_string(size_needed));
		}

		std::string result(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, &utf16_str.at(0), (int)utf16_str.size(), &result.at(0), size_needed, nullptr, nullptr);
		return result;
	}


	typedef DWORD_PTR(__cdecl* LPFN_INITIALIZEMMMOJO)(DWORD_PTR, DWORD_PTR);
	typedef DWORD_PTR(__cdecl* LPFN_SHUTDOWNMMMOJO)();
	typedef DWORD_PTR* (__cdecl* LPFN_CREATEMMMOJOENVIRONMENT)();
	typedef DWORD_PTR* (__cdecl* LPFN_SETMMMOJOENVIRONMENTCALLBACKS)(DWORD_PTR*, int, DWORD_PTR*);//arg: ��һ���������� ��������ʱ���ص���ָ��, ���, ����ָ��
	typedef DWORD_PTR(__cdecl* LPFN_SETMMMOJOENVIRONMENTINITPARAMS)(DWORD_PTR*, DWORD_PTR, DWORD_PTR);
	typedef void(__cdecl* LPFN_APPENDMMSUBPROCESSSWITCHNATIVE)(DWORD_PTR*, const char*, wchar_t*);//arg: "user-lib-dir", ����Ŀ¼
	typedef DWORD_PTR(__cdecl* LPFN_STARTMMMOJOENVIRONMENT)(DWORD_PTR*);
	typedef DWORD_PTR(__cdecl* LPFN_STOPMMMOJOENVIRONMENT)(DWORD_PTR*);
	typedef DWORD_PTR(__cdecl* LPFN_REMOVEMMMOJOENVIRONMENT)(DWORD_PTR*);
	typedef DWORD_PTR* (__cdecl* LPFN_GETMMMOJOREADINFOREQUEST)(DWORD_PTR*, DWORD_PTR*);
	typedef DWORD_PTR* (__cdecl* LPFN_GETMMMOJOREADINFOATTACH)(DWORD_PTR*, DWORD_PTR*);
	typedef DWORD_PTR(__cdecl* LPFN_REMOVEMMMOJOREADINFO)(DWORD_PTR*);
	typedef DWORD_PTR(__cdecl* LPFN_GETMMMOJOREADINFOMETHOD)(DWORD_PTR*);
	typedef DWORD_PTR(__cdecl* LPFN_GETMMMOJOREADINFOSYNC)(DWORD_PTR*);
	typedef DWORD_PTR* (__cdecl* LPFN_CREATEMMMOJOWRITEINFO)(DWORD_PTR, DWORD_PTR, DWORD_PTR);
	typedef DWORD_PTR* (__cdecl* LPFN_GETMMMOJOWRITEINFOREQUEST)(DWORD_PTR*, DWORD_PTR);
	typedef DWORD_PTR(__cdecl* LPFN_REMOVEMMMOJOWRITEINFO)(DWORD_PTR*);
	typedef DWORD_PTR* (__cdecl* LPFN_GETMMMOJOWRITEINFOATTACH)(DWORD_PTR*, DWORD_PTR*);
	typedef DWORD_PTR(__cdecl* LPFN_SETMMMOJOWRITEINFOMESSAGEPIPE)(DWORD_PTR*, DWORD_PTR*);
	typedef DWORD_PTR(__cdecl* LPFN_SETMMMOJOWRITEINFORESPONSESYNC)(DWORD_PTR*, DWORD_PTR*);
	typedef DWORD_PTR(__cdecl* LPFN_SENDMMMOJOWRITEINFO)(DWORD_PTR*, DWORD_PTR*);
	typedef DWORD_PTR(__cdecl* LPFN_SWAPMMMOJOWRITEINFOCALLBACK)(DWORD_PTR*, DWORD_PTR*);
	typedef DWORD_PTR(__cdecl* LPFN_SWAPMMMOJOWRITEINFOMESSAGE)(DWORD_PTR*, DWORD_PTR*);

	struct MMMojoDLL
	{
		HMODULE	hMMMojo;
		LPFN_INITIALIZEMMMOJO				lpInitializeMMMojo;
		LPFN_SHUTDOWNMMMOJO					lpShutdownMMMojo;
		LPFN_CREATEMMMOJOENVIRONMENT		lpCreateMMMojoEnvironment;
		LPFN_SETMMMOJOENVIRONMENTCALLBACKS	lpSetMMMojoEnvironmentCallbacks;
		LPFN_SETMMMOJOENVIRONMENTINITPARAMS	lpSetMMMojoEnvironmentInitParams;
		LPFN_APPENDMMSUBPROCESSSWITCHNATIVE	lpAppendMMSubProcessSwitchNative;
		LPFN_STARTMMMOJOENVIRONMENT			lpStartMMMojoEnvironment;
		LPFN_STOPMMMOJOENVIRONMENT			lpStopMMMojoEnvironment;
		LPFN_REMOVEMMMOJOENVIRONMENT		lpRemoveMMMojoEnvironment;
		LPFN_GETMMMOJOREADINFOREQUEST		lpGetMMMojoReadInfoRequest;
		LPFN_GETMMMOJOREADINFOATTACH		lpGetMMMojoReadInfoAttach;
		LPFN_REMOVEMMMOJOREADINFO			lpRemoveMMMojoReadInfo;
		LPFN_GETMMMOJOREADINFOMETHOD		lpGetMMMojoReadInfoMethod;
		LPFN_GETMMMOJOREADINFOSYNC			lpGetMMMojoReadInfoSync;
		LPFN_CREATEMMMOJOWRITEINFO			lpCreateMMMojoWriteInfo;
		LPFN_GETMMMOJOWRITEINFOREQUEST		lpGetMMMojoWriteInfoRequest;
		LPFN_REMOVEMMMOJOWRITEINFO			lpRemoveMMMojoWriteInfo;
		LPFN_GETMMMOJOWRITEINFOATTACH		lpGetMMMojoWriteInfoAttach;
		LPFN_SETMMMOJOWRITEINFOMESSAGEPIPE	lpSetMMMojoWriteInfoMessagePipe;
		LPFN_SETMMMOJOWRITEINFORESPONSESYNC	lpSetMMMojoWriteInfoResponseSync;
		LPFN_SENDMMMOJOWRITEINFO			lpSendMMMojoWriteInfo;
		LPFN_SWAPMMMOJOWRITEINFOCALLBACK	lpSwapMMMojoWriteInfoCallback;
		LPFN_SWAPMMMOJOWRITEINFOMESSAGE		lpSwapMMMojoWriteInfoMessage;
	};


#ifdef _WIN64
	DWORD64* OnThrowException(DWORD64 arg1);
	int OnReadPush(DWORD64 arg_this, DWORD64 arg1, DWORD64* arg2);
	void OnReserved_(DWORD64 arg_this, DWORD64 arg1, DWORD64 arg2);
	int OnRemoteConnect(DWORD64 arg_this, DWORD64 arg1);
	int OnRemoteDisconnect(DWORD64 arg_this);
	int OnRemoteProcessLaunched(DWORD64 arg_this);
	void OnRemoteProcessLaunchFailed(DWORD64 arg_this, DWORD64 arg1);
	int OnRemoteMojoError(DWORD64 arg_this, DWORD64 arg1, DWORD64 arg2);
#else
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
#endif 

	std::wstring _last_err;
	DWORD_PTR* g_callbacks[8] = { (DWORD_PTR*)OnReadPushTransfer, (DWORD_PTR*)OnReserved2Transfer, (DWORD_PTR*)OnReserved3Transfer, (DWORD_PTR*)OnRemoteConnectTransfer,
						(DWORD_PTR*)OnRemoteDisconnectTransfer, (DWORD_PTR*)OnRemoteProcessLaunchedTransfer, (DWORD_PTR*)OnRemoteProcessLaunchFailedTransfer,
						(DWORD_PTR*)OnRemoteMojoErrorTransfer };

	qqimpl::qqocr::QQOCRManager		g_ocr_manager;//��Ϊ����ȥ����, ���Ƿ�д, ��������ṹ�����Լ�����
	MMMojoDLL						g_mmmojo;

	#define MAX_TASK_ID 32
	BYTE g_task_id[MAX_TASK_ID] = { 0 };//���ͬʱ��MAX_TASK_ID��task (1 - MAX_TASK_ID), 0������� 1����ռ��
	std::map<int, std::string> g_id_path;//TASK_ID�Ͷ�Ӧ��·��, protobuf��û��ͼƬ·������Ϣ, ֻ���Լ���¼һ���� ��SendOCRTask�������¼
	std::mutex task_mutex;//���ڻ����ȡTASK ID

	qqimpl::qqocr::LPFN_ONUSRREADPUSH g_usr_readpush = NULL;//void UsrReadPush(std::string pic_path, ocr_protobuf::OcrResponse ocr_response);
	std::wstring g_usr_lib_path;

	bool is_disconnect_signal = false;//�Ƿ������disconnect�ص�����
	bool is_remoteconnect_singal = false;//�Ƿ������remote connect�ص�����
	std::mutex connect_mutex;//mutex��condition_variable����ֻ���ڻص���RemoteConnectҲ����Connect�Ϻ��ٷ���OCR����
	std::condition_variable connect_con_var;

	//��ȡ���е�Task ID
	int GetIdleTaskId()
	{
		task_mutex.lock();
		for (size_t i = 0; i < MAX_TASK_ID; i++)
		{
			if (g_task_id[i] == 0)
			{
				g_task_id[i] = 1;
				task_mutex.unlock();
				return (i + 1);
			}
		}
		task_mutex.unlock();
		return 0;//����0˵��û�п��е�
	}

	bool SetTaskIdIdle(int num)
	{
		if (num < 1 || num > MAX_TASK_ID) return false;
		g_task_id[num - 1] = 0;
		return true;
	}

	void InitManagerA(std::string exe_path, std::string usr_lib_path)
	{
		std::wstring exe_path_w = Utf8ToUnicode(exe_path),
					 usr_lib_path_w = Utf8ToUnicode(usr_lib_path);
		qqimpl::qqocr::InitManager(exe_path_w, usr_lib_path_w);
	}

	void InitManager(std::wstring exe_path, std::wstring usr_lib_path)
	{
		g_ocr_manager.vTable = (qqimpl::qqocr::OCRVTABLE*)calloc(sizeof(qqimpl::qqocr::OCRVTABLE), sizeof(BYTE));

		g_ocr_manager.vTable->ThrowException = (DWORD_PTR*)OnThrowException;
		g_ocr_manager.vTable->OnReadPush = (DWORD_PTR*)OnReadPush;
		g_ocr_manager.vTable->Reserved_2 = (DWORD_PTR*)OnReserved_;
		g_ocr_manager.vTable->Reserved_3 = (DWORD_PTR*)OnReserved_;
		g_ocr_manager.vTable->OnRemoteConnect = (DWORD_PTR*)OnRemoteConnect;
		g_ocr_manager.vTable->OnRemoteDisconnect = (DWORD_PTR*)OnRemoteDisconnect;
		g_ocr_manager.vTable->OnRemoteMojoError = (DWORD_PTR*)OnRemoteMojoError;
		g_ocr_manager.vTable->OnRemoteProcessLaunched = (DWORD_PTR*)OnRemoteProcessLaunched;
		g_ocr_manager.vTable->OnRemoteProcessLaunchFailed = (DWORD_PTR*)OnRemoteProcessLaunchFailed;

		g_ocr_manager.isEnvInit = 0;
		g_ocr_manager.MMMojoEnvPtr = NULL;

		//�ж��Ƿ�Ϊ64λϵͳ
		SYSTEM_INFO si; GetNativeSystemInfo(&si);
		if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 || si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
			g_ocr_manager.isArch64 = true;
		else
			g_ocr_manager.isArch64 = false;

#ifdef _DEBUG
		std::cout << "[*] isArch64?: " << g_ocr_manager.isArch64 << std::endl;
#endif // _DEBUG


		if (!exe_path.empty()) SetOcrExePath(exe_path);
		if (!usr_lib_path.empty()) SetOcrUsrLibPath(usr_lib_path);
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
		is_remoteconnect_singal = false;
		g_usr_readpush = NULL;
		g_usr_lib_path = L"";
		g_id_path.clear();
		for (size_t i = 0; i < MAX_TASK_ID; i++) SetTaskIdIdle(i + 1);
		_last_err = L"ERR_OK";
	}

	bool SetOcrExePathA(std::string exe_path)
	{
		std::wstring exe_path_w = Utf8ToUnicode(exe_path);
		return SetOcrExePath(exe_path_w);
	}

	bool SetOcrExePath(std::wstring exe_path)
	{
		g_ocr_manager.ExePath = exe_path;
		g_ocr_manager.isEnvInit = 1;
		_last_err = L"ERR_OK";
		return true;
	}

	bool SetOcrUsrLibPathA(std::string usr_lib_path)
	{
		std::wstring usr_lib_path_w = Utf8ToUnicode(usr_lib_path);
		return SetOcrUsrLibPath(usr_lib_path_w);
	}

	bool SetOcrUsrLibPath(std::wstring usr_lib_path)
	{
		g_usr_lib_path = usr_lib_path;
		return true;
	}

	std::string GetLastErrStrA()
	{
		return UnicodeToUtf8(_last_err);
	}

	std::wstring GetLastErrStr()
	{
		return _last_err;
	}

	bool DoOCRTaskA(std::string pic_path)
	{
		if (g_ocr_manager.isEnvInit < 1)
		{
			_last_err = L"OCR Env Doesn't Init!";
			return false;
		}

		if (g_ocr_manager.isArch64 == false)
		{
			_last_err = L"TencentOCR can't run on x86 32bit!";
			return false;
		}

		g_ocr_manager.PicPath = pic_path;

		if (g_mmmojo.hMMMojo == NULL || g_ocr_manager.MMMojoEnvPtr == NULL)
		{
			if (!OCRdoInit())
				return false;
		}


		int id = GetIdleTaskId();
		if (id == 0)
		{
			_last_err = L"No Idle TASK ID";
			return false;
		}
		if (!SendOCRTask(id, pic_path))
		{
			SetTaskIdIdle(id);
			_last_err = L"[!] SendOCRTask Err\n";
			return false;
		}

		_last_err = L"ERR_OK";
		return true;
	}

	bool DoOCRTask(std::wstring pic_path)
	{
		std::string pic_path_a = UnicodeToUtf8(pic_path);
		return DoOCRTaskA(pic_path_a);
	}

	bool OCRdoInit()
	{
		if (g_ocr_manager.isArch64 == false)
		{
			_last_err = L"TencentOCR can't run on x86 32bit!";
			return false;
		}

		if (g_ocr_manager.ExePath.empty() || g_usr_lib_path.empty())
		{
			_last_err = L"OCRdoInit Err: Exe or DLL Path is Empty!";
			return false;
		}

		if (g_mmmojo.hMMMojo == NULL)
		{
			std::wstring mmmojo_dll_path = g_usr_lib_path, dll_name;
#ifdef _WIN64
			dll_name = L"mmmojo_64.dll";
#else
			dll_name = L"mmmojo.dll";
#endif // _WIN64

			if ((mmmojo_dll_path.back() == L'\\') || (mmmojo_dll_path.back() == L'/'))
				mmmojo_dll_path += dll_name;
			else
				mmmojo_dll_path += (L"\\" + dll_name);

			g_mmmojo.hMMMojo = LoadLibraryW(mmmojo_dll_path.c_str());
			HMODULE hMMMojo = g_mmmojo.hMMMojo;
			if (hMMMojo == NULL)
			{
				_last_err = L"LoadMMMojoDll Err";
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
				_last_err = L"MMMojoDll GetProcAddress Err";
				return false;
			}
		}

		if (g_ocr_manager.MMMojoEnvPtr == NULL)
		{
			g_mmmojo.lpInitializeMMMojo(0, 0);

			g_ocr_manager.MMMojoEnvPtr = g_mmmojo.lpCreateMMMojoEnvironment();
			if (g_ocr_manager.MMMojoEnvPtr == NULL)
			{
				_last_err = L"CreateMMMojoEnvironment Err";
				return false;
			}

			if (g_ocr_manager.vTable->OnReadPush == NULL)
			{
				_last_err = L"vTable->OnReadPush is invaild!";
				return false;
			}
			//����OCR�Ļص�����
			g_mmmojo.lpSetMMMojoEnvironmentCallbacks(g_ocr_manager.MMMojoEnvPtr, 0, (DWORD_PTR*)&g_ocr_manager);

			for (size_t i = 1; i <= 8; i++)
			{
				g_mmmojo.lpSetMMMojoEnvironmentCallbacks(g_ocr_manager.MMMojoEnvPtr, i, g_callbacks[i - 1]);
			}

			g_mmmojo.lpSetMMMojoEnvironmentInitParams(g_ocr_manager.MMMojoEnvPtr, 0, 1);
			g_mmmojo.lpSetMMMojoEnvironmentInitParams(g_ocr_manager.MMMojoEnvPtr, 2, (DWORD_PTR)g_ocr_manager.ExePath.c_str());

			if (g_usr_lib_path.empty())
			{
				_last_err = L"Usr Lib Path is Empty!";
				return false;
			}

			g_mmmojo.lpAppendMMSubProcessSwitchNative(g_ocr_manager.MMMojoEnvPtr, "user-lib-dir", (wchar_t*)g_usr_lib_path.c_str());

			g_mmmojo.lpStartMMMojoEnvironment(g_ocr_manager.MMMojoEnvPtr);
		}

		g_ocr_manager.isEnvInit = 2;
		_last_err = L"ERR_OK";
		return true;
	}


	void OCRdoUnInit()
	{
		if (g_mmmojo.hMMMojo != NULL && g_ocr_manager.MMMojoEnvPtr != NULL)
		{
			g_mmmojo.lpStopMMMojoEnvironment(g_ocr_manager.MMMojoEnvPtr);
			g_mmmojo.lpRemoveMMMojoEnvironment(g_ocr_manager.MMMojoEnvPtr);
			//g_mmmojo.lpShutdownMMMojo();//���������ʵQQû���� Ҳ���ܵ�
		}
		g_ocr_manager.isSending = 0;
		g_ocr_manager.MMMojoEnvPtr = NULL;

		if (g_ocr_manager.ExePath.empty())
			g_ocr_manager.isEnvInit = 0;
		else
			g_ocr_manager.isEnvInit = 1;

		is_remoteconnect_singal = false;
		is_disconnect_signal = false;
	}

	bool SendOCRTask(long long task_id, std::string pic_path)
	{
		if (g_ocr_manager.isArch64 == false)
		{
			_last_err = L"WeChatOCR can't run on x86 32bit!";
			return false;
		}

		g_id_path[task_id] = pic_path;//��¼ID��ͼƬ·���Ĺ�ϵ

		ocr_protobuf::OcrRequest ocr_request;
		ocr_request.set_unknow(0);
		ocr_request.set_task_id(task_id);
		ocr_protobuf::OcrRequest::PicPaths* pp = new ocr_protobuf::OcrRequest::PicPaths();
		pp->add_pic_path(pic_path);
		ocr_request.set_allocated_pic_path(pp);

		std::string data_;
		ocr_request.SerializeToString(&data_);

		DWORD_PTR* write_info = g_mmmojo.lpCreateMMMojoWriteInfo(1, 0, 1);
		if (write_info == NULL)
		{
			_last_err = L"CreateMMMojoWriteInfo Err";
			return false;
		}

		DWORD_PTR* request = g_mmmojo.lpGetMMMojoWriteInfoRequest(write_info, data_.size());//����protobufָ�� vector��ʽ

		memmove(request, data_.c_str(), data_.size());

		//��Send֮ǰ, ����ȴ�OCR��������
		std::unique_lock<std::mutex> lock(connect_mutex);
		connect_con_var.wait(lock, []() { return is_remoteconnect_singal; });
		g_mmmojo.lpSendMMMojoWriteInfo(g_ocr_manager.MMMojoEnvPtr, write_info);

		_last_err = L"ERR_OK";
		return true;
	}

	void SetUsrReadPushCallbck(qqimpl::qqocr::LPFN_ONUSRREADPUSH callback)
	{
		g_usr_readpush = callback;
	}

	bool HasDisconnectSignal()
	{
		return is_disconnect_signal;
	}

	//����Ϊ�Լ�ʵ�ֵ������
#ifdef _WIN64
	DWORD64* OnThrowException(DWORD64 arg1)
	{
		return 0;
	}

	int OnReadPush(DWORD64 arg_this, DWORD64 arg1, DWORD64* arg2)
	{
		QQOCRManager* pThis = (QQOCRManager*)arg_this;

		//std::cout << std::format("[*] OnReadPush: this:0x{:08X} ARG1:{}, ARG2:0x{:08X}\n", (DWORD64)pThis, arg1, (DWORD64)arg2);

		DWORD64 request_size = 0, attach_size = 0;
		DWORD64* read_request = g_mmmojo.lpGetMMMojoReadInfoRequest(arg2, &request_size);
		DWORD64* read_attach = g_mmmojo.lpGetMMMojoReadInfoAttach(arg2, &attach_size);

		ocr_protobuf::OcrResponse ocr_response;
		ocr_response.ParseFromArray(read_request, request_size);

		if (ocr_response.type() == 1)//WeChatOCR�����ɹ�
		{
			if (g_usr_readpush != NULL) g_usr_readpush("", ocr_response);//�����û�����
			g_mmmojo.lpRemoveMMMojoReadInfo(arg2);
			return 0;
		}

		int32_t task_id_ = ocr_response.task_id();
		std::string pic_path;

		if (g_id_path.count(task_id_) == 0)
		{
			//[!] OnReadPush: Get PicPath From ID_PATH_MAP Failed!\;
			g_mmmojo.lpRemoveMMMojoReadInfo(arg2);
			return 0;
		}
		pic_path = g_id_path[task_id_];

		if (ocr_response.type() == 0)
		{
			//����
		}

		if (g_usr_readpush != NULL) //�����û�����
			g_usr_readpush(pic_path, ocr_response);

		g_mmmojo.lpRemoveMMMojoReadInfo(arg2);

		//ɾ��id��pic_path��map
		SetTaskIdIdle(task_id_);
		g_id_path.erase(task_id_);
		return 0;
	}

	void OnReserved_(DWORD64 arg_this, DWORD64 arg1, DWORD64 arg2)
	{
		;
	}

	int OnRemoteConnect(DWORD64 arg_this, DWORD64 arg1)
	{
		qqimpl::qqocr::QQOCRManager* pThis = (qqimpl::qqocr::QQOCRManager*)arg_this;

		//std::cout << std::format("[*] OnRemoteConnect: this: 0x{:08X}\n", (DWORD64)pThis);

		std::lock_guard<std::mutex> lock(connect_mutex);
		is_remoteconnect_singal = true;
		connect_con_var.notify_all();

		return 0;
	}

	int __stdcall OnRemoteDisconnect(DWORD64 arg_this)
	{
		qqimpl::qqocr::QQOCRManager* pThis = (qqimpl::qqocr::QQOCRManager*)arg_this;

		//std::cout << std::format("[*] OnRemoteDisconnect: this: 0x{:08X}\n", (DWORD64)pThis);
		is_disconnect_signal = true;
		return 0;
	}

	int __stdcall OnRemoteProcessLaunched(DWORD64 arg_this)
	{
		qqimpl::qqocr::QQOCRManager* pThis = (qqimpl::qqocr::QQOCRManager*)arg_this;

		//std::cout << std::format("[*] OnRemoteProcessLaunched: this: 0x{:08X}\n", (DWORD64)pThis);
		return 0;
	}

	void __stdcall OnRemoteProcessLaunchFailed(DWORD64 arg_this, DWORD64 arg1)
	{
		qqimpl::qqocr::QQOCRManager* pThis = (qqimpl::qqocr::QQOCRManager*)arg_this;

		//std::cout << std::format("[*] OnRemoteProcessLaunchFailed: this: 0x{:08X}\n", (DWORD64)pThis);
	}

	int __stdcall OnRemoteMojoError(DWORD64 arg_this, DWORD64 arg1, DWORD64 arg2)
	{
		qqimpl::qqocr::QQOCRManager* pThis = (qqimpl::qqocr::QQOCRManager*)arg_this;

		//std::cout << std::format("[*] OnRemoteMojoError: this: 0x{:08X}\n", (DWORD64)pThis);
		return 0;
	}
#else
	DWORD* __stdcall OnThrowException(DWORD arg1)
	{
		return 0;
	}

	//��ʵ��__thiscall
	int __stdcall OnReadPush(DWORD arg1, DWORD* arg2)
	{
		qqimpl::qqocr::QQOCRManager* pThis = NULL;
		__asm mov pThis, ecx


		DWORD_PTR request_size = 0, attach_size = 0;
		DWORD_PTR* read_request = g_mmmojo.lpGetMMMojoReadInfoRequest(arg2, &request_size);
		DWORD_PTR* read_attach = g_mmmojo.lpGetMMMojoReadInfoAttach(arg2, &attach_size);

		ocr_protobuf::OcrResponse ocr_response;
		ocr_response.ParseFromArray(read_request, request_size);

		if (ocr_response.type() == 1)//WeChatOCR�����ɹ�
		{
			if (g_usr_readpush != NULL) g_usr_readpush("", ocr_response);//�����û�����
			g_mmmojo.lpRemoveMMMojoReadInfo(arg2);
			return 0;
		}

		int32_t task_id_ = ocr_response.task_id();
		std::string pic_path;

		if (g_id_path.count(task_id_) == 0)
		{
			//OnReadPush: Get PicPath From ID_PATH_MAP Failed!
			g_mmmojo.lpRemoveMMMojoReadInfo(arg2);
			return 0;
		}
		pic_path = g_id_path[task_id_];

		if (ocr_response.type() == 0)
		{
			//����
		}

		if (g_usr_readpush != NULL) //�����û�����
			g_usr_readpush(pic_path, ocr_response);

		g_mmmojo.lpRemoveMMMojoReadInfo(arg2);

		//ɾ��id��pic_path��map
		SetTaskIdIdle(task_id_);
		g_id_path.erase(task_id_);
		return 0;
	}

	void __stdcall OnReserved_(DWORD arg1, DWORD arg2)
	{
		;
	}

	//��ʵ��__thiscall
	int __stdcall OnRemoteConnect(DWORD arg1)
	{
		qqimpl::qqocr::QQOCRManager* pThis = NULL;
		__asm mov pThis, ecx

		//std::cout << std::format("[*] OnRemoteConnect: this: 0x{:08X}\n", (DWORD64)pThis);

		std::lock_guard<std::mutex> lock(connect_mutex);
		is_remoteconnect_singal = true;
		connect_con_var.notify_all();

		return 0;
	}

	int __stdcall OnRemoteDisconnect()
	{
		qqimpl::qqocr::QQOCRManager* pThis = NULL;
		__asm mov pThis, ecx

		//std::cout << std::format("[*] OnRemoteDisconnect: this: 0x{:08X}\n", (DWORD)pThis);
		is_disconnect_signal = true;
		return 0;
	}

	int __stdcall OnRemoteProcessLaunched()
	{
		qqimpl::qqocr::QQOCRManager* pThis = NULL;
		__asm mov pThis, ecx

		//std::cout << std::format("[*] OnRemoteProcessLaunched: this: 0x{:08X}\n", (DWORD)pThis);
		return 0;
	}

	void __stdcall OnRemoteProcessLaunchFailed(DWORD arg1)
	{
		qqimpl::qqocr::QQOCRManager* pThis = NULL;
		__asm mov pThis, ecx

		//std::cout << std::format("[*] OnRemoteProcessLaunchFailed: this: 0x{:08X}\n", (DWORD)pThis);
	}

	int __stdcall OnRemoteMojoError(DWORD arg1, DWORD arg2)
	{
		qqimpl::qqocr::QQOCRManager* pThis = NULL;
		__asm mov pThis, ecx

		//std::cout << std::format("[*] OnRemoteMojoError: this: 0x{:08X}\n", (DWORD)pThis);
		return 0;
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
#endif // _WIN64
}
}

