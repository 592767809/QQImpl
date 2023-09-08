#pragma once
//ͨ���������ʵ��һЩQQ��IPC���� 

#include <string>
#include <Windows.h>

#ifdef NEED_WECHATOCR
#include "ocr_protobuf.pb.h"
#endif

namespace qqimpl
{
	//QQLAUNCHINFO�ṹ����������������Ķ�Ӧ��, ʵ�ʵ����лḴ��һ�ݵ�pIMojoIpc, Ҳ��������pIMojoIpc���һ����Ա����
	// {ʵ��λ��Ϊ (*(DWORD*)(((DWORD*)(*(DWORD*)(*(pIMojoIpc + 29)))) + 3))} (32λ��)
	//(�е�λ�ÿ����Ǵ����, ����������ʹ��)
	struct QQLAUNCHINFO
	{
		DWORD Reserved_0;		// + 00	Ӧ�������
		DWORD Reserved_1;		// + 04
		DWORD Reserved_2;		// + 08
		DWORD Reserved_3; 		// + 12 ��������в����õ�
		DWORD Reserved_4;		// + 16 �����в����Ľ�ֹ��ַ
		DWORD* pIMojoIpc;		// + 20 COMָ��
		DWORD launchResult;		// + 24 LaunchChildProcess()����ֵ
		DWORD PlaceHolder_0[5];	// + 28
		std::wstring ExePath;	// + 48 QQScreenShot.exe·�� <- �������+56�ᱻд������ֵ û��������ν
		DWORD Reserved_18;		// + 72
		DWORD PlaceHolder_1[28];// + 76
		DWORD* ProcessClass;	// + 188 Chrome�ڲ�Process��
		DWORD Reserved_48;		// + 192
		DWORD ChildPid;			// + 196 �ӽ���PID
		DWORD PlaceHolder_3[14];// + 200
	};//size = 256

	//QQ��IpcһЩ�����ķ�װ
	class QQIpcParentWrapper {
	public:
		QQIpcParentWrapper();
		~QQIpcParentWrapper();

		/**
		 * @brief Ĭ�ϵ��ӽ��̽�����Ϣ�ĺ���.
		 * @param pArg pIMojoIpc
		 * @param msg IPC_MSG
		 * @param arg3 �ò���
		 * @param arg4 ������int 
		 * @param addition_msg ���addition_msg_size��Ϊ0������addition_msg(ͼƬ·��)
		 * @param addition_msg_size strlen(addition_msg)
		 */
		static void __stdcall OnDefaultReceiveMsg(DWORD* pArg, char* msg, DWORD arg3, DWORD arg4, char* addition_msg, int addition_msg_size);

		/**
		 * @brief ��ʼ������.
		 * @param exe_path �ӽ���EXE·��
		 * @param dll_path parent-ipc-core-x86.dll��·�� Ĭ��Ϊ����Ŀ¼��
		 * @return �ɹ�����true
		 */
		bool Init(std::wstring exe_path, std::wstring dll_path = L"parent-ipc-core-x86.dll");

		/**
		 * @brief �����ӽ���Exe·��.
		 * @param exe_path wstring����·��
		 * @param �ɹ�����true
		 */
		bool SetExePath(std::wstring exe_path);

		/**
		 * @brief ��ȡQQ��ͼ��Ϣ�ṹ��.
		 * @return struct QQLAUNCHINFO*
		 */
		struct QQLAUNCHINFO* GetInfoPtr();

		/**
		 * @brief ����dll�ڲ�����־�ȼ�.
		 * * @param level 0-4 5��ر�
		 */
		void SetLogLevel(int level);

		/**
		 * @brief ��ȡ��һ�εĴ�����Ϣ�ַ���.
		 * @return ������Ϣ 
		 */
		std::wstring GetLastErrStr();

		/**
		 * @brief ��pIMojoIpc->InitParentLog�ķ�װ.
		 * @param level ��־�ȼ�
		 * @param LogCallbackΪ��
		 */
		void InitLog(int level = 0, void* callback = NULL);

		/**
		 * @brief ��pIMojoIpc->InitParentIpc()�ķ�װ.
		 */
		void InitParentIpc();

		/**
		 * @brief ��pIMojoIpc->LaunchChildProcess�ķ�װ.
		 * @param �û��Զ��������Ϣ�ĺ��� ����Ϊ6��������Ϊ__stdcall
		 * @return �ӽ���PID ʧ�ܷ���0
		 */
		UINT LaunchChildProcess(void* callback = NULL);

		/**
		 * @brief ��pIMojoIpc->ConnectedToChildProcess�ķ�װ.
		 * @return �ɹ�����true
		 */
		bool ConnectedToChildProcess();

		/**
		 * @brief ��pIMojoIpc->SendMessageUsingBufferInIPCThread�ķ�װ.
		 * @param command IPC_MSG
		 * @param addition_msg ����
		 * @return �ɹ�����true
		 */
		bool SendIpcMessage(std::string command, std::string addition_msg = "");

