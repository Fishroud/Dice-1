#pragma once
#include "DiceJob.h"
#include "DiceConsole.h"
#include <TlHelp32.h>
#include <Psapi.h>
#include "StrExtern.hpp"
#include "CQAPI.h"
#include "ManagerSystem.h"
#include "DiceCloud.h"
#include "BlackListManager.h"
#include "GlobalVar.h"
#include "CardDeck.h"
#include "DiceNetwork.h"
#include "Jsonio.h"
#include "S3PutObject.h"

#pragma warning(disable:28159)

using namespace std;
using namespace CQ;

int sendSelf(const string msg) {
	static long long selfQQ = CQ::getLoginQQ();
	return CQ::sendPrivateMsg(selfQQ, msg);
}

void cq_exit(DiceJob& job) {
	int pid = _getpid();
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		job.note("����ʧ�ܣ����̿��մ���ʧ�ܣ�", 1);
	}
	BOOL bResult = Process32First(hProcessSnap, &pe32);
	int ppid(0);
	while (bResult) {
		if (pe32.th32ProcessID == pid) {
			ppid = pe32.th32ParentProcessID;
			break;
		}
		bResult = Process32Next(hProcessSnap, &pe32);
	}
	if (!ppid) {
		job.note("����ʧ�ܣ�δ�ҵ����̣�", 1);
	}
	string strCMD("taskkill /f /pid " + to_string(ppid));
	job.note("������" + getMsg("self") + "��5�����ɱ", 1);
	std::this_thread::sleep_for(5s);
	job.echo(strCMD);
	Enabled = false;
	dataBackUp();
	system(strCMD.c_str());
}

inline PROCESSENTRY32 getProcess(int pid) {
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hParentProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	Process32First(hParentProcess, &pe32);
	return pe32;
}
void cq_restart(DiceJob& job) {
	//string strSelfPath;
	string strSelfName;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		job.note("����ʧ�ܣ����̿��մ���ʧ�ܣ�", 1);
	}
	BOOL bResult = Process32First(hProcessSnap, &pe32);
	int ppid(0);
	if (Mirai) {
		char buffer[MAX_PATH];
		const DWORD length = GetModuleFileNameA(nullptr, buffer, sizeof buffer);
		std::string pathSelf(buffer, length);
		pathSelf = pathSelf.substr(0, pathSelf.find("jre\\bin\\java.exe")) + "MiraiOK.exe";
		char pathFull[MAX_PATH];
		while (bResult) {
			if (strcmp(pe32.szExeFile, "MiraiOK.exe") == 0) {
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
				GetModuleFileNameEx(hProcess, NULL, pathFull, sizeof(pathFull));
				if (pathSelf != pathFull)continue;
				ppid = pe32.th32ProcessID;
				job.echo("ȷ�Ͻ���" + pathSelf + "\n����id:" + to_string(ppid));
				break;
			}
			bResult = Process32Next(hProcessSnap, &pe32);
		}
	}
	else {
		int pid = _getpid();
		while (bResult) {
			if (pe32.th32ProcessID == pid) {
				ppid = pe32.th32ParentProcessID;
				PROCESSENTRY32 pp32 = getProcess(ppid);
				strSelfName = pp32.szExeFile;
				job.echo("ȷ�Ͻ���" + strSelfName + "\n������id:" + to_string(pe32.th32ProcessID) + "\n������id:" + to_string(ppid));
				break;
			}
			bResult = Process32Next(hProcessSnap, &pe32);
		}
	}
	if (!ppid) {
		job.note("����ʧ�ܣ�δ�ҵ����̣�", 1);
		return;
	}
	string command = "taskkill /f /pid " + to_string(ppid) + "\nstart .\\" + strSelfName + " /account " + to_string(console.DiceMaid);
	if (Mirai) command = "taskkill /f /pid " + to_string(ppid) + " /t\nstart " + dirExe + "MiraiOK.exe";
	ofstream fout("reload.bat");
	fout << command << std::endl;
	fout.close();
	job.note(command, 0);
	std::this_thread::sleep_for(3s);
	Enabled = false;
	dataBackUp();
	std::this_thread::sleep_for(3s);
	switch (UINT res = -1; res = WinExec(".\\reload.bat", SW_SHOW)) {
	case 0:
		job.note("����ʧ�ܣ��ڴ����Դ�Ѻľ���", 1);
		break;
	case ERROR_FILE_NOT_FOUND:
		job.note("����ʧ�ܣ�ָ�����ļ�δ�ҵ���", 1);
		break;
	case ERROR_PATH_NOT_FOUND:
		job.note("����ʧ�ܣ�ָ����·��δ�ҵ���", 1);
		break;
	default:
		if (res > 31)job.note("�����ɹ���", 0);
		else job.note("����ʧ�ܣ�δ֪����" + to_string(res), 0);
		break;
	}
}

