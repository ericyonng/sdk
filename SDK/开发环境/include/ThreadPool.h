#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#pragma once

#include "ThreadHead.h"
//�������ؽ��
#define RES_UN_DO		0											//�޶���
#define RES_SUCCESS		1											//�ɹ�
#define RES_FAILED		2											//ʧ��

//�㲥״̬
#define CAST_UN_DO		0											//�޶���
#define CASTED_ON		1											//֪ͨ
#define CASTED_OFF		2											//�ر�


//���߳�֧��
#include <process.h> 
#include<afxmt.h>
//#include"stdafx.h"


//�������
#include"TaskBase.h"

//����
#include<list>
using namespace std;


//����
struct stThreadPara
{
	HANDLE hObject;													//�߳���Դ
	void *pArg;														//����
};


//������
class THREAD_CLASS CDataLocker
{
	//��������
protected:
	volatile	INT								m_nLockCount;					//��������
	CCriticalSection							m_CSLockerObj;					//�ٽ���
	CCriticalSection							m_CSLockerMe;					//������������

	//��������
public:
	//���캯��
	CDataLocker(bool bLockAtOnce=false);
	//��������
	virtual ~CDataLocker();
	//�ͷŶ���
	virtual void Release(){ try{ delete this; } catch (...){} }

	//��������
public:
	//��������
	INT Lock();
	//�������� 
	INT UnLock();

	//״̬����
public:
	//��������
	INT GetLockCount();
};

//�߳�ͬ����
class THREAD_CLASS CSynLocker : public CDataLocker
{
	//ͬ������ʶ
protected:
	CDataLocker										m_ResLocker;					//������Դ��
	HANDLE											m_hEvent;						//�¼����
	TCHAR											m_tcSynName[32];				//�¼�����
	volatile	INT									m_nTimOut;						//�Ƿ�ʱ

	//�㲥�ź�
protected:
	//CDataLocker									m_BroadCastLocker;				//�㲥������
	INT												m_nIsBroadCast;					//�㲥״̬
	volatile	INT									m_nWakeThreadNum;				//�����߳���
	volatile	INT									m_nCurThreadNum;				//��ǰ�߳���

	//��һ�ź�
protected:
	//CDataLocker									m_SinalLocker;					//��һ�źŻ�����
	volatile	bool								m_bIsSinal;						//��һ�źŷ��ͱ�־
	volatile	bool								m_bIsOneThreadWaked;			//һ���̻߳��ѱ�־

	//��ʼ�����ͷ�
public:
	//���캯��
	//CSynLocker(void *lpszName=TEXT("YCLocker"), bool bLockAtOnce=false, bool bInitiallyOwn=false, bool bManualReset=true);
	//���캯��
	CSynLocker(void *lpszName = NULL, bool bLockAtOnce = false, bool bInitiallyOwn = false, bool bManualReset = true);
	//��������
	virtual ~CSynLocker();

	//�߳�ͬ������
public:
	//�̵߳ȴ�����
	INT WaitForSingleObject(int nWaitTimeOut=INFINITE);
	//�ȴ�ȫ���߳�����
	INT WaitForAllWake(INT *pnCastStatus);
	//�̻߳���
	INT Sinal();
	//�߳�Ⱥ����
	INT BroadCast(INT nCurThreadNum, INT **pnCastStatus);
	//��ȡ��ʱ״̬
	INT GetTimeOut();	
	//��ȡ�߳�ͬ����ָ��
	CSynLocker *GetThis();	
};

//�߳���
//class THREAD_CLASS CThread
//{
//public:
//	CThread();
//	virtual ~CThread();

	//�߳�����
	//�̴߳���������
	//�̷߳�����join
	//�̵߳���
	//������
//protected:
//};

//�̳߳�
class THREAD_CLASS CThreadPool
{
	//ʵ������
protected:
	static CThreadPool *								m_pPool;

	//�̳߳ز���
protected:
	volatile	int										m_nMinNum;						//��С�߳���
	volatile	int										m_nMaxNum;						//����߳���
	volatile	int										m_nCurNum;						//��ǰ�ܵ��߳���
	volatile	int										m_nWaitNum;						//�ȴ��߳���
	volatile	bool									m_bIsDestroy;					//�̳߳��Ƿ�����

	//����
protected:
	volatile	bool									m_bIsStopAdd;					//ֹͣ�������
	CSynLocker											m_CSynLockerObj;				//���߳��������ͬ��
	list<CTaskBase *>									m_listTasklist;					//�������
	//list<stThreadPara *>								m_listThreadHandle;				//�߳���Դ��ʶ

	//����
protected:
	//CSynLocker *										m_pRecycleThreadObj;			//�̻߳���ͬ��

	//��ʼ�����ͷ�
public:
	//���캯��
	CThreadPool(int nMinNum, int nMaxNum, LPTSTR lpszName=TEXT("YCLocker"), bool bLockAtOnce=false, BOOL bInitiallyOwn=FALSE, BOOL bManualReset=TRUE);	
	//��������
	virtual ~CThreadPool();	
	//�ͷŶ���
	virtual void Release(){ try{ delete this; } catch (...){} }

	//�̲߳���
public:
	//�����߳�
	INT CreateThread(int nThreadNum);
	//�̴߳�����
	static unsigned __stdcall ThreadHandler(LPVOID pThreadData);

	//����
public:
	//�������
	INT AddTask(CTaskBase *pTask, int iThreadPerNum = 1);
	//ֹͣ�������
	INT StopAdd();
	//ʹ���������
	INT EnAbleAdd();

	//ͬ������
public:
	//�߳�ͬ�������
	CSynLocker *GetSynLockerObj(){ return &m_CSynLockerObj; }
	//�̻߳���ͬ�������
	//CSynLocker *GetSynRecycleObj(){return m_pRecycleThreadObj;}	

	//ʵ��
public:
	static CThreadPool *GetInstance(){ return m_pPool; }
};

//�̳߳ض���
extern  CThreadPool THREAD_CLASS GlobalThreadPool;

#endif
