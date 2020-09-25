#pragma once
#include <string>
#include "STLExtern.hpp"
#include <unordered_set>
using std::string;
using std::unordered_set;

class Censor {
	string dirWords;
public:
	enum class Level :size_t {
		Ignore,		//����
		Notice,		//��0��֪ͨ
		Caution,		//��1��֪ͨ
		Warning,		//�����û��Ҿܾ��ƺ�����1��֪ͨ
		Danger,		//�����û��Ҿܾ�ָ���3��֪ͨ
		Critical,		//��ռλ��������
	};
	map<string, Level, less_ci> words;
	void insert(const string& word, Level);
	//void load();
	void build();
	size_t size()const { return words.size(); }
	int search(const string& text, unordered_set<string>& res);
};

inline Censor censor;

int load_words(const string& filename, Censor& cens);