void mirai_reload(DiceJob& job){
	using cq_reload_type = int(__stdcall*)(int32_t);
	HMODULE hModule = LoadLibraryA("CQP.dll");
	cq_reload_type cq_reload = (cq_reload_type)GetProcAddress(hModule, "CQ_reload");
	if (!cq_reload) {
		job.note("����MiraiNativeʧ�ܡ�\nʹ���˹��ɻ������CQP.dll\n�뱣֤��������汾��MiraiNative��ɾ����CQP.dll", 0b10);
		return;
	}
	cq_reload(getAuthCode());
	FreeLibrary(hModule);
}

void auto_save(DiceJob& job) {
	if (sch.is_job_cold("autosave"))return;
	dataBackUp();
	console.log(GlobalMsg["strSelfName"] + "���Զ�����", 0, printSTNow());
	if (console["AutoSaveInterval"] > 0) {
		sch.refresh_cold("autosave", time(NULL) + console["AutoSaveInterval"]);
		sch.add_job_for(console["AutoSaveInterval"] * 60, "autosave");
	}
}

void check_system(DiceJob& job) {
	static int perRAM(0), perLastRAM(0);
	static double  perLastCPU(0), perLastDisk(0),
		 perCPU(0), perDisk(0);
	static bool isAlarmRAM(false), isAlarmCPU(false), isAlarmDisk(false);
	static double mbFreeBytes = 0, mbTotalBytes = 0;
	//�ڴ���
	if (console["SystemAlarmRAM"] > 0) {
		perRAM = getRamPort();
		if (perRAM > console["SystemAlarmRAM"] && perRAM > perLastRAM) {
			console.log("���棺" + GlobalMsg["strSelfName"] + "����ϵͳ�ڴ�ռ�ô�" + to_string(perRAM) + "%", 0b1000, printSTime(stNow));
			perLastRAM = perRAM;
			isAlarmRAM = true;
		}
		else if (perLastRAM > console["SystemAlarmRAM"] && perRAM < console["SystemAlarmRAM"]) {
			console.log("���ѣ�" + GlobalMsg["strSelfName"] + "����ϵͳ�ڴ�ռ�ý���" + to_string(perRAM) + "%", 0b10, printSTime(stNow));
			perLastRAM = perRAM;
			isAlarmRAM = false;
		}
	}
	//CPU���
	if (console["SystemAlarmCPU"] > 0) {
		perCPU = getWinCpuUsage() / 10.0;
		if (perCPU > console["SystemAlarmCPU"] && (!isAlarmCPU || perCPU > perLastCPU + 1)) {
			console.log("���棺" + GlobalMsg["strSelfName"] + "����ϵͳCPUռ�ô�" + toString(perCPU) + "%", 0b1000, printSTime(stNow));
			perLastCPU = perCPU;
			isAlarmCPU = true;
		}
		else if (perLastCPU > console["SystemAlarmCPU"] && perCPU < console["SystemAlarmCPU"]) {
			console.log("���ѣ�" + GlobalMsg["strSelfName"] + "����ϵͳCPUռ�ý���" + toString(perCPU) + "%", 0b10, printSTime(stNow));
			perLastCPU = perCPU;
			isAlarmCPU = false;
		}
	}
	//Ӳ�̼��
	if (console["SystemAlarmRAM"] > 0) {
		perDisk = getDiskUsage(mbFreeBytes, mbTotalBytes) / 10.0;
		if (perDisk > console["SystemAlarmDisk"] && (!isAlarmDisk || perDisk > perLastDisk + 1)) {
			console.log("���棺" + GlobalMsg["strSelfName"] + "����ϵͳӲ��ռ�ô�" + toString(perDisk) + "%", 0b1000, printSTime(stNow));
			perLastDisk = perDisk;
			isAlarmDisk = true;
		}
		else if (perLastDisk > console["SystemAlarmDisk"] && perDisk < console["SystemAlarmDisk"]) {
			console.log("���ѣ�" + GlobalMsg["strSelfName"] + "����ϵͳӲ��ռ�ý���" + toString(perDisk) + "%", 0b10, printSTime(stNow));
			perLastDisk = perDisk;
			isAlarmDisk = false;
		}
	}
	if (isAlarmRAM || isAlarmCPU || isAlarmDisk) {
		sch.add_job_for(5 * 60, job);
	}
	else {
		sch.add_job_for(30 * 60, job);
	}
}

