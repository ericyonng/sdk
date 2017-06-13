#ifndef TASK_H_
#define TASK_H_

#pragma once

#include"../Thread/TaskBase.h"
#include"../结构定义/BusinessLayerProtocol.h"

class CMonitorSockTask :public CTaskBase
{
public:
	CMonitorSockTask(void *pArg){ m_pArg = pArg; }
	virtual ~CMonitorSockTask(){}

public:
	//任务执行体
	virtual int Run();
	//释放
	virtual void Release(){ delete this; }

protected:
	void *m_pArg;
};


class CReadDataTask :public CTaskBase
{
public:
	CReadDataTask(void *pArg, int iSockFD){ m_iSockFD = iSockFD; m_pArg = pArg; }
	virtual ~CReadDataTask(){}

public:
	//任务执行体
	virtual int Run();
	//释放
	virtual void Release(){ delete this; }

protected:
	void *m_pArg;
	int m_iSockFD;
};

class CRegTask :public CTaskBase
{
public:
	CRegTask(SOCKET hSock, REGISTER_BODY_PACK_T stClntRegPack, void *pArg){ m_hSock = hSock; m_pArg = pArg; m_stClntRegPack = stClntRegPack; }
	virtual ~CRegTask(){}

public:
	//任务执行体
	virtual int Run();
	//释放
	virtual void Release(){ delete this; }
protected:
	SOCKET m_hSock;
	REGISTER_BODY_PACK_T m_stClntRegPack;
	void *m_pArg;
};

class CModPwdTask :public CTaskBase
{
public:
	CModPwdTask(SOCKET hSock, MODIFY_PWD_BODY_PACK_T stClntModPack, void *pArg){ m_hSock = hSock; m_pArg = pArg; m_stClntModPack = stClntModPack; }
	virtual ~CModPwdTask(){}

public:
	//任务执行体
	virtual int Run();
	//释放
	virtual void Release(){ delete this; }
protected:
	SOCKET m_hSock;
	MODIFY_PWD_BODY_PACK_T m_stClntModPack;
	void *m_pArg;
};

//数据库自动重连
class CDBReconnectTask :public CTaskBase
{
public:
	CDBReconnectTask(long lReconnectTime, void *pMySql, void *pLocker, void *pListenDlg){ m_lReconnectTime = lReconnectTime; m_pMySql = pMySql; m_pLocker = pLocker; m_pListenDlg = pListenDlg; }
	~CDBReconnectTask(){}

public:
	//任务执行体
	virtual int Run();
	//释放
	virtual void Release(){ delete this; }

protected:
	long	m_lReconnectTime;	//自动重连时间
	void *	m_pMySql;
	void *  m_pLocker;
	void * m_pListenDlg;
};

//监控窗口变大与否
class CListenSizeLimitTask :public CTaskBase
{
public:
	CListenSizeLimitTask(void *pArg){ m_pArg = pArg; }
	virtual ~CListenSizeLimitTask(){}
	//任务执行体
	virtual int Run();
	//释放
	virtual void Release(){ delete this; }

protected:
	void *m_pArg;

};
#endif