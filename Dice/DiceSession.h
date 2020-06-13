#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>

using std::pair;
using std::string;
using std::to_string;
using std::vector;
using std::map;
using std::multimap;
using std::set;

class FromMsg;
class DiceTableMaster;

class DiceSession
{
	//��ֵ��
	map<string, map<string, int>> mTable;
	//�Թ���
	set<long long> sOB;
public:
	//Ⱥ��
	long long room;

	DiceSession(long long group) : room(group)
	{
		tUpdate = tCreate = time(nullptr);
	}

	friend void dataInit();
	friend class DiceTableMaster;

	//��¼����ʱ��
	time_t tCreate;
	//������ʱ��
	time_t tUpdate;

	DiceSession& create(time_t tt)
	{
		tCreate = tt;
		return *this;
	}

	DiceSession& update(time_t tt)
	{
		tUpdate = tt;
		save();
		return *this;
	}

	DiceSession& update()
	{
		tUpdate = time(nullptr);
		return *this;
	}

	bool table_count(string key) const { return mTable.count(key); }
	int table_add(string, int, string);
	string table_prior_show(string key) const;
	bool table_clr(string key);

	//�Թ�ָ��
	int ob_enter(FromMsg*);
	int ob_exit(FromMsg*);
	int ob_list(FromMsg*) const;
	int ob_clr(FromMsg*);
	set<long long> get_ob() const { return sOB; }

	DiceSession& clear_ob()
	{
		sOB.clear();
		return *this;
	}

	void save() const;
};

using Session = DiceSession;

class DiceFullSession : public DiceSession
{
	set<long long> sGM;
	set<long long> sPL;
};

class DiceTableMaster
{
public:
	map<long long, std::shared_ptr<Session>> mSession;
	Session& session(long long group);
	void session_end(long long group);
	void save();
	int load();
};

inline std::unique_ptr<DiceTableMaster> gm;