//�����õ�ͼƬ�б�
void clear_image(DiceJob& job) {
	if (!job.fromQQ) {
		if (sch.is_job_cold("clrimage"))return;
		if (console["AutoClearImage"] <= 0) {
			sch.add_job_for(60 * 60, job);
			return;
		}
	}
	scanImage(GlobalMsg, sReferencedImage);
	scanImage(HelpDoc, sReferencedImage);
	scanImage(CardDeck::mPublicDeck, sReferencedImage);
	scanImage(CardDeck::mReplyDeck, sReferencedImage);
	scanImage(CardDeck::mGroupDeck, sReferencedImage);
	scanImage(CardDeck::mPrivateDeck, sReferencedImage);
	for (auto it : ChatList) {
		scanImage(it.second.strConf, sReferencedImage);
	}
	job.note("����" + GlobalMsg["strSelfName"] + "������ͼƬ" + to_string(sReferencedImage.size()) + "��", 0b0);
	int cnt = clrDir("data\\image\\", sReferencedImage);
	job.note("������image�ļ�"+ to_string(cnt) + "��", 1);
	if (console["AutoClearImage"] > 0) {
		sch.refresh_cold("clrimage", time(NULL) + console["AutoClearImage"]);
		sch.add_job_for(console["AutoClearImage"] * 60 * 60, "clrimage");
	}
}

