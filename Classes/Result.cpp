#include "Result.h"

RowInfo::RowInfo(const char* str)
{
	if (NULL == str)
	{
		_str = new char[1];
		*_str = '\0';
	}
	else
	{
		int len = strlen(str);
		_str = new char[len + 1];
		strncpy(_str, str, len + 1);
	}
}
// 拷贝构造器  
RowInfo::RowInfo(const RowInfo & another)
{
	int len = strlen(another._str);
	_str = new char[len + 1];
	strncpy(_str, another._str, len + 1);
}
// 析构函数  
RowInfo::~RowInfo()
{
	delete[]_str;
}
// 赋值运算符重载  
RowInfo & RowInfo::operator = (const RowInfo & another)
{
	// 自赋值，出现错误  
	if (this == &another)
	{
		return *this;
	}
	// 先删除自己开辟的空间  
	delete[]_str;
	int len = strlen(another._str);
	this->_str = new char[len + 1];
	strncpy(this->_str, another._str, len + 1);
	return *this;
}
// 加法运算符重载  
RowInfo RowInfo::operator + (const RowInfo & another)
{
	int len = strlen(this->_str) + strlen(another._str);
	RowInfo str;
	delete[]str._str;
	str._str = new char[len + 1];
	memset(str._str, 0, len + 1);
	int len1 = strlen(this->_str) + 1;
	strncpy(str._str, this->_str, len1);
	// 源串长度 + 目标串长度 + 结束符  
	int len2 = strlen(this->_str) + strlen(another._str) + 1;
	strncpy(str._str, another._str, len2);
	return str;
}
// ==关系运算符重载  
bool RowInfo::operator==(const RowInfo &other)
{
	if (strcmp(this->_str, other._str) == 0)
		return true;
	else
		return false;
}
// >关系运算符重载  
bool RowInfo::operator>(const RowInfo &other)
{
	if (strcmp(this->_str, other._str) > 0)
		return true;
	else
		return false;
}
// <运算符重载  
bool RowInfo::operator<(const RowInfo &other)
{
	if (strcmp(this->_str, other._str) < 0)
		return true;
	else
		return false;
}
// []运算符重载  
char& RowInfo::operator[](int idx)
{
	return _str[idx];
}