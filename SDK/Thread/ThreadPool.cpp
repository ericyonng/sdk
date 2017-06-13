#include "stdafx.h"

#include "ThreadPool.h"
#include<string.h>
//#include"WriteLog.h"

static int ThreadID_i=0;

CThreadPool *CThreadPool::m_pPool = NULL;

//�̳߳�פ���ڴ�
CThreadPool GlobalThreadPool(0, 200);//��ʼ��5���̣߳��������200���߳�

//������Դ��
//���캯��
CDataLocker::CDataLocker(bool bLockAtOnce) 
{
	//���ñ���
	m_nLockCount=0;

	return;
}

//��������
CDataLocker::~CDataLocker()
{
	//CWriteLog::writeLog("CDataLocker ~CDataLocker 1", strlen("CDataLocker ~CDataLocker 1"), "test.txt");

	//�������
	while (m_nLockCount>0)
	{

		//CWriteLog::writeLog("CDataLocker ~CDataLocker 2", strlen("CDataLocker ~CDataLocker 2"), "test.txt");

		if(UnLock()!=RES_SUCCESS)  break;

		//CWriteLog::writeLog("CDataLocker ~CDataLocker 3", strlen("CDataLocker ~CDataLocker 3"), "test.txt");

	}

	//CWriteLog::writeLog("CDataLocker ~CDataLocker 4", strlen("CDataLocker ~CDataLocker 4"), "test.txt");

	return;
}

//��������
INT CDataLocker::Lock()
{
	m_CSLockerMe.Lock();
	m_nLockCount++;
	m_CSLockerMe.Unlock();
	BOOL bRet = m_CSLockerObj.Lock();

	if (bRet == FALSE)
	{
		m_CSLockerMe.Lock();
		m_nLockCount--;
		m_CSLockerMe.Unlock();
	}

	return bRet == TRUE ? RES_SUCCESS : RES_FAILED;
}

//��������
INT CDataLocker::UnLock()
{
	//Ч��״̬
	m_CSLockerMe.Lock();
	if (m_nLockCount <= 0)
	{
		m_CSLockerMe.Unlock();
		return RES_UN_DO;
	}

	//���ñ���
	BOOL bRet = m_CSLockerObj.Unlock();

	if (bRet == TRUE)
	{
		m_nLockCount--;
	}

	m_CSLockerMe.Unlock();

	return bRet == TRUE ? RES_SUCCESS : RES_FAILED;
}

//��������
INT CDataLocker::GetLockCount() 
{
	m_CSLockerMe.Lock();
	INT nCount=m_nLockCount;
	m_CSLockerMe.Unlock();

	return nCount; 
}

//�߳�ͬ����
//����
CSynLocker::CSynLocker(void * lpszName, bool bLockAtOnce, bool bInitiallyOwn, bool bManualReset)
:CDataLocker(bLockAtOnce)
{
	LPTSTR pszName = NULL;
	if (lpszName)
	{
		pszName = (LPTSTR)lpszName;
		ZeroMemory(m_tcSynName, sizeof(m_tcSynName));
		memmove(this->m_tcSynName, lpszName, lstrlen(pszName)*sizeof(TCHAR));
	}


	//״̬��ʼ��
	m_nTimOut = RES_FAILED;
	m_nIsBroadCast = CAST_UN_DO;
	m_bIsSinal = false;
	m_bIsOneThreadWaked = false;

	m_nWakeThreadNum = 0;
	m_nCurThreadNum = 0;

	//�¼�
	if (lpszName)
	{
		m_hEvent = CreateEvent(NULL, bManualReset, bInitiallyOwn, m_tcSynName);
	}
	else
	{
		m_hEvent = CreateEvent(NULL, bManualReset, bInitiallyOwn, NULL);	//��������
	}
		

	//if (m_hEvent == NULL) delete this;
}

//����
CSynLocker::~CSynLocker()
{
	try
	{
		if (m_hEvent != NULL) CloseHandle(m_hEvent);

		m_hEvent = NULL;
	}
	catch (...)
	{
	}

	m_hEvent=NULL;
}

