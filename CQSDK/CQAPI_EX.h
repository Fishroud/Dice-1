#pragma once

#include "cqdefine.h"

#include <string>
#include <map>
#include <vector>

class Unpack;

namespace CQ
{
	//����������־ 
	int addLog(int Priorty, const char* Type, const char* Content) noexcept;

	//���ͺ�����Ϣ
	//Auth=106 ʧ�ܷ��ظ�ֵ,�ɹ�������ϢID 
	int sendPrivateMsg(long long QQ, const char* msg) noexcept;
	//���ͺ�����Ϣ
	//Auth=106 ʧ�ܷ��ظ�ֵ,�ɹ�������ϢID 
	int sendPrivateMsg(long long QQ, const std::string& msg) noexcept;

	//����Ⱥ��Ϣ 
	//Auth=101 ʧ�ܷ��ظ�ֵ,�ɹ�������ϢID
	int sendGroupMsg(long long GroupID, const char* msg) noexcept;
	//����Ⱥ��Ϣ 
	//Auth=101 ʧ�ܷ��ظ�ֵ,�ɹ�������ϢID
	int sendGroupMsg(long long GroupID, const std::string& msg) noexcept;


	//������������Ϣ 
	//Auth=103 ʧ�ܷ��ظ�ֵ,�ɹ�������ϢID
	int sendDiscussMsg(long long DiscussID, const char* msg) noexcept;
	//������������Ϣ 
	//Auth=103 ʧ�ܷ��ظ�ֵ,�ɹ�������ϢID
	int sendDiscussMsg(long long DiscussID, const std::string& msg) noexcept;

	//������ Auth=110
	int sendLike(long long QQID, int times) noexcept;

	//ȡCookies (���ã��˽ӿ���Ҫ�ϸ���Ȩ) 
	//Auth=20
	const char* getCookies(const char* Domain) noexcept;

	//ȡCookies (���ã��˽ӿ���Ҫ�ϸ���Ȩ) 
	//Auth=20
	const char* getCookies(const std::string& Domain) noexcept;

	//�������� 
	const char* getRecord(
		const char* file, // �յ���Ϣ�е������ļ��� (file) 
		const char* outformat // Ӧ������ĸ�ʽ mp3,amr,wma,m4a,spx,ogg,wav,flac
	) noexcept;
	//�������� 
	std::string getRecord(
		const std::string& file, // �յ���Ϣ�е������ļ��� (file) 
		const std::string& outformat // Ӧ������ĸ�ʽ mp3,amr,wma,m4a,spx,ogg,wav,flac
	) noexcept;

	//ȡCsrfToken (���ã��˽ӿ���Ҫ�ϸ���Ȩ) 
	//Auth=20 ��QQ��ҳ�õ���bkn/g_tk��
	int getCsrfToken() noexcept;

	//ȡӦ��Ŀ¼ 
	//���ص�·��ĩβ��"\" 
	const char* getAppDirectory() noexcept;

	//ȡ��¼QQ 
	long long getLoginQQ() noexcept;

	//ȡ��¼�ǳ� 
	const char* getLoginNick() noexcept;

	//��ȺԱ�Ƴ� Auth=120 
	int setGroupKick(
		long long GroupID, long long QQID,
		CQBOOL RefuseForever = false // ���Ϊ�棬�򡰲��ٽ��մ��˼�Ⱥ���롱�������� 
	) noexcept;

	//��ȺԱ���� Auth=121 
	int setGroupBan(
		long long GroupID, long long QQID,
		long long Time = 60 // ���Ե�ʱ�䣬��λΪ�롣���Ҫ�����������д0 
	) noexcept;

	//��Ⱥ����Ա Auth=122 
	int setGroupAdmin(
		long long GroupID, long long QQID,
		CQBOOL isAdmin = true // ��/���ù���Ա ��/ȡ������Ա 
	) noexcept;

	//��Ⱥ��Աר��ͷ�� Auth=128 ��Ⱥ��Ȩ�� 
	int setGroupSpecialTitle(
		long long GroupID, long long QQID,
		const char* Title, // ���Ҫɾ����������� 
		long long ExpireTime = -1 // ר��ͷ����Ч�ڣ���λΪ�롣���������Ч��������д-1
	) noexcept;
	//��Ⱥ��Աר��ͷ�� Auth=128 ��Ⱥ��Ȩ�� 
	int setGroupSpecialTitle(
		long long GroupID, long long QQID,
		const std::string& Title, // ���Ҫɾ����������� 
		long long ExpireTime = -1 // ר��ͷ����Ч�ڣ���λΪ�롣���������Ч��������д-1
	) noexcept;

	//��ȫȺ���� Auth=123 
	int setGroupWholeBan(
		long long GroupID,
		CQBOOL isBan = true // ��/���� ��/�ر� 
	) noexcept;

	//������ȺԱ���� Auth=124 
	int setGroupAnonymousBan(
		long long GroupID,
		const char* AnonymousToken, // Ⱥ��Ϣ�¼��յ��ġ����������� 
		long long banTime = 60 // ���Ե�ʱ�䣬��λΪ�롣��֧�ֽ�� 
	) noexcept;

	//��Ⱥ�������� Auth=125 
	int setGroupAnonymous(long long GroupID, CQBOOL enableAnonymous = true) noexcept;