void clear_group(DiceJob& job) {
	int intCnt = 0;
	ResList res;
	std::map<string, string>strVar;
	if (job.strVar["clear_mode"] == "unpower") {
		for (auto& [id, grp] : ChatList) {
			if (grp.isset("����") || grp.isset("����") || grp.isset("δ��") || grp.isset("����") || grp.isset("Э����Ч"))continue;
			if (grp.isGroup && getGroupMemberInfo(id, console.DiceMaid).permissions == 1) {
				res << printGroup(id);
				grp.leave(getMsg("strLeaveNoPower"));
				intCnt++;
				this_thread::sleep_for(3s);
			}
		}
		console.log(GlobalMsg["strSelfName"] + "ɸ����ȺȨ��Ⱥ��" + to_string(intCnt) + "��:" + res.show(), 0b10, printSTNow());
	}
	else if (isdigit(static_cast<unsigned char>(job.strVar["clear_mode"][0]))) {
		int intDayLim = stoi(job.strVar["clear_mode"]);
		string strDayLim = to_string(intDayLim);
		time_t tNow = time(NULL);
		for (auto& [id, grp] : ChatList) {
			if (grp.isset("����") || grp.isset("����") || grp.isset("δ��") || grp.isset("����") || grp.isset("Э����Ч"))continue;
			time_t tLast = grp.tLastMsg;
			if (int tLMT; grp.isGroup && (tLMT = getGroupMemberInfo(id, console.DiceMaid).LastMsgTime) > 0 && tLMT > tLast)tLast = tLMT;
			if (!tLast)continue;
			int intDay = (int)(tNow - tLast) / 86400;
			if (intDay > intDayLim) {
				strVar["day"] = to_string(intDay);
				res << printGroup(id) + ":" + to_string(intDay) + "��\n";
				grp.leave(getMsg("strLeaveUnused", strVar));
				intCnt++;
				this_thread::sleep_for(2s);
			}
		}
		console.log(GlobalMsg["strSelfName"] + "��ɸ��Ǳˮ" + strDayLim + "��Ⱥ��" + to_string(intCnt) + "����" + res.show(), 0b10, printSTNow());
	}
	else if (job.strVar["clear_mode"] == "black") {
		try {
			for (auto& [id, grp_name] : getGroupList()) {
				Chat& grp = chat(id).group().name(grp_name);
				if (grp.isset("����") || grp.isset("����") || grp.isset("δ��") || grp.isset("����") || grp.isset("���") || grp.isset("Э����Ч"))continue;
				if (blacklist->get_group_danger(id)) {
					res << printGroup(id) + "��" + "������Ⱥ";
					if (console["LeaveBlackGroup"])grp.leave(getMsg("strBlackGroup"));
				}
				vector<GroupMemberInfo> MemberList = getGroupMemberList(id);
				for (auto eachQQ : MemberList) {
					if (blacklist->get_qq_danger(eachQQ.QQID) > 1) {
						if (eachQQ.permissions < getGroupMemberInfo(id, getLoginQQ()).permissions) {
							continue;
						}
						else if (eachQQ.permissions > getGroupMemberInfo(id, getLoginQQ()).permissions) {
							res << printChat(grp) + "��" + printQQ(eachQQ.QQID) + "�Է�ȺȨ�޽ϸ�";
							grp.leave("���ֺ���������Ա" + printQQ(eachQQ.QQID) + "\n" + GlobalMsg["strSelfName"] + "��Ԥ������Ⱥ");
							intCnt++;
							break;
						}
						else if (console["LeaveBlackQQ"]) {
							res << printChat(grp) + "��" + printQQ(eachQQ.QQID);
							grp.leave("���ֺ�������Ա" + printQQ(eachQQ.QQID) + "\n" + GlobalMsg["strSelfName"] + "��Ԥ������Ⱥ");
							intCnt++;
							break;
						}
					}
				}
			}
		}
		catch (...) {
			console.log("���ѣ�" + GlobalMsg["strSelfName"] + "��������Ⱥ��ʱ����", 0b10, printSTNow());
		}
		if (intCnt) {
			job.note("�Ѱ�" + getMsg("strSelfName") + "���������Ⱥ��" + to_string(intCnt) + "����" + res.show(), 0b10);
		}
		else if (job.fromQQ) {
			job.echo(getMsg("strSelfName") + "��������δ���ִ����Ⱥ��");
		}
	}
	else if (job["clear_mode"] == "preserve") {
		for (auto& [id, grp] : ChatList) {
			if (grp.isset("����") || grp.isset("����") || grp.isset("δ��") || grp.isset("ʹ�����") || grp.isset("����") || grp.isset("Э����Ч"))continue;
			if (grp.isGroup && getGroupMemberInfo(id, console.master()).permissions) {
				grp.set("ʹ�����");
				continue;
			}
			res << printChat(grp);
			grp.leave(getMsg("strPreserve"));
			intCnt++;
			this_thread::sleep_for(3s);
		}
		console.log(GlobalMsg["strSelfName"] + "ɸ�������Ⱥ��" + to_string(intCnt) + "����" + res.show(), 1, printSTNow());
	}
	else
		job.echo("�޷�ʶ��ɸѡ������");
}

//�������
void cloud_beat(DiceJob& job) {
	Cloud::update();
	sch.add_job_for(5 * 60, job);
}

void dice_update(DiceJob& job) {
	job.note("��ʼ����Dice\n�汾:" + job.strVar["ver"], 1);
	if (Mirai) {
		mkDir("plugins/MiraiNative/pluginsnew");
		char pathDll[] = "plugins/MiraiNative/pluginsnew/com.w4123.dice.dll";
		char pathJson[] = "plugins/MiraiNative/pluginsnew/com.w4123.dice.json";
		string urlDll("https://shiki.stringempty.xyz/DiceVer/" + job.strVar["ver"] + "/com.w4123.dice.dll?" + to_string(job.fromTime));
		string urlJson("https://shiki.stringempty.xyz/DiceVer/" + job.strVar["ver"] + "/com.w4123.dice.json?" + to_string(job.fromTime));
		switch (Cloud::DownloadFile(urlDll.c_str(), pathDll)) {
		case -1:
			job.echo("����ʧ��:" + urlDll);
			break;
		case -2:
			job.note("����Diceʧ��!dll�ļ�δ���ص�ָ��λ��", 0b1);
			break;
		case 0:
		default:
			switch (Cloud::DownloadFile(urlJson.c_str(), pathJson)) {
			case -1:
				job.echo("����ʧ��:" + urlJson);
				break;
			case -2:
				job.note("����Diceʧ��!json�ļ�δ���ص�ָ��λ��", 0b1);
				break;
			case 0:
			default:
				job.note("����Dice!" + job.strVar["ver"] + "��ɹ���", 1);
			}
		}
	}
	else {
		char** path = new char* ();
		_get_pgmptr(path);
		string strAppPath(*path);
		delete path;
		strAppPath = strAppPath.substr(0, strAppPath.find_last_of("\\")) + "\\app\\com.w4123.dice.cpk";
		string strURL("https://shiki.stringempty.xyz/DiceVer/" + job.strVar["ver"] + "?" + to_string(job.fromTime));
		switch (Cloud::DownloadFile(strURL.c_str(), strAppPath.c_str())) {
		case -1:
			job.echo("����ʧ��:" + strURL);
			break;
		case -2:
			job.note("����Diceʧ��!�ļ�δ�ҵ�:" + strAppPath, 0b10);
			break;
		case 0:
			job.note("����Dice!" + job.strVar["ver"] + "��ɹ���\n����.system reload ����Ӧ�ø���", 1);
		}
	}
}

