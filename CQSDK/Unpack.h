#pragma once
#include <vector>
#include <string>

void show(void* t, int len);

class Unpack final
{
	std::vector<unsigned char> buff;
public:
	Unpack() noexcept;
	explicit Unpack(const char*);
	explicit Unpack(std::vector<unsigned char>) noexcept;
	explicit Unpack(const std::string&);

	Unpack& setData(const char* i, int len);
	Unpack& clear() noexcept;
	[[nodiscard]] int len() const noexcept;

	Unpack& add(int i); //���һ������
	int getInt() noexcept; //����һ������

	Unpack& add(long long i); //���һ��������
	long long getLong() noexcept; //����һ��������

	Unpack& add(short i); //���һ��������
	short getshort() noexcept; //����һ��������

	Unpack& add(const unsigned char* i, short len); //���һ���ֽڼ�(����add(std::string i);)
	std::vector<unsigned char> getchars() noexcept; //����һ���ֽڼ�(����getstring();)

	Unpack& add(std::string i); //���һ���ַ���
	std::string getstring(); //����һ���ַ���

	Unpack& add(Unpack& i); //���һ��Unpack
	Unpack getUnpack() noexcept; //����һ��Unpack

	std::string getAll() noexcept; //���ر�������

	void show();
};
