/*****************************************************************//**
 * @file   mmmojo_player.h
 * @brief  �ڵ���XPlugin�Ļ����϶Ե���WeChatPlayer�ķ������з�װ
 * 
 * @author 0xEEEE
 * @date   2023.11.17
 *********************************************************************/
#pragma once

#include "mojo_call_export.h"
#include "mmmojo_call.h"

#include "player_protobuf.pb.h"

#include <mutex>
#include <Windows.h>

namespace qqimpl
{
namespace mmmojocall
{
	//WeChatPlayer�Ὣ��Ƶ����������(Ҳ�п�����ע��)����ʼ��ʱ�趨�Ĵ��ھ����

	//WeChatPlayerͨ�Ź���:
	//WeChatPlayer����ʱ����WeChat����PlayerHiPush��Ϣ, WeChat���յ�PlayerHiPush��Ϣ������PlayerInitPullReq, ֮��WeChatPlayer����PlayerInitPullResp, ����������ʼ���������
	//��Ҫ������Ƶʱ:
	//WeChat��Player����PlayerCreatePlayerCorePullReq, ���󴴽�һ��PlayerCore, Player�ظ�PlayerCreatePlayerCorePullResp, ָ���Ƿ񴴽��ɹ�
	//֮��WeChat����PlayerInitPlayerCorePush��ʼ���������ں�, ��������Ƶ·��,Ҫע��Ĵ��ڵ�һЩ��Ϣ, Player���յ�InitCore��Ϣ���ظ�PlayerVideoSizeChangedPush��PlayerPrepareAsyncPush��Ϣ, 
	//����PlayerVideoSizeChangedPush��������Ƶ�Ŀ��, PlayerPrepareAsyncPush��������Ƶ��ʱ��, �����յ�PlayerPrepareAsyncPush��Ϣʱ��˵����Ƶ׼������
	//֮��WeChat����PlayerStartPush����ʼ������Ƶ
	//֮��WeChat��һֱ����PlayerGetCurrentPositionMsPullReq����ȡ��Ƶ��ǰ�Ĳ��Ž���


	//���·�װ���첽������ʱ��Ϊ��ͬ������
	class MMMOJOCALL_API PlayerManager : public XPluginManager
	{
	public:
		struct CoreStatus
		{
			bool create_success;	//�Ƿ񴴽��ɹ�
			int duration;			//��Ƶʱ��
			int org_width;			//��Ƶԭʼ���
			int org_height;			//��Ƶԭʼ�߶�
			int cur_pos;			//��Ƶ��ǰ���ŵ���λ��
			int state;				//4��ʾ���ڲ��� 5��ʾ��ͣ���� 8��ʾֹͣ����
		};

	public:
		PlayerManager();
		~PlayerManager();

		/**
		 * @brief �����������������в���.
		 * @param usr_lib_dir (--user-lib-path) ��΢��mmmojo(_64).dll����Ŀ¼
		 * @param xlog_path (--xlog_path) ���Բ����� ΢��ΪC:\Users\{YourName}\AppData\Roaming\Tencent\WeChat\log\player
		 * @param xlog_prefix (--xlog_prefix) ���Բ����� ΢��Ϊplayer
		 */
		bool SetSwitchArgs(const char* usr_lib_dir, const char* xlog_path = NULL, const char* xlog_prefix = NULL);

		/**
		 * @brief ����MMMojo�����Լ�WeChatPlayer.exe����.
		 * @return �ɹ�����true
		 */
		bool StartWeChatPlayer();

		/**
		 * @brief ����MMMojo�����Լ�WeChatPlayer.exe����.
		 */
		void KillWeChatPlayer();

		/**
		 * @brief ��������״̬ [����Ҫ�ֶ�����].
		 * @param trueΪ������
		 */
		void SetConnectState(bool connect);

		/**
		 * @brief ��ȡ����״̬
		 * @return �Ƿ�������
		 */
		bool GetConnectState();

		//����ΪPlayerCore�ķ�װ

		/**
		 * @brief ����һ���������ں�
		 * @return �ں�ID ����ʧ�ܷ���0
		 */
		int CreatePlayerCore();

		/**
		 * @brief ��ȡ�Ѵ����Ĳ����ں�����.
		 * @return �����ں�����
		 */
		int GetPlayerCoreNum();