		/**
		 * @brief ��pIMojoIpc->TerminateChildProcess�ķ�װ.
		 * @param exit_code ��chrome process_win.cc���Process::Terminate����
		 * @param wait The process may not end immediately due to pending I/O
		 * @return �ɹ�����true
		 */
		bool TerminateChildProcess(int exit_code, bool wait_);

		/**
		 * @brief ��pIMojoIpc->ReLaunchChildProcess�ķ�װ.
		 * @return �ɹ�����true
		 */
		bool ReLaunchChildProcess();


	private:
		HMODULE m_ipc_dll;
		int m_child_pid;
		DWORD* m_ptr_IMojoIpc;
		std::wstring m_exe_path;
		void* m_usr_receive_callback;

		std::wstring _last_err;
	};

	class QQIpcChildWrapper
	{
	public:
		QQIpcChildWrapper();
		~QQIpcChildWrapper();

		/**
		 * @brief ��ȡ��һ�εĴ�����Ϣ�ַ���.
		 * @return ������Ϣ
		 */
		std::wstring GetLastErrStr();

		/**
		 * @brief ��ʼ������.
		 * @param dll_path child(��parent, ��Ϊ������һ����)-ipc.dll��·�� Ĭ��Ϊ����Ŀ¼��
		 * @return �ɹ�����true
		 */
		bool Init(std::wstring dll_path = L"child-ipc-core-x86.dll");

		/**
		 * @brief ��pIMojoIpc->InitChildIpc()�ķ�װ.
		 */
		void InitChildIpc();

		/**
		 * @brief ��pIMojoIpc->InitChildLog�ķ�װ.
		 * @param level ��־�ȼ�
		 * @param LogCallbackΪ��
		 */
		void InitLog(int level = 0, void* callback = NULL);

		/**
		 * @brief ��pIMojoIpc->SetChildReceiveCallbackAndCOMPtr()�ķ�װ.
		 * @param callback ����IPC��Ϣ�ĺ���
		 */
		void SetChildReceiveCallback(void* callback = nullptr);

		/**
		 * @brief ��pIMojoIpc->ChildSendMessageUsingBuffer�ķ�װ.
		 * @param command IPC_MSG
		 * @param addition_msg ����
		 */
		void ChildSendIpcMessage(std::string command, std::string addition_msg = "");

	private:
		HMODULE m_ipc_dll;
		DWORD* m_ptr_IMojoIpc;// +00 ��� +04 ����IPC MSG�ĺ�����ַ +12 ��־�ȼ� +16 &(COMָ��)

		std::wstring _last_err;
	};

#ifdef NEED_WECHATOCR
	namespace qqocrwrapper
	{
		//QQOCRManager�ṹ����������������Ķ�Ӧ��
		struct OCRVTABLE
		{
			DWORD* ThrowException;
			DWORD* OnReadPush;
			DWORD* Reserved_2;
			DWORD* Reserved_3;
			DWORD* OnRemoteConnect;
			DWORD* OnRemoteDisconnect;
			DWORD* OnRemoteProcessLaunched;
			DWORD* OnRemoteProcessLaunchFailed;
			DWORD* OnRemoteMojoError;
		};

		struct QQOCRManager
		{
			OCRVTABLE* vTable;		// + 00 ���
			DWORD Reserved_1;		// + 04
			DWORD Reserved_2;		// + 08
			DWORD isSending;		// + 12	�Ƿ����ڷ���OCR Task (1 = ��) 
			DWORD isEnvInit;		// + 16	�Ƿ��ʼ���˻���(����·����MMMojo) (0 = δ��ʼ�� 1 = ֻ��·�� 2 = ȫ����ʼ��)
			DWORD* MMMojoEnvPtr;	// + 20 mmmojo!CreateMMMOjoEnvironment()
			std::wstring ExePath;	// + 24 TencentOCR.exe·��
			DWORD isArch64;			// + 48 �Ƿ���64λ�ܹ� 32λ�ܹ�������TencentOCR
			DWORD Reserved_13;		// + 52
			DWORD Reserved_14;		// + 56
			DWORD PlaceHolder[11];	// + 60 ռλ��
			std::string PicPath;	// + 104 Ҫʶ���ͼƬ·��
		};//size = 128

