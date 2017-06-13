#ifndef TASK_H_
#define TASK_H_

#pragma once

#include"TdxmHead.h"
#include "../�ṹ����/BusinessLayerProtocol.h"

//���ӷ�����
class CConnectServerTask :public CTaskBase
{
public:
	CConnectServerTask(void * pArg, void *pClient){ m_pClient = pClient; m_pArg = pArg; }
	virtual ~CConnectServerTask(){}

public:
	virtual int Run();
	//�ͷ�
	virtual void Release(){ delete this; }

protected:
	void *  m_pClient;
	void *	m_pArg;
};

//��������
class CSendDataToServerTask :public CTaskBase
{
public:
	CSendDataToServerTask(){}
	virtual ~CSendDataToServerTask(){}

public:
	//����ִ����
	virtual int Run();
	//�ͷ�
	virtual void Release(){ delete this; }
};

//ע������
class CRegTask :public CTaskBase
{
public:
	CRegTask(REGISTER_BODY_PACK_T stRegPack){ m_stRegPack = stRegPack;  }
	virtual ~CRegTask(){}

public:
	//����ִ����
	virtual int Run();
	//�ͷ�
	virtual void Release(){ delete this; }
protected:
	REGISTER_BODY_PACK_T m_stRegPack;
};


//�޸���������
class CModPwdTask :public CTaskBase
{
public:
	CModPwdTask(MODIFY_PWD_BODY_PACK_T stModPwdPack){ m_stModPwdPack = stModPwdPack; }
	virtual ~CModPwdTask(){}

public:
	//����ִ����
	virtual int Run();
	//�ͷ�
	virtual void Release(){ delete this; }
protected:
	MODIFY_PWD_BODY_PACK_T m_stModPwdPack;
};
#endif