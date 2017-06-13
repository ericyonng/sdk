#ifndef TASK_H_
#define TASK_H_

#pragma once

#include"TdxmHead.h"
#include "../结构定义/BusinessLayerProtocol.h"

//链接服务器
class CConnectServerTask :public CTaskBase
{
public:
	CConnectServerTask(void * pArg, void *pClient){ m_pClient = pClient; m_pArg = pArg; }
	virtual ~CConnectServerTask(){}

public:
	virtual int Run();
	//释放
	virtual void Release(){ delete this; }

protected:
	void *  m_pClient;
	void *	m_pArg;
};

//发送数据
class CSendDataToServerTask :public CTaskBase
{
public:
	CSendDataToServerTask(){}
	virtual ~CSendDataToServerTask(){}

public:
	//任务执行体
	virtual int Run();
	//释放
	virtual void Release(){ delete this; }
};

//注册任务
class CRegTask :public CTaskBase
{
public:
	CRegTask(REGISTER_BODY_PACK_T stRegPack){ m_stRegPack = stRegPack;  }
	virtual ~CRegTask(){}

public:
	//任务执行体
	virtual int Run();
	//释放
	virtual void Release(){ delete this; }
protected:
	REGISTER_BODY_PACK_T m_stRegPack;
};


//修改密码任务
class CModPwdTask :public CTaskBase
{
public:
	CModPwdTask(MODIFY_PWD_BODY_PACK_T stModPwdPack){ m_stModPwdPack = stModPwdPack; }
	virtual ~CModPwdTask(){}

public:
	//任务执行体
	virtual int Run();
	//释放
	virtual void Release(){ delete this; }
protected:
	MODIFY_PWD_BODY_PACK_T m_stModPwdPack;
};
#endif