		/**
		 * @brief ��ʼ�������ں�.
		 * @param palyer_id �������ں�ID
		 * @param window_surface Ҫ�����Ĵ��ھ��
		 * @param file_name ��Ƶ�ļ���
		 * @param file_path Ҫ���ŵ���Ƶ·��
		 * @param file_size ������Ƶ�Ĵ�С ����ʱΪ0
		 * @param is_local ��Ƶ�Ƿ����Ա����ļ� (falseΪ��������)
		 * @param is_mute �Ƿ���
		 * @param volume ����
		 * @param is_repeate �Ƿ��ظ�����
		 * @param speed ��������
		 * @return ��Ƶʱ�� ʧ�ܷ���0
		 */
		int InitPlayerCore(int player_id, HWND window_surface, const char* file_name, const char* file_path, int64_t file_size = 0, bool is_local = true, bool is_mute = false, float volume = 1.0, bool is_repeat = true, float speed = 1.0);

		/**
		 * @brief �ı�PlayerCore�Ĵ�С.
		 * @param player_id �ں�ID
		 * @param width �¿��
		 * @param height �¸߶�
		 * @return �Ƿ�����ɹ�
		 */
		bool ResziePlayerCore(int player_id, int width, int height);

		/**
		 * @brief ��ʼ������Ƶ.
		 * @param �ں�ID
		 * @return �����Ƿ�ɹ�
		 */
		bool StartVideo(int player_id);

		/**
		 * @brief ��ͣ������Ƶ.
		 * @param �ں�ID
		 * @return �����Ƿ�ɹ�
		 */
		bool PauseVideo(int player_id);

		/**
		 * @brief �ָ�������Ƶ.
		 * @param �ں�ID
		 * @return �����Ƿ�ɹ�
		 */
		bool ResumeVideo(int player_id);

		/**
		 * @brief ֹͣ������Ƶ.
		 * @param �ں�ID
		 * @return �����Ƿ�ɹ�
		 */
		bool StopVideo(int player_id);

		/**
		 * @brief ����/ȡ������.
		 * @param player_id �ں�ID
		 * @param is_mute �Ƿ���
		 * @return �����Ƿ�ɹ�
		 */
		bool MuteVideo(int player_id, bool is_mute = true);

		/**
		 * @brief ��ת���Ž���.
		 * @param player_id �ں�ID
		 * @param pos_ms ����
		 * @return �����Ƿ�ɹ�
		 */
		bool SeekToVideo(int player_id, int pos_ms);

		/**
		 * @brief ����ע��Ĵ���.
		 * @param player_id �ں�ID
		 * @param window_hwnd Ҫע��Ĵ��ھ��
		 * @return �����Ƿ�ɹ�
		 */
		bool SetSurfaceVideo(int player_id, HWND window_hwnd);

		/**
		 * @brief ������Ƶ����.
		 * @param player_id �ں�ID
		 * @param volume ������С
		 * @return �����Ƿ�ɹ�
		 */
		bool SetVolumeVideo(int player_id, float volume);

		/**
		 * @brief �Ƿ��ظ�������Ƶ.
		 * @param player_id �ں�ID
		 * @param is_repeat �Ƿ��ظ�����
		 * @return �����Ƿ�ɹ�
		 */
		bool RepeatVideo(int player_id, bool is_repeat = true);

		/**
		 * @brief ���ò�������.
		 * @param player_id �ں�ID
		 * @param speed_ratio ��������
		 * @return �����Ƿ�ɹ�
		 */
		bool SetSpeedVideo(int player_id, float speed_ratio);

		/**
		 * @brief ��ȡ��ǰ��Ƶ���Ž���.
		 * @param player_id �ں�ID
		 * @return 
		 */
		int GetCurrentPosition(int player_id);

		/**
		 * @brief ���ٲ������ں�.
		 * @param �ں�ID
		 */
		void DestroyPlayerCore(int player_id);

		CoreStatus* GetPlayerCoreStatus(int player_id);

		//��������ͬ������
		void WaitEvent(int player_id, int which_one);
		void RunEvent(int player_id, int which_one);

	private:
		int GetIdlePlayerCoreId();
		bool SetPlayerCoreIdIdle(int id);

		void CreateCoreStatusSync(int player_id);
		void DeleteCoreStatusSync(int player_id);

	private:
		bool m_wechatplayer_running;
		bool m_connect_state;				//�Ƿ�������

		#define MAX_PLAYER_CORE_ID 256
		std::mutex m_player_core_mutex;					//���ڻ����ȡPlayerCore ID
		BYTE m_player_core_id[MAX_PLAYER_CORE_ID];		//���е�PlayerCoreID

		std::map<int, CoreStatus> m_core_status;		//�������ں�״̬
		
		struct SyncEvent
		{
			HANDLE hEventCreateCore;
			HANDLE hEventInitCore;
			HANDLE hEventGetPos;
		};
		std::map<int, SyncEvent> m_core_sync;	//����ͬ���ں˲���
	};



}
}