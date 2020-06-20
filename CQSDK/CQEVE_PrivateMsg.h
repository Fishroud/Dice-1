#pragma once
#include "CQMsgSend.h"
#include "CQEVEMsg.h"

/*
˽����Ϣ(Type=21)

�˺����������²���
subType		�����ͣ�11/���Ժ��� 1/��������״̬ 2/����Ⱥ 3/����������
msgId	��ϢID
fromQQ		��ԴQQ
msg			��Ϣ����
font		����

���ӳ�����ڿ�Q���̡߳��б����ã���ע��ʹ�ö������Ҫ��ʼ��(CoInitialize,CoUninitialize)
�������ʹ���»��߿�ͷ��Ҫ�ĳ�˫�»���
���ط���ֵ,��Ϣ��������,������Ȳ�������
*/
#define EVE_PrivateMsg_EX(Name)																	\
	void Name(CQ::EVEPrivateMsg & eve);															\
	EVE_PrivateMsg(Name)																		\
	{																							\
		CQ::EVEPrivateMsg tep(subType, msgId, fromQQ, msg, font); \
		Name(tep); \
		return tep._EVEret;																		\
	}																							\
	void Name(CQ::EVEPrivateMsg & eve)


namespace CQ
{
	struct EVEPrivateMsg final : EVEMsg
	{
		EVEPrivateMsg(int subType, int msgId, long long fromQQ, const char* msg, int font) noexcept;

		//���Ժ���
		[[nodiscard]] bool fromPrivate() const noexcept;

		//��������״̬
		[[nodiscard]] bool fromOnlineStatus() const noexcept;

		//����Ⱥ��ʱ
		[[nodiscard]] bool fromGroup() const noexcept;

		//������������ʱ
		[[nodiscard]] bool fromDiscuss() const noexcept;

		// ͨ�� EVEMsg �̳�
		[[nodiscard]] msg sendMsg() const noexcept override;

		int sendMsg(const char*) const noexcept override;

		[[nodiscard]] int sendMsg(const std::string&) const noexcept override;
	};
}