//�̵߳ȴ�����
INT CSynLocker::WaitForSingleObject(int nWaitTimeOut)
{
	//�Ƚ�����Դ���ж�֮ǰ�Ƿ������������������������
	//��wait
	//���źŵ����������������ж�wait֮ǰ���޽��������޽������������������ڵ�����
	//�ټ���

	//CWriteLog::writeLog("CSynLocker WaitForSingleObject 1", strlen("CSynLocker WaitForSingleObject 1"), "test.txt");

	bool bIsUnLockedBeforeWait = false;

	if (m_hEvent != NULL)
	{
		while (true)
		{
			if (m_nLockCount>0)
			{
				if (UnLock() == RES_SUCCESS) bIsUnLockedBeforeWait = true;
			}

			DWORD dwRet = ::WaitForSingleObject(m_hEvent, nWaitTimeOut);
			//DWORD dwRet=::WaitForMultipleObjects(1, &m_hEvent, FALSE, nWaitTimeOut);

			if (bIsUnLockedBeforeWait) Lock();

			if (dwRet == WAIT_OBJECT_0)
			{
				bool bBroadCast = false;
				m_ResLocker.Lock();

				if (m_nIsBroadCast == CASTED_ON)
				{
					bBroadCast = true;
					m_nWakeThreadNum++;
					if (m_nWakeThreadNum == m_nCurThreadNum)
					{
						m_nIsBroadCast = CASTED_OFF;
					}
				}

				m_ResLocker.UnLock();

				if (bBroadCast) return RES_SUCCESS;

				//��֤��һ�ź�����
				bool bSinal = false;
				m_ResLocker.Lock();

				if (m_bIsSinal&&m_bIsOneThreadWaked == false)
				{
					m_bIsOneThreadWaked = true;
					m_bIsSinal = false;
					ResetEvent(m_hEvent);
					bSinal = true;
				}
				else
				{
					m_ResLocker.UnLock();
					continue;
					//����˯��
				}

				m_ResLocker.UnLock();

				if (bSinal) return RES_SUCCESS;

				return RES_SUCCESS;
			}

			if (dwRet == WAIT_TIMEOUT)
				m_nTimOut = RES_SUCCESS;
			else
				m_nTimOut = RES_FAILED;

			if (dwRet == WAIT_FAILED) return RES_FAILED;

			break;
		}
	}
	else
	{
		return RES_UN_DO;
	}

	return RES_SUCCESS;
}
	
//�ȴ�ȫ���߳�����
INT CSynLocker::WaitForAllWake(INT *pnCastStatus)
{
//	CWriteLog::writeLog("CSynLocker WaitForAllWake 1", strlen("CSynLocker WaitForAllWake 1"), "test.txt");

	INT nRet=-1;
	while((*pnCastStatus!=CASTED_OFF))
	{
//		CWriteLog::writeLog("CSynLocker WaitForAllWake 2", strlen("CSynLocker WaitForAllWake 2"), "test.txt");

		nRet=WaitForSingleObject(INFINITE);
		if(nRet==RES_FAILED) return RES_FAILED;
		if(nRet!=RES_FAILED) break;

//		CWriteLog::writeLog("CSynLocker WaitForAllWake 3", strlen("CSynLocker WaitForAllWake 3"), "test.txt");
	}

//	CWriteLog::writeLog("CSynLocker WaitForAllWake 4", strlen("CSynLocker WaitForAllWake 4"), "test.txt");

	return RES_SUCCESS;
}

