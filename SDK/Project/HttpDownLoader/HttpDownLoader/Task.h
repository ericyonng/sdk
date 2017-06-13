#ifndef HTTP_TASK_H_
#define HTTP_TASK_H_

#pragma once

#include "ThreadHead.h"

//class CTaskBase
//{};

//����http����
class CHttpListenTask : public CTaskBase
{
public:
	CHttpListenTask(void *pArg){ m_pArg = pArg; }
	virtual ~CHttpListenTask(){}
	//����ִ����
	virtual int Run();
	//�ͷ�
	virtual void Release(){ delete this; }

protected:
	void *		m_pArg;
};

//����ƽ̨΢�Žӿ�����ת����΢��ƽ̨�����ά�����ӣ�
class CHttpWxApiAnalysisTask : public CTaskBase
{
public:
	CHttpWxApiAnalysisTask(char *pWxApiLink, void *pArg)
	{
		//m_strLoginType = pLoginType;
		strWxApiLink = pWxApiLink;
		m_pArg = pArg; 

	}
	virtual ~CHttpWxApiAnalysisTask(){}
	//����ִ����
	virtual int Run();
	//�ͷ�
	virtual void Release(){ delete this; }

protected:
	void *		m_pArg;
	string      strWxApiLink;
	string		m_strQrCode;
	//string		m_strLoginType;
	string		m_strCodeBack;
};

//��ȡ������Ϣ����
class CHttpGetPersonalInfoTask :public CTaskBase
{
public:
	CHttpGetPersonalInfoTask(char *pWxApiLink, char *redirect_uri, void *pArg, void *pCodeWnd){ m_strWxApiLink = pWxApiLink; m_strRedirect_uri = redirect_uri; m_pArg = pArg; m_pCodeWnd = pCodeWnd; }
	virtual ~CHttpGetPersonalInfoTask(){}
	//����ִ����
	virtual int Run();
	//�ͷ�
	virtual void Release(){ delete this; }

protected:
	void *		m_pArg;
	void *		m_pCodeWnd;
	string      m_strWxApiLink;
	string		m_strRedirect_uri;

};

//��������
class CHttpDownloadTask :public CTaskBase
{
public:
	CHttpDownloadTask(string strLink, unsigned short port, string strPath, bool bSaveHttpHeaderInfo, void *pArg)
	{
		m_strLink = strLink;
		m_port = port;
		m_strPath = strPath;
		m_bSaveHttpHeaderInfo = bSaveHttpHeaderInfo;
		m_pArg = pArg; 
	}
	virtual ~CHttpDownloadTask(){}
	//����ִ����
	virtual int Run();
	//�ͷ�
	virtual void Release(){ delete this; }

protected:
	void *				m_pArg;
	string				m_strLink;
	unsigned short		m_port;
	string				m_strPath;
	bool				m_bSaveHttpHeaderInfo;
};

//����ͳ��
class CDataFlowCount :public CTaskBase
{
public:
	CDataFlowCount(long *plTotalSize, long *plCurrentRecvByte,bool *pbFinish, void *pArg)
	{
		m_pArg = pArg;
		m_plTotalSize = plTotalSize;
		m_plCurrentRecvByte = plCurrentRecvByte;
		m_pbFinish = pbFinish;
	}
	virtual ~CDataFlowCount(){}
	//����ִ����
	virtual int Run();
	//�ͷ�
	virtual void Release(){ delete this; }

protected:
	void *				m_pArg;
	long *				m_plTotalSize;
	long *				m_plCurrentRecvByte;
	bool *				m_pbFinish;
};
#endif