	//��Ⱥ��Ա��Ƭ Auth=126 
	int setGroupCard(long long GroupID, long long QQID, const char* newGroupNick) noexcept;

	//��Ⱥ��Ա��Ƭ Auth=126 
	int setGroupCard(long long GroupID, long long QQID, const std::string& newGroupNick) noexcept;

	//��Ⱥ�˳� Auth=127 ����,�˽ӿ���Ҫ�ϸ���Ȩ 
	int setGroupLeave(
		long long GroupID,
		CQBOOL isDismiss = false // ��/��ɢ��Ⱥ (Ⱥ��) ��/�˳���Ⱥ (����Ⱥ��Ա) 
	) noexcept;

	//���������˳� Auth=140 
	int setDiscussLeave(
		long long DiscussID
	) noexcept;

	//�ú���������� Auth=150 
	int setFriendAddRequest(
		const char* RequestToken, // �����¼��յ��ġ�������ʶ������ 
		int ReturnType, // #����_ͨ�� �� #����_�ܾ� 
		const char* Remarks // ��Ӻ�ĺ��ѱ�ע 
	) noexcept;

	//��Ⱥ������� Auth=151 
	int setGroupAddRequest(
		const char* RequestToken, // �����¼��յ��ġ�������ʶ������ 
		int RequestType, // ���������¼������������� #����_Ⱥ��� �� #����_Ⱥ���� 
		int ReturnType, // #����_ͨ�� �� #����_�ܾ� 
		const char* Reason // �������ɣ��� #����_Ⱥ��� �� #����_�ܾ� ʱ����
	) noexcept;

	//������������ʾ,��ʱ��֪����ʲô�õ�
	int setFatal(const char* ErrorMsg) noexcept;


	class GroupMemberInfo;
	//ȡȺ��Ա��Ϣ (֧�ֻ���) Auth=130 
	GroupMemberInfo getGroupMemberInfo(long long GroupID, long long QQID, CQBOOL DisableCache = false) noexcept;

	class StrangerInfo;
	//ȡİ������Ϣ (֧�ֻ���) Auth=131 
	StrangerInfo getStrangerInfo(long long QQID, CQBOOL DisableCache = false) noexcept;

	//ȡȺ��Ա�б� Auth=160  
	std::vector<GroupMemberInfo> getGroupMemberList(long long GroupID);

	//ȡȺ�б� Auth=161  
	std::map<long long, std::string> getGroupList(bool disableCache = false);

	class FriendInfo;
	//ȡ�����б� Auth=162  
	std::map<long long, FriendInfo> getFriendList(bool disableCache = false);

	//�Ƿ�֧�ַ���ͼƬ������trueΪ֧�֣�����falseΪ��֧��
	bool canSendImage() noexcept;

	//�Ƿ�֧�ַ������������ش��� 0 Ϊ֧�֣����� 0 Ϊ��֧��
	bool canSendRecord() noexcept;

	//����ͼƬ��������ͼƬ�ļ�����·��
	const char* getImage(const char* file) noexcept;

	//����ͼƬ��������ͼƬ�ļ�����·��
	const char* getImage(const std::string& file) noexcept;

	//������Ϣ Auth=180
	int deleteMsg(long long MsgId) noexcept;

	const char* getlasterrmsg() noexcept;
	// Ⱥ��Ϣ
	class GroupInfo final {
		void setdata(Unpack& u);
	public:
		long long llGroup{};
		std::string strGroupName{};
		int nGroupSize = 0; // Ⱥ����
		int nMaxGroupSize = 0;//Ⱥ��ģ
		int nFriendCnt = 0;//������

		explicit GroupInfo(long long group);
		GroupInfo() = default;
		[[nodiscard]] std::string tostring() const;
	};
	// Ⱥ��Ա��Ϣ
	class GroupMemberInfo final
	{
		void setdata(Unpack& u);
	public:
		long long Group{};
		long long QQID{};
		std::string Nick{};
		std::string GroupNick{};
		int Gender{}; // 0/���� 1/Ů��
		int Age{};
		std::string Region{};
		int AddGroupTime{};
		int LastMsgTime{};
		std::string LevelName{};
		int permissions{}; //1/��Ա 2/����Ա 3/Ⱥ��
		std::string Title{};
		int ExpireTime{}; // -1 ��������
		CQBOOL NaughtyRecord{};
		CQBOOL canEditGroupNick{};

		explicit GroupMemberInfo(Unpack& msg);
		explicit GroupMemberInfo(const char* msg); //��API����
		explicit GroupMemberInfo(const std::vector<unsigned char>& data); //��Unpack����
		GroupMemberInfo() = default;

		[[nodiscard]] std::string tostring() const;
	};
	// ������Ϣ
	class FriendInfo final
	{
	public:
		long long QQID = 0;
		std::string nick; //�ǳ�
		std::string remark; //��ע

		explicit FriendInfo(Unpack p);
		FriendInfo() = default;
		[[nodiscard]] std::string tostring() const;
	};
	// İ������Ϣ
	class StrangerInfo final
	{
	public:
		long long QQID = 0;
		std::string nick = ""; //�ǳ�
		int sex = 255; //0/���� 1/Ů�� 255/δ֪
		int age = -1; //����

		explicit StrangerInfo(const char* msg);
		StrangerInfo() = default;

		[[nodiscard]] std::string tostring() const;
	};
}