//�̻߳���
INT CSynLocker::Sinal()
{
//	CWriteLog::writeLog("CSynLocker Sinal 1", strlen("CSynLocker Sinal 1"), "test.txt");

	if(m_hEvent!=NULL)
	{
//		CWriteLog::writeLog("CSynLocker Sinal 2", strlen("CSynLocker Sinal 2"), "test.txt");

		m_ResLocker.Lock();

//		CWriteLog::writeLog("CSynLocker Sinal 3", strlen("CSynLocker Sinal 3"), "test.txt");

		BOOL bRet=SetEvent(m_hEvent);
		
//		CWriteLog::writeLog("CSynLocker Sinal 4", strlen("CSynLocker Sinal 4"), "test.txt");

		if(bRet==TRUE)
		{	
			//m_SinalLocker.Lock();
			m_bIsOneThreadWaked=false;
			m_bIsSinal=true;
			//m_SinalLocker.UnLock();
			
//			CWriteLog::writeLog("CSynLocker Sinal 5", strlen("CSynLocker Sinal 5"), "test.txt");
		}
		
		m_ResLocker.UnLock();

//		CWriteLog::writeLog("CSynLocker Sinal 6", strlen("CSynLocker Sinal 6"), "test.txt");

		return bRet==TRUE?RES_SUCCESS:RES_FAILED;
	}
	else
	{
//		CWriteLog::writeLog("CSynLocker Sinal 7", strlen("CSynLocker Sinal 7"), "test.txt");

		return RES_UN_DO;
	}

//	CWriteLog::writeLog("CSynLocker Sinal 8", strlen("CSynLocker Sinal 8"), "test.txt");

	return RES_SUCCESS;
}

//�߳�Ⱥ����
INT CSynLocker::BroadCast(INT nCurThreadNum, INT **pnCastStatus)
{
//	CWriteLog::writeLog("CSynLocker BroadCast 1", strlen("CSynLocker BroadCast 1"), "test.txt");

	if(m_hEvent!=NULL)
	{
//		CWriteLog::writeLog("CSynLocker BroadCast 2", strlen("CSynLocker BroadCast 2"), "test.txt");

		//Ⱥ֪ͨ
		BOOL bRet=PulseEvent(m_hEvent);

//		CWriteLog::writeLog("CSynLocker BroadCast 3", strlen("CSynLocker BroadCast 3"), "test.txt");

		if(bRet==TRUE)
		{
//			CWriteLog::writeLog("CSynLocker BroadCast 4", strlen("CSynLocker BroadCast 4"), "test.txt");

			//����Ⱥ֪ͨ״̬
			//m_BroadCastLocker.Lock();//---�ٽ���
			m_ResLocker.Lock();//---�ٽ���
			m_nCurThreadNum=nCurThreadNum;	//��ǰ�߳���
			m_nIsBroadCast=CASTED_ON;		//״̬
			if(pnCastStatus!=NULL) *pnCastStatus=&m_nIsBroadCast;//����״̬
			//m_BroadCastLocker.UnLock();//---�ٽ���
			m_ResLocker.UnLock();

//			CWriteLog::writeLog("CSynLocker BroadCast 5", strlen("CSynLocker BroadCast 5"), "test.txt");

			//��һ�ź�֪ͨ״̬
			//m_SinalLocker.Lock();//---�ٽ���	
			m_ResLocker.Lock();//---�ٽ���
			m_bIsOneThreadWaked=false;	//һ���߳�����״̬
			m_bIsSinal=false;			//�źŷ���״̬
			//m_SinalLocker.UnLock();//---�ٽ���
			m_ResLocker.UnLock();//---�ٽ���

//			CWriteLog::writeLog("CSynLocker BroadCast 6", strlen("CSynLocker BroadCast 6"), "test.txt");
		}

//		CWriteLog::writeLog("CSynLocker BroadCast 7", strlen("CSynLocker BroadCast 7"), "test.txt");

		return bRet==TRUE?RES_SUCCESS:RES_FAILED;
	}
	else
	{
//		CWriteLog::writeLog("CSynLocker BroadCast 8", strlen("CSynLocker BroadCast 8"), "test.txt");

		return RES_UN_DO;
	}

//	CWriteLog::writeLog("CSynLocker BroadCast 9", strlen("CSynLocker BroadCast 9"), "test.txt");

	return RES_SUCCESS;
}

//��ʱ״̬
INT CSynLocker::GetTimeOut()
{
	return m_nTimOut;
}

//��ȡ�߳�ͬ����ָ��
CSynLocker *CSynLocker::GetThis()									
{
	return this;
}


//�̳߳�

//����
CThreadPool::CThreadPool(int nMinNum, int nMaxNum, LPTSTR lpszName, bool bLockAtOnce, BOOL bInitiallyOwn, BOOL bManualReset)
{
	m_pPool = this;

	//��ʼ������
	m_nMinNum = nMinNum;		//��ʼ�߳���
	m_nMaxNum = nMaxNum;		//����߳���
	m_bIsDestroy = false;				//�̳߳�����
	m_nCurNum = 0;					//��ǰ�߳���
	m_nWaitNum = 0;					//�����߳���
	m_bIsStopAdd = false;				//�Ƿ�ֹͣ��������

	//�̹߳�����ͬ������
	//m_pCSynLockerObj = NULL;
	//m_pCSynLockerObj = new CSynLocker(lpszName, bLockAtOnce, bInitiallyOwn, bManualReset);
	//	if (m_pCSynLockerObj == NULL) delete this;

	//�̻߳���ͬ��
	//m_pRecycleThreadObj = NULL;
	//CString str = TEXT("Recycle");
	//str += lpszName;
	//m_pRecycleThreadObj = new CSynLocker(str.GetBuffer(), false, FALSE, FALSE);
	//if (m_pRecycleThreadObj == NULL) delete this;

	//��ʼ���ֳ�
	this->CreateThread(m_nMinNum);
}

//����
CThreadPool::~CThreadPool()
{
	//�����̣߳������б�������������
	try
	{
		//����У��
		//if (m_pCSynLockerObj == NULL || m_pRecycleThreadObj == NULL) return;

		//���������߳�
		INT ret = m_CSynLockerObj.Lock();	//--�ٽ���	
		StopAdd();							//ֹͣ�������
		this->m_bIsDestroy = true;			//�߳��˳�
		INT *pnCastStatus = NULL;				//���ع㲥״̬

		//��������б�
		list<CTaskBase *>::iterator itTask;
		CTaskBase *pTask = NULL;
		for (itTask = m_listTasklist.begin(); itTask != m_listTasklist.end(); itTask++)
		{
			pTask = *itTask;
			if (pTask)
			{
				delete pTask;
			}
		}

		if (m_listTasklist.empty() != true) m_listTasklist.clear();

		ret = m_CSynLockerObj.UnLock();//--�ٽ���

		//ret = m_CSynLockerObj.BroadCast(m_nCurNum, &pnCastStatus);//�㲥


		//�ȴ��߳�ȫ������
		//	m_CSynLockerObj.Lock();//--�̻߳����ٽ���
		//	m_CSynLockerObj.WaitForAllWake(pnCastStatus);
		//	m_CSynLockerObj.UnLock();//--�̻߳����ٽ���




		//�ͷ��߳���Դ
		/*
		while(!m_listThreadHandle.empty())
		{
		stThreadPara *pObject=m_listThreadHandle.front();
		if(pObject!=NULL)
		{
		CloseHandle(pObject->hObject);
		delete pObject;
		}
		m_listThreadHandle.pop_front();
		}*/
	}
	catch (...)
	{
	}
}

//�����߳�
INT CThreadPool::CreateThread(int nThreadNum)
{
	HANDLE ret;
	//У�����
	if (nThreadNum<0 || nThreadNum>m_nMaxNum) return RES_FAILED;

	//����
	unsigned int uThreadID;

	for (int i = 0; i<nThreadNum; i++)
	{
		//������ʼ��
		ret = (HANDLE)::_beginthreadex(NULL, 0, ThreadHandler, (void *)(this), 0, &uThreadID);	//�����߳�
		CloseHandle(ret);	//�ͷ��ں���Դ

		//�����ж�
		if (ret == INVALID_HANDLE_VALUE)
		{
			if (i == 0)
			{
				return RES_FAILED;
			}
			else
			{
				break;
			}

		}

		this->m_nCurNum++;	//��ǰ�߳���
	}

	return RES_SUCCESS;
}

