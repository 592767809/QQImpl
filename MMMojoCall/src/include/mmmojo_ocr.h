/*****************************************************************//**
 * @file   mmmojo_ocr.h
 * @brief  �ڵ���XPlugin�Ļ����϶Ե���WeChatOCR�ķ������з�װ
 * 
 * @author 0xEEEE
 * @date   2023.10.29
 *********************************************************************/
#pragma once

#include "mojo_call_export.h"
#include "mmmojo_call.h"

#include "ocr_protobuf.pb.h"

#include <mutex>
#include <Windows.h>

namespace qqimpl
{
namespace mmmojocall
{
	class MMMOJOCALL_API OCRManager : public XPluginManager
	{
	public:
		OCRManager();
		~OCRManager();

		/**
		 * @brief ����--user-lib-dir�����в���.
		 * @param usr_lib_dir ��΢��mmmojo(_64).dll����Ŀ¼
		 * @return �ɹ�����true 
		 */
		bool SetUsrLibDir(const char* usr_lib_dir);

		/**
		 * @brief ���Ÿ��û���ReadOnPush�ص������Ľӿ�.
		 * @param pic_path ͼƬ·��
		 * @param serialized_data ���л��������
		 * @param data_size ���ݴ�С
		 */
		typedef void (*LPFN_OCRREADONPUSH)(const char* pic_path, const void* serialized_data, int data_size);

		/**
		 * @brief ���ý��ս���Ļص�����.
		 * @param pfunc ӦΪLPFN_OCRREADONPUSH����
		 * @return �ɹ�����true 
		 */
		void SetReadOnPush(LPFN_OCRREADONPUSH pfunc);

		/**
		 * @brief ����MMMojo�����Լ�WeChatOCR.exe����.
		 * @return �ɹ�����true
		 */
		bool StartWeChatOCR();

		/**
		 * @brief ����MMMojo�����Լ�WeChatOCR.exe����. 
		 */
		void KillWeChatOCR();

		/**
		 * @brief ����һ��OCR����.
		 * @param pic_path ͼƬ·��
		 * @return �ɹ�����true 
		 */
		bool DoOCRTask(const char* pic_path);

		/**
		 * @brief ��������״̬ [ֻ��Ϊtrue���ܵ���DoOCRTask ����Ҫ�ֶ�����].
		 * @param trueΪ������
		 */
		void SetConnectState(bool connect);

		/**
		 * @brief �����û����õĻص�����.
		 * @param requst_id RequestIdOCR
		 * @param serialized_data pb���л��������
		 * @param data_size pb���ݵĴ�С
		 */
		void CallUsrCallback(int request_id, const void* serialized_data, int data_size);

	private:
		bool SendOCRTask(uint32_t task_id, std::string pic_path);

		int GetIdleTaskId();
		bool SetTaskIdIdle(int id);

	private:
		#define OCR_MAX_TASK_ID 32			//���ͬʱ��������ID, Ĭ��32, һ��Ҫ<=32
		BYTE m_task_id[OCR_MAX_TASK_ID];
		std::map<int, std::string> m_id_path;
		std::mutex m_task_mutex;			//���ڻ����ȡTASK ID
		std::mutex m_connect_mutex;			//mutex��condition_variable����ֻ���ڻص���RemoteConnectҲ����Connect�Ϻ��ٷ���OCR����
		std::condition_variable m_connect_con_var;
		std::string m_usr_lib_dir;
		bool m_wechatocr_running;			//WeChatOCR.exe�Ƿ���������
		bool m_is_arch64;					//�Ƿ���64λϵͳ
		bool m_connect_state;
		LPFN_OCRREADONPUSH m_usr_callback;	//�û����õĻ�ȡOCR����Ļص�����
	};
}
}
