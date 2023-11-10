/*****************************************************************//**
 * @file   mmmojo_utility.h
 * @brief  �ڵ���XPlugin�Ļ����϶Ե���WeChatUtility�ķ������з�װ
 * 
 * @author 0xEEEE
 * @date   2023.10.29
 *********************************************************************/
#pragma once

#include "mojo_call_export.h"
#include "mmmojo_call.h"

#include "utility_protobuf.pb.h"

namespace qqimpl
{
namespace mmmojocall
{
	class MMMOJOCALL_API UtilityManager : public XPluginManager
	{
	public:
		UtilityManager();
		~UtilityManager();

		/**
		 * @brief ����--user-lib-dir�����в���.
		 * @param usr_lib_dir ��΢��mmmojo(_64).dll����Ŀ¼
		 * @return �ɹ�����true
		 */
		bool SetUsrLibDir(const char* usr_lib_dir);

		/**
		 * @brief ���Ÿ��û���ReadOnPull�ص������Ľӿ�.
		 * @param type ��requst_id��ֵһ��
		 * @param serialized_data ���л��������
		 * @param data_size ���ݴ�С
		 */
		typedef void (*LPFN_UTILITYREADONPUSHLL)(int type, const void* data, int data_size);

		/**
		 * @brief ���ý���Pull����Ļص����� (QRScan�Ľ����ͨ��Pull����).
		 * @param pfunc ӦΪLPFN_UTILITYREADONPUSHLL����
		 * @return �ɹ�����true
		 */
		void SetReadOnPull(LPFN_UTILITYREADONPUSHLL pfunc);

		/**
		 * @brief ���ý���Pull����Ļص����� (TextScan�Ľ����ͨ��Push����).
		 * @param pfunc ӦΪLPFN_UTILITYREADONPUSHLL����
		 * @return �ɹ�����true
		 */
		void SetReadOnPush(LPFN_UTILITYREADONPUSHLL pfunc);

		/**
		 * @brief ����MMMojo�����Լ�WeChatUtility.exe����.
		 * @return �ɹ�����true
		 */
		bool StartWeChatUtility();

		/**
		 * @brief ����MMMojo�����Լ�WeChatUtility.exe����.
		 */
		void KillWeChatUtility();

		/**
		 * @brief ����һ�ζ�ά��ʶ������ (�ڽ�ͼ�п�ѡʱ���͵�����) [��δʵ��].
		 * @param chunk ĳ�ֱ����ʽ ��δ���
		 * @return �ɹ�����true
		 */
		bool DoQRScan(void* chunk);

		/**
		 * @brief ����һ�ζ�ά��ʶ������ (��΢���д�ͼƬʱ���͵�����).
		 * @param pic_path ͼƬ·��
		 * @param text_scan_id TextScan������ID
		 * @return �ɹ�����true
		 */
		bool DoPicQRScan(const char* pic_path, int text_scan_id = 1);
		
		/**
		 * @brief ����һ���ز������� [��δʵ��].
		 * @param origin_encode_path ΢�ż��ܺ��ͼƬ·��
		 * @param decode_pic_path ������ͼƬ·��
		 * @param pic_x ͼƬ��
		 * @param pic_y ͼƬ��
		 * @return �ɹ�����true
		 */
		bool DoResampleImage(std::string origin_encode_path, std::string decode_pic_path, int pic_x, int pic_y);

		/**
		 * @brief [��δ���].
		 * @param pic_path
		 * @return 
		 */
		bool DoDecodeImage(std::string pic_path);

		/**
		 * @brief ��������״̬ [ֻ��Ϊtrue���ܵ���DoPicQRScan ����Ҫ�ֶ�����].
		 * @param trueΪ������
		 */
		void SetConnectState(bool connect);

		/*
		 * @brief ��ȡ����״̬
		 * @return �Ƿ������� 
		 */
		bool GetConnectState();

		/**
		 * @brief ���ô����û��ص���������������(protobuf/json)
		 * @param use_json ���Ϊtrue�򴫵�json�ַ���, Ϊfalse�򴫵����л����pb����������
		 */
		void SetCallbackDataMode(bool use_json);

		/**
		 * @brief �����û����õĻص�����.
		 * @param request_id RequstIdUtility
		 * @param serialized_data pb���л��������
		 * @param data_size pb���ݵĴ�С
		 */
		void CallUsrCallback(int request_id, const void* serialized_data, int data_size, std::string pull_or_push = "pull");

	private:
		std::mutex m_connect_mutex;				//mutex��condition_variable����ֻ���ڻص���RemoteConnectҲ����Connect�Ϻ��ٷ�������
		std::condition_variable m_connect_con_var;
		std::string m_usr_lib_dir;
		bool m_wechatutility_running;			//WeChatUtility.exe�Ƿ���������
		bool m_connect_state;
		bool m_cb_data_use_json;
		LPFN_UTILITYREADONPUSHLL m_usr_cb_pull;	//�û����õĻ�ȡUtility Pull����Ļص�����
		LPFN_UTILITYREADONPUSHLL m_usr_cb_push;
	};




}
}
