#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#pragma once

#include "ThreadHead.h"
//函数返回结果
#define RES_UN_DO		0											//无动作
#define RES_SUCCESS		1											//成功
#define RES_FAILED		2											//失败

//广播状态
#define CAST_UN_DO		0											//无动作
#define CASTED_ON		1											//通知
#define CASTED_OFF		2											//关闭


//多线程支持
#include <process.h> 
#include<afxmt.h>
//#include"stdafx.h"


//任务基类
#include"TaskBase.h"

//容器
#include<list>
using namespace std;


//参数
struct stThreadPara
{
	HANDLE hObject;													//线程资源
	void *pArg;														//参数
};


//数据锁
class THREAD_CLASS CDataLocker
{
	//变量定义
protected:
	volatile	INT								m_nLockCount;					//锁定计数
	CCriticalSection							m_CSLockerObj;					//临界区
	CCriticalSection							m_CSLockerMe;					//保护自身区域

	//函数定义
public:
	//构造函数
	CDataLocker(bool bLockAtOnce=false);
	//析构函数
	virtual ~CDataLocker();
	//释放对象
	virtual void Release(){ try{ delete this; } catch (...){} }

	//操作函数
public:
	//锁定函数
	INT Lock();
	//解锁函数 
	INT UnLock();

	//状态函数
public:
	//锁定次数
	INT GetLockCount();
};

//线程同步类
class THREAD_CLASS CSynLocker : public CDataLocker
{
	//同步锁标识
protected:
	CDataLocker										m_ResLocker;					//公共资源锁
	HANDLE											m_hEvent;						//事件句柄
	TCHAR											m_tcSynName[32];				//事件名称
	volatile	INT									m_nTimOut;						//是否超时

	//广播信号
protected:
	//CDataLocker									m_BroadCastLocker;				//广播唤醒锁
	INT												m_nIsBroadCast;					//广播状态
	volatile	INT									m_nWakeThreadNum;				//唤醒线程数
	volatile	INT									m_nCurThreadNum;				//当前线程数

	//单一信号
protected:
	//CDataLocker									m_SinalLocker;					//单一信号唤醒锁
	volatile	bool								m_bIsSinal;						//单一信号发送标志
	volatile	bool								m_bIsOneThreadWaked;			//一个线程唤醒标志

	//初始化与释放
public:
	//构造函数
	//CSynLocker(void *lpszName=TEXT("YCLocker"), bool bLockAtOnce=false, bool bInitiallyOwn=false, bool bManualReset=true);
	//构造函数
	CSynLocker(void *lpszName = NULL, bool bLockAtOnce = false, bool bInitiallyOwn = false, bool bManualReset = true);
	//析构函数
	virtual ~CSynLocker();

	//线程同步操作
public:
	//线程等待阻塞
	INT WaitForSingleObject(int nWaitTimeOut=INFINITE);
	//等待全部线程苏醒
	INT WaitForAllWake(INT *pnCastStatus);
	//线程唤醒
	INT Sinal();
	//线程群唤醒
	INT BroadCast(INT nCurThreadNum, INT **pnCastStatus);
	//获取超时状态
	INT GetTimeOut();	
	//获取线程同步类指针
	CSynLocker *GetThis();	
};

//线程类
//class THREAD_CLASS CThread
//{
//public:
//	CThread();
//	virtual ~CThread();

	//线程配置
	//线程创建与销毁
	//线程分离与join
	//线程调度
	//数据区
//protected:
//};

//线程池
class THREAD_CLASS CThreadPool
{
	//实例对象
protected:
	static CThreadPool *								m_pPool;

	//线程池参数
protected:
	volatile	int										m_nMinNum;						//最小线程数
	volatile	int										m_nMaxNum;						//最大线程数
	volatile	int										m_nCurNum;						//当前总的线程数
	volatile	int										m_nWaitNum;						//等待线程数
	volatile	bool									m_bIsDestroy;					//线程池是否销毁

	//任务
protected:
	volatile	bool									m_bIsStopAdd;					//停止添加任务
	CSynLocker											m_CSynLockerObj;				//多线程任务队列同步
	list<CTaskBase *>									m_listTasklist;					//任务队列
	//list<stThreadPara *>								m_listThreadHandle;				//线程资源标识

	//回收
protected:
	//CSynLocker *										m_pRecycleThreadObj;			//线程回收同步

	//初始化与释放
public:
	//构造函数
	CThreadPool(int nMinNum, int nMaxNum, LPTSTR lpszName=TEXT("YCLocker"), bool bLockAtOnce=false, BOOL bInitiallyOwn=FALSE, BOOL bManualReset=TRUE);	
	//析构函数
	virtual ~CThreadPool();	
	//释放对象
	virtual void Release(){ try{ delete this; } catch (...){} }

	//线程操作
public:
	//创建线程
	INT CreateThread(int nThreadNum);
	//线程处理函数
	static unsigned __stdcall ThreadHandler(LPVOID pThreadData);

	//任务
public:
	//添加任务
	INT AddTask(CTaskBase *pTask, int iThreadPerNum = 1);
	//停止添加任务
	INT StopAdd();
	//使能添加任务
	INT EnAbleAdd();

	//同步对象
public:
	//线程同步类对象
	CSynLocker *GetSynLockerObj(){ return &m_CSynLockerObj; }
	//线程回收同步类对象
	//CSynLocker *GetSynRecycleObj(){return m_pRecycleThreadObj;}	

	//实例
public:
	static CThreadPool *GetInstance(){ return m_pPool; }
};

//线程池对象
extern  CThreadPool THREAD_CLASS GlobalThreadPool;

#endif
