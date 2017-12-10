/************************************************************************************
 * file: 		Singleton.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/10 22:53:43
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/10			
 *
 ************************************************************************************/
#ifndef __Singleton_h__
#define __Singleton_h__

#include <stdio.h>

#define CONNECT(A,B)		A##B
#define CONNECT_PTR(A,B)	(* A##B)

#define INSTANCE(VAR)		VAR::getInstance()

// singleton model
#define CREATE_SIGNLETON(VAR, pInstance, NULLPTR)		\
private:												\
	VAR(){}												\
	VAR(VAR const&);									\
	VAR& operator= (VAR const&);						\
	~VAR(){}											\
														\
public:													\
	static VAR* getInstance()							\
	{													\
		if (NULLPTR == pInstance)						\
			pInstance = new VAR();						\
		return pInstance;								\
	}													\
														\
	static void destroy()								\
	{													\
		if (NULLPTR != pInstance)						\
		{												\
			delete pInstance;							\
			pInstance = NULLPTR;						\
		}												\
	}													\
														\
private:												\

// singleton model init
#define CREATE_SINGLETON_INIT(VAR, pInstance, NULLPTR)	\
private:												\
	VAR(){}												\
	VAR(VAR const&);									\
	VAR& operator= (VAR const&);						\
	~VAR(){}											\
														\
public:													\
	static VAR* getInstance()							\
	{													\
		if (NULLPTR == pInstance)						\
		{												\
			pInstance = new VAR();						\
			pInstance->init();							\
		}												\
		return pInstance;								\
	}													\
														\
	static void destroy()								\
	{													\
		if (NULLPTR != pInstance)						\
		{												\
			delete pInstance;							\
			pInstance = NULLPTR;						\
		}												\
	}													\
														\
private:												\
														
#define CREATE_SINGLETON_MUTEX(VAR, pInstance, NULLPTR)	\
private:												\
	VAR(){}												\
	VAR(VAR const&);									\
	VAR& operator= (VAR const&);						\
	~VAR(){}											\
														\
public:													\
	static VAR* getInstance()							\
	{													\
		if (NULLPTR == pInstance)						\
		{												\
			static std::mutex m_mutex;					\
			std::lock_guard<std::mutex> lck(m_mutex);	\
			if (NULLPTR == pInstance)					\
			{											\
				pInstance = new VAR();					\
				pInstance->init();						\
			}											\
		}												\
		return pInstance;								\
	}													\
														\
	static void destroy()								\
	{													\
		if (NULLPTR != pInstance)						\
		{												\
			static std::mutex m_mutex;					\
			std::lock_guard<std::mutex> lck(m_mutex);	\
			if (NULLPTR != pInstance)					\
			{											\
				delete pInstance;						\
				pInstance = NULLPTR;					\
			}											\
		}												\
	}													\
														\
private:												\

// singleton model mutex clear
#define CREATE_SINGLETON_MUTEX_CLEAR(VAR,pInstance,NULLPTR)\
private:												\
	VAR(){}												\
	VAR(VAR const&);									\
	VAR& operator= (VAR const&);						\
	~VAR(){}											\
														\
public:													\
	static VAR* getInstance()							\
	{													\
		if (NULLPTR == pInstance)						\
		{												\
			static std::mutex m_mutex;					\
			std::lock_guard<std::mutex> lck(m_mutex);	\
			if (NULLPTR == pInstance)					\
			{											\
				pInstance = new VAR();					\
				pInstance->init();						\
			}											\
		}												\
		return pInstance;								\
	}													\
														\
	static void destroy()								\
	{													\
		if (NULLPTR != pInstance)						\
		{												\
			static std::mutex m_mutex;					\
			std::lock_guard<std::mutex> lck(m_mutex);	\
			if (NULLPTR != pInstance)					\
			{											\
				pInstance->clear();						\
				delete pInstance;						\
				pInstance = NULLPTR;					\
			}											\
		}												\
		return;											\
	}													\
														\
private:												\

#endif // __Singleton_h__