void dice_api_update(DiceJob& job) {
	string strURL("http://benzenpenxil.xyz/Oliva-Archives/");
	char** path = new char* ();
	_get_pgmptr(path);
	string strAppPath(*path);
	string strApiSaveLoc;
	if (Mirai)
	{
		strApiSaveLoc = "Dice" + to_string(console.DiceMaid) + "\\update\\DiceUpdateArchives.json";
	}
	else
	{
		strApiSaveLoc = "DiceData\\update\\DiceUpdateArchives.json";
	}
	if (job.strVar["ver"] == "list")
	{
		job.note("��ʼˢ�¸���Դ", 1);
		switch (Cloud::DownloadFile(strURL.c_str(), strApiSaveLoc.c_str())) {
		case -1:
			job.echo("����Դ��ȡʧ��:" + strURL);
			break;
		case -2:
			job.note("����Դ����ʧ��!����Դ����δ�ҵ�:" + strApiSaveLoc, 0b10);
			break;
		case 0:
			nlohmann::json j_api = freadJson(strApiSaveLoc);
			if (j_api != nlohmann::json())
			{
				try
				{
					vector<string> public_list = UTF8toGBK(j_api["public"].get<vector<string>>());
					string strNameTmp = UTF8toGBK(j_api["name"].get<string>());
					string strCommentTmp = UTF8toGBK(j_api["comment"].get<string>());
					string strPublicTmp;
					strPublicTmp += "\n��ָ��:" + strNameTmp;
					strPublicTmp += "\n��ѡ�ĸ���ָ����:";
					for (auto it : public_list)
					{
						strPublicTmp += "\n[.cloud update " + it + "]";
					}
					job.echo("�ѳɹ�ˢ�¸���Դ:" + strPublicTmp);
					job.echo("���Ըø���Դ�Ĺ���:\n" + strCommentTmp);
					job.note("�ѳɹ�������Դָ��:" + strNameTmp, 1);
				}
				catch (...)
				{
					job.note("����Դ��������쳣!", 1);
				}
			}
			else
			{
				job.note("����Դ�����쳣!", 1);
			}
			break;
		}
	}
	else
	{
		//job.note("���Ը���ģ��:" + job.strVar["ver"] + "��֧", 1);

		nlohmann::json j_api = freadJson(strApiSaveLoc);
		if (j_api != nlohmann::json())
		{
			try
			{
				vector<string> public_list = UTF8toGBK(j_api["public"].get<vector<string>>());
				if (count(public_list.begin(), public_list.end(), job.strVar["ver"]) > 0)
				{
					string strNameTmp = UTF8toGBK(j_api["data"][job.strVar["ver"]]["name"].get<string>());
					string strCommentTmp = UTF8toGBK(j_api["data"][job.strVar["ver"]]["comment"].get<string>());
					string strVersionTypeTmp = UTF8toGBK(j_api["data"][job.strVar["ver"]]["versiontype"].get<string>());
					string strVersionTmp = UTF8toGBK(j_api["data"][job.strVar["ver"]]["version"].get<string>());
					string strDownloadTmp = UTF8toGBK(j_api["data"][job.strVar["ver"]]["download"].get<string>());
					job.echo("��������:[" + strNameTmp + "]\n��֧˵��:\n" + strCommentTmp + "\n��֧�汾:" + strVersionTmp + "\n��������:" + strDownloadTmp);
				}
				else
				{
					job.echo(job.strVar["ver"] + "��֧:\n������!");
				}
			}
			catch (...)
			{
				job.note("����Դ��������쳣!", 1);
			}
		}
		else
		{
			job.note("����Դ�����쳣!", 1);
		}
	}
}