		typedef DWORD(__cdecl* LPFN_INITIALIZEMMMOJO)(DWORD, DWORD);
		typedef DWORD(__cdecl* LPFN_SHUTDOWNMMMOJO)();
		typedef DWORD* (__cdecl* LPFN_CREATEMMMOJOENVIRONMENT)();
		typedef DWORD* (__cdecl* LPFN_SETMMMOJOENVIRONMENTCALLBACKS)(DWORD*, int, DWORD*);//arg: ��һ���������� ��������ʱ���ص���ָ��, ���, ����ָ��
		typedef DWORD(__cdecl* LPFN_SETMMMOJOENVIRONMENTINITPARAMS)(DWORD*, DWORD, DWORD);
		typedef void(__cdecl* LPFN_APPENDMMSUBPROCESSSWITCHNATIVE)(DWORD*, const char*, wchar_t*);//arg: "user-lib-dir", ����Ŀ¼
		typedef DWORD(__cdecl* LPFN_STARTMMMOJOENVIRONMENT)(DWORD*);
		typedef DWORD(__cdecl* LPFN_STOPMMMOJOENVIRONMENT)(DWORD*);
		typedef DWORD(__cdecl* LPFN_REMOVEMMMOJOENVIRONMENT)(DWORD*);
		typedef DWORD* (__cdecl* LPFN_GETMMMOJOREADINFOREQUEST)(DWORD*, DWORD*);
		typedef DWORD* (__cdecl* LPFN_GETMMMOJOREADINFOATTACH)(DWORD*, DWORD*);
		typedef DWORD(__cdecl* LPFN_REMOVEMMMOJOREADINFO)(DWORD*);
		typedef DWORD(__cdecl* LPFN_GETMMMOJOREADINFOMETHOD)(DWORD*);
		typedef DWORD(__cdecl* LPFN_GETMMMOJOREADINFOSYNC)(DWORD*);
		typedef DWORD* (__cdecl* LPFN_CREATEMMMOJOWRITEINFO)(DWORD, DWORD, DWORD);
		typedef DWORD* (__cdecl* LPFN_GETMMMOJOWRITEINFOREQUEST)(DWORD*, DWORD);
		typedef DWORD(__cdecl* LPFN_REMOVEMMMOJOWRITEINFO)(DWORD*);
		typedef DWORD* (__cdecl* LPFN_GETMMMOJOWRITEINFOATTACH)(DWORD*, DWORD*);
		typedef DWORD(__cdecl* LPFN_SETMMMOJOWRITEINFOMESSAGEPIPE)(DWORD*, DWORD*);
		typedef DWORD(__cdecl* LPFN_SETMMMOJOWRITEINFORESPONSESYNC)(DWORD*, DWORD*);
		typedef DWORD(__cdecl* LPFN_SENDMMMOJOWRITEINFO)(DWORD*, DWORD*);
		typedef DWORD(__cdecl* LPFN_SWAPMMMOJOWRITEINFOCALLBACK)(DWORD*, DWORD*);
		typedef DWORD(__cdecl* LPFN_SWAPMMMOJOWRITEINFOMESSAGE)(DWORD*, DWORD*);

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

		/**
		 * @brief ��ʼ��OCRManager.
		 * @param exe_path TencentOCR.exe·��
		 * @param dll_path mmmojo,dll��·�� Ĭ��Ϊ����Ŀ¼��
		 * @param need_timer �Ƿ����ö�ʱ������OCR����
		 */
		void InitManager(std::wstring exe_path = L"", std::wstring dll_path = L"mmmojo.dll", bool need_timer = true);

		/**
		 * @brief ɾ��OCRManager.
		 */
		void UnInitManager();

		/**
		 * @brief ������ѶOCR Exe·��.
		 * @param exe_path wstring����·��
		 * @param �ɹ�����true
		 */
		bool SetOcrExePath(std::wstring exe_path);

		/**
		 * @brief ������ѶOCR��--user-lib-dir������·��.
		 * @param usr_lib_path Ҳ����mmmojo_64.dll���ڵ�·��
		 */
		bool SetOcrUsrLibPath(std::wstring usr_lib_path);

		/**
		 * @brief ��ȡ��һ�εĴ�����Ϣ�ַ���.
		 * @return ������Ϣ
		 */
		std::wstring GetLastErrWStr();

		/**
		 * @brief ��װ.
		 * @param pic_path string����
		 */
		bool DoOCRTask(std::string pic_path);

		/**
		 * @brief ��ʼ��MMMojo����.
		 * @return �ɹ�����true
		 */
		bool OCRdoInit();

		/**
		 * @brief ж��OCR����.
		 */
		void OCRdoUnInit();

		/**
		 * @brief ����OCR Task.
		 * @param task_id �������
		 * @param pic_path Ҫʶ���ͼƬ·��
		 * @return �ɹ�����true
		 */
		bool SendOCRTask(long long task_id, std::string pic_path);

		typedef void(__stdcall* LPFN_ONUSRREADPUSH)(ocr_protobuf::OcrResponse ocr_response);

		/**
		 * @brief �����û���ȡOCR����Ļص�����.
		 * @param callback ��OcrResponse��Ϊ����
		 */
		void SetUsrReadPushCallbck(LPFN_ONUSRREADPUSH callback);

		/**
		 * @brief �Ƿ������Disconnect�ص�����.
		 * @return �����˷���true
		 */
		bool HasDisconnectSignal();
	}
#endif
};
