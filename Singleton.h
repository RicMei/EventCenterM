/*****************************************************************************
* @Copyright 2017, Meishuhao
* @All rights reserved.
* @brief : singleton : no thread safe
* @author : alpha.shuhao@gmail.com
* @date : 2017/07/26
* @file : Singleton.h
* @version : ver 1.0
*****************************************************************************/
#ifndef FILE_DEF_SINGLETON_H_
#define FILE_DEF_SINGLETON_H_
#include <mutex>
class noncopyable
{
protected:
	noncopyable() {}
	~noncopyable() {}
private:  // emphasize the following members are private 
	noncopyable(const noncopyable&);
	const noncopyable& operator=(const noncopyable&);
};
template<typename T>
class Singleton : public noncopyable
{
public:
	static T& instance()
	{
		call_once(ponce_, &Singleton::init);
		return *value_;
	}
private:
	static void init()
	{
		value_ = new T();
	}
private:
	static std::once_flag ponce_;
	static T*             value_;
};
template<typename T>
std::once_flag Singleton<T>::ponce_;
template<typename T>
T* Singleton<T>::value_ = nullptr;

#endif //FILE_DEF_SINGLETON_H_