void dice_cnmods_api(DiceJob& job) {
	string strURL("https://www.cnmods.net/index/moduleListPage.do?title=" + UrlEncode(GBKtoUTF8(job.strVar["name"])) + "&page=" + GBKtoUTF8(job.strVar["page"]));
	char** path = new char* ();
	_get_pgmptr(path);
	string strAppPath(*path);
	string strApiSaveLoc;
	if (Mirai)
	{
		strApiSaveLoc = "Dice" + to_string(console.DiceMaid) + "\\cnmods\\cnmods_search_" + to_string(job.fromQQ) + ".json";
	}
	else
	{
		strApiSaveLoc = "DiceData\\cnmods\\cnmods_search_" + to_string(job.fromQQ) + ".json";
	}
	switch (Cloud::DownloadFile(strURL.c_str(), strApiSaveLoc.c_str())) {
	case -1:
		job.echo("ħ��ģ�����ʧ��");
		break;
	case -2:
		job.echo("ħ��ģ�黺�����");
		break;
	case 0:
		nlohmann::json j_api = freadJson(strApiSaveLoc);
		if (j_api != nlohmann::json())
		{
			try
			{
				string strPublicTmp;
				if (job.strVar["name"] == "")
				{
					strPublicTmp = "ħ���Ƽ�:";
					job.strVar["name"] = "ħ���Ƽ�";
				}
				else
				{
					strPublicTmp = "[" + job.strVar["name"] + "]��ħ��ģ���������:";
				}
				int intCountThisPage = 0;
				for (auto it : j_api["data"]["list"])
				{
					intCountThisPage += 1;
					strPublicTmp += "\n" + UTF8toGBK(it["title"].get<string>());
				}
				strPublicTmp += "\n---[" + job.strVar["page"] + "/" + to_string(j_api["data"]["totalPages"].get<long long>()) + "]---";
				if (intCountThisPage > 0)
				{
					job.echo(strPublicTmp);
				}
				else
				{
					job.echo("û�й���[" + job.strVar["name"] + "]��ħ��ģ���������");
				}
			}
			catch (...)
			{
				job.echo("ħ��ģ�����ݽ�������");
			}
		}
		else
		{
			job.echo("ħ��ģ�����ݽ���ʧ��");
		}
		break;
	}
}

//��ȡ�Ʋ�����¼
void dice_cloudblack(DiceJob& job) {
	job.echo("��ʼ��ȡ�ƶ˼�¼"); 
	string strURL("https://shiki.stringempty.xyz/blacklist/checked.json?" + to_string(job.fromTime));
	switch (Cloud::DownloadFile(strURL.c_str(), (DiceDir + "/conf/CloudBlackList.json").c_str())) {
	case -1:
		job.echo("ͬ���Ʋ�����¼ͬ��ʧ��:" + strURL);
		break;
	case -2:
		job.echo("ͬ���Ʋ�����¼ͬ��ʧ��!�ļ�δ�ҵ�");
		break;
	case 0:
		job.note("ͬ���Ʋ�����¼�ɹ���" + getMsg("self") + "��ʼ��ȡ", 1);
		blacklist->loadJson(DiceDir + "/conf/CloudBlackList.json", true);
	}
}

void log_put(DiceJob& job) {
	job["ret"] = put_s3_object("dicelogger",
							   job.strVar["log_file"],
							   job.strVar["log_path"],
							   "ap-southeast-1");
	if (job["ret"] == "SUCCESS") {
		job.echo(getMsg("strLogUpSuccess", job.strVar));
	}
	else if (job.cntExec++ > 1) {
		job.echo(getMsg("strLogUpFailureEnd",job.strVar));
	}
	else {
		job["retry"] = to_string(job.cntExec);
		job.echo(getMsg("strLogUpFailure", job.strVar));
		sch.add_job_for(2 * 60, job);
	}
}

string print_master() {
	if (!console.master())return "��������";
	return printQQ(console.master());
}

string list_deck() {
	return listKey(CardDeck::mPublicDeck);
}
string list_extern_deck() {
	return listKey(CardDeck::mExternPublicDeck);
}