//�������
INT CThreadPool::AddTask(CTaskBase *pTask, int iThreadPerNum)
{
	INT ret;
	//����У��
	m_CSynLockerObj.Lock();
	if (m_bIsStopAdd == true)
	{
		m_CSynLockerObj.UnLock();
		return RES_FAILED;
	}
	m_CSynLockerObj.UnLock();

	if (iThreadPerNum <= 0) return RES_FAILED;
	if (iThreadPerNum > this->m_nMaxNum) iThreadPerNum = this->m_nMaxNum;
	if (pTask == NULL) return RES_FAILED;

	//�������
	if (!this->m_bIsDestroy)
	{
		ret = m_CSynLockerObj.Lock();//---�ٽ���
		if (ret == RES_FAILED) return RES_FAILED;
		m_listTasklist.push_back(pTask);//�������

		//if (m_nWaitNum>0)
		//{
		//�����߳�
		//	ret = m_CSynLockerObj.UnLock();//---�ٽ���
		//	if (ret == RES_FAILED) return RES_FAILED;
		//	m_CSynLockerObj.Sinal();//�����߳�
		//}
		//else
		{
			//�����߳�
			if (m_nCurNum<m_nMaxNum)
			{
				ret = CreateThread(((iThreadPerNum>(m_nMaxNum - m_nCurNum)) ? (m_nMaxNum - m_nCurNum) : iThreadPerNum));//�����߳�
				if (ret == RES_FAILED)
				{
					ret = m_CSynLockerObj.UnLock();//---�ٽ���
					return RES_FAILED;
				}
			}


			ret = this->m_CSynLockerObj.UnLock();
			if (ret == RES_FAILED) return RES_FAILED;
		}
	}
	else
	{
		return RES_FAILED;
	}

	return RES_SUCCESS;
}

//�̴߳�����
unsigned __stdcall CThreadPool::ThreadHandler(LPVOID pThreadData)
{
	//mfc���̷߳���
	CTaskBase *pTask = NULL;
	CThreadPool *pPool = (CThreadPool *)(pThreadData);

	//����У��
	if (pPool == NULL)
	{
		pPool->m_nCurNum--;
		//��ֹ�߳�
		_endthreadex(0L);
		return 0L;
	}

	try
	{
		//�߳���������
		//while (!pPool->m_bIsDestroy)
		{
			if ((pPool->GetSynLockerObj())->Lock() == RES_FAILED)
			{
				pPool->m_nCurNum--;
				//��ֹ�߳�
				_endthreadex(0L);
				return 0L;
			}

			if (pPool->m_listTasklist.empty() != true)//�ж������Ƿ�Ϊ��
			{
				pTask = pPool->m_listTasklist.front();//ȡ����
				pPool->m_listTasklist.pop_front();//�������unlock֮ǰɾ�������߳��п���ִ�кͱ��߳�һ��������
				if ((pPool->GetSynLockerObj())->UnLock() == RES_FAILED)
				{
					pPool->m_nCurNum--;
					//��ֹ�߳�
					_endthreadex(0L);
					return 0L;
				}

				try
				{
					pTask->Run();	//ִ������
					//delete pTask;//��������
					pTask->Release();
					pTask = NULL;
				}
				catch (...)
				{
				}

			}

			if ((pPool->GetSynLockerObj())->UnLock() == RES_FAILED)
			{
				pPool->m_nCurNum--;
				//��ֹ�߳�
				_endthreadex(0L);
				return 0L;
			}


			//else
			/*{
			pPool->m_nWaitNum++;
			if ((pPool->GetSynLockerObj())->WaitForSingleObject(0) == RES_FAILED)
			{
			if ((pPool->GetSynLockerObj())->UnLock() == RES_FAILED)
			{
			pPool->m_nCurNum--;
			//��ֹ�߳�
			_endthreadex(0L);
			return 0L;
			}
			pPool->m_nCurNum--;
			//��ֹ�߳�
			_endthreadex(0L);
			return 0L;
			}
			pPool->m_nWaitNum--;

			if ((pPool->GetSynLockerObj())->UnLock() == RES_FAILED)
			{
			pPool->m_nCurNum--;
			//��ֹ�߳�
			_endthreadex(0L);
			return 0L;
			}
			}*/
		}
	}
	catch (...)
	{
	}


	//�̳߳��˳�
	(pPool->GetSynLockerObj())->Lock();
	pPool->m_nCurNum--;
	(pPool->GetSynLockerObj())->UnLock();

	//��ֹ�߳�
	_endthreadex(0L);
	return 0L;
}

INT CThreadPool::StopAdd()
{
	m_bIsStopAdd=true;
	return RES_SUCCESS;
}

INT CThreadPool::EnAbleAdd()
{
	m_bIsStopAdd=false;
	return RES_SUCCESS;
}



