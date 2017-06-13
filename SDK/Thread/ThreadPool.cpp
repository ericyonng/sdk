#include "stdafx.h"

#include "ThreadPool.h"
#include<string.h>
//#include"WriteLog.h"

static int ThreadID_i=0;

CThreadPool *CThreadPool::m_pPool = NULL;

//线程池驻留内存
CThreadPool GlobalThreadPool(0, 200);//初始化5个线程，最大限制200个线程

//共享资源锁
//构造函数
CDataLocker::CDataLocker(bool bLockAtOnce) 
{
	//设置变量
	m_nLockCount=0;

	return;
}

//析构函数
CDataLocker::~CDataLocker()
{
	//CWriteLog::writeLog("CDataLocker ~CDataLocker 1", strlen("CDataLocker ~CDataLocker 1"), "test.txt");

	//解除锁定
	while (m_nLockCount>0)
	{

		//CWriteLog::writeLog("CDataLocker ~CDataLocker 2", strlen("CDataLocker ~CDataLocker 2"), "test.txt");

		if(UnLock()!=RES_SUCCESS)  break;

		//CWriteLog::writeLog("CDataLocker ~CDataLocker 3", strlen("CDataLocker ~CDataLocker 3"), "test.txt");

	}

	//CWriteLog::writeLog("CDataLocker ~CDataLocker 4", strlen("CDataLocker ~CDataLocker 4"), "test.txt");

	return;
}

//锁定函数
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

//解锁函数
INT CDataLocker::UnLock()
{
	//效验状态
	m_CSLockerMe.Lock();
	if (m_nLockCount <= 0)
	{
		m_CSLockerMe.Unlock();
		return RES_UN_DO;
	}

	//设置变量
	BOOL bRet = m_CSLockerObj.Unlock();

	if (bRet == TRUE)
	{
		m_nLockCount--;
	}

	m_CSLockerMe.Unlock();

	return bRet == TRUE ? RES_SUCCESS : RES_FAILED;
}

//锁定次数
INT CDataLocker::GetLockCount() 
{
	m_CSLockerMe.Lock();
	INT nCount=m_nLockCount;
	m_CSLockerMe.Unlock();

	return nCount; 
}

//线程同步类
//构造
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


	//状态初始化
	m_nTimOut = RES_FAILED;
	m_nIsBroadCast = CAST_UN_DO;
	m_bIsSinal = false;
	m_bIsOneThreadWaked = false;

	m_nWakeThreadNum = 0;
	m_nCurThreadNum = 0;

	//事件
	if (lpszName)
	{
		m_hEvent = CreateEvent(NULL, bManualReset, bInitiallyOwn, m_tcSynName);
	}
	else
	{
		m_hEvent = CreateEvent(NULL, bManualReset, bInitiallyOwn, NULL);	//无名对象
	}
		

	//if (m_hEvent == NULL) delete this;
}

//析构
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

//线程等待阻塞
INT CSynLocker::WaitForSingleObject(int nWaitTimeOut)
{
	//先解锁资源（判断之前是否上锁若无上锁则无需解锁）
	//再wait
	//有信号到来先重新上锁（判断wait之前有无解锁，若无解锁则无需上锁，用于抵消）
	//再继续

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

				//保证单一信号醒来
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
					//返回睡觉
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
	
//等待全部线程苏醒
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

//线程唤醒
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

//线程群唤醒
INT CSynLocker::BroadCast(INT nCurThreadNum, INT **pnCastStatus)
{
//	CWriteLog::writeLog("CSynLocker BroadCast 1", strlen("CSynLocker BroadCast 1"), "test.txt");

	if(m_hEvent!=NULL)
	{
//		CWriteLog::writeLog("CSynLocker BroadCast 2", strlen("CSynLocker BroadCast 2"), "test.txt");

		//群通知
		BOOL bRet=PulseEvent(m_hEvent);

//		CWriteLog::writeLog("CSynLocker BroadCast 3", strlen("CSynLocker BroadCast 3"), "test.txt");

		if(bRet==TRUE)
		{
//			CWriteLog::writeLog("CSynLocker BroadCast 4", strlen("CSynLocker BroadCast 4"), "test.txt");

			//保护群通知状态
			//m_BroadCastLocker.Lock();//---临界区
			m_ResLocker.Lock();//---临界区
			m_nCurThreadNum=nCurThreadNum;	//当前线程数
			m_nIsBroadCast=CASTED_ON;		//状态
			if(pnCastStatus!=NULL) *pnCastStatus=&m_nIsBroadCast;//带出状态
			//m_BroadCastLocker.UnLock();//---临界区
			m_ResLocker.UnLock();

//			CWriteLog::writeLog("CSynLocker BroadCast 5", strlen("CSynLocker BroadCast 5"), "test.txt");

			//单一信号通知状态
			//m_SinalLocker.Lock();//---临界区	
			m_ResLocker.Lock();//---临界区
			m_bIsOneThreadWaked=false;	//一个线程苏醒状态
			m_bIsSinal=false;			//信号发送状态
			//m_SinalLocker.UnLock();//---临界区
			m_ResLocker.UnLock();//---临界区

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

//超时状态
INT CSynLocker::GetTimeOut()
{
	return m_nTimOut;
}

//获取线程同步类指针
CSynLocker *CSynLocker::GetThis()									
{
	return this;
}


//线程池

//构造
CThreadPool::CThreadPool(int nMinNum, int nMaxNum, LPTSTR lpszName, bool bLockAtOnce, BOOL bInitiallyOwn, BOOL bManualReset)
{
	m_pPool = this;

	//初始化参数
	m_nMinNum = nMinNum;		//初始线程数
	m_nMaxNum = nMaxNum;		//最大线程数
	m_bIsDestroy = false;				//线程池销毁
	m_nCurNum = 0;					//当前线程数
	m_nWaitNum = 0;					//空闲线程数
	m_bIsStopAdd = false;				//是否停止增加任务

	//线程共享区同步锁类
	//m_pCSynLockerObj = NULL;
	//m_pCSynLockerObj = new CSynLocker(lpszName, bLockAtOnce, bInitiallyOwn, bManualReset);
	//	if (m_pCSynLockerObj == NULL) delete this;

	//线程回收同步
	//m_pRecycleThreadObj = NULL;
	//CString str = TEXT("Recycle");
	//str += lpszName;
	//m_pRecycleThreadObj = new CSynLocker(str.GetBuffer(), false, FALSE, FALSE);
	//if (m_pRecycleThreadObj == NULL) delete this;

	//初始化现场
	this->CreateThread(m_nMinNum);
}

//析构
CThreadPool::~CThreadPool()
{
	//销毁线程，任务列表，销毁条件变量
	try
	{
		//参数校验
		//if (m_pCSynLockerObj == NULL || m_pRecycleThreadObj == NULL) return;

		//唤醒所有线程
		INT ret = m_CSynLockerObj.Lock();	//--临界区	
		StopAdd();							//停止任务添加
		this->m_bIsDestroy = true;			//线程退出
		INT *pnCastStatus = NULL;				//返回广播状态

		//清空任务列表
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

		ret = m_CSynLockerObj.UnLock();//--临界区

		//ret = m_CSynLockerObj.BroadCast(m_nCurNum, &pnCastStatus);//广播


		//等待线程全部结束
		//	m_CSynLockerObj.Lock();//--线程回收临界区
		//	m_CSynLockerObj.WaitForAllWake(pnCastStatus);
		//	m_CSynLockerObj.UnLock();//--线程回收临界区




		//释放线程资源
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

//创建线程
INT CThreadPool::CreateThread(int nThreadNum)
{
	HANDLE ret;
	//校验参数
	if (nThreadNum<0 || nThreadNum>m_nMaxNum) return RES_FAILED;

	//参数
	unsigned int uThreadID;

	for (int i = 0; i<nThreadNum; i++)
	{
		//参数初始化
		ret = (HANDLE)::_beginthreadex(NULL, 0, ThreadHandler, (void *)(this), 0, &uThreadID);	//创建线程
		CloseHandle(ret);	//释放内核资源

		//错误判断
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

		this->m_nCurNum++;	//当前线程数
	}

	return RES_SUCCESS;
}

//添加任务
INT CThreadPool::AddTask(CTaskBase *pTask, int iThreadPerNum)
{
	INT ret;
	//参数校验
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

	//添加任务
	if (!this->m_bIsDestroy)
	{
		ret = m_CSynLockerObj.Lock();//---临界区
		if (ret == RES_FAILED) return RES_FAILED;
		m_listTasklist.push_back(pTask);//添加任务

		//if (m_nWaitNum>0)
		//{
		//唤醒线程
		//	ret = m_CSynLockerObj.UnLock();//---临界区
		//	if (ret == RES_FAILED) return RES_FAILED;
		//	m_CSynLockerObj.Sinal();//唤醒线程
		//}
		//else
		{
			//创建线程
			if (m_nCurNum<m_nMaxNum)
			{
				ret = CreateThread(((iThreadPerNum>(m_nMaxNum - m_nCurNum)) ? (m_nMaxNum - m_nCurNum) : iThreadPerNum));//创建线程
				if (ret == RES_FAILED)
				{
					ret = m_CSynLockerObj.UnLock();//---临界区
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

//线程处理函数
unsigned __stdcall CThreadPool::ThreadHandler(LPVOID pThreadData)
{
	//mfc无线程分离
	CTaskBase *pTask = NULL;
	CThreadPool *pPool = (CThreadPool *)(pThreadData);

	//参数校验
	if (pPool == NULL)
	{
		pPool->m_nCurNum--;
		//中止线程
		_endthreadex(0L);
		return 0L;
	}

	try
	{
		//线程启动处理
		//while (!pPool->m_bIsDestroy)
		{
			if ((pPool->GetSynLockerObj())->Lock() == RES_FAILED)
			{
				pPool->m_nCurNum--;
				//中止线程
				_endthreadex(0L);
				return 0L;
			}

			if (pPool->m_listTasklist.empty() != true)//判断任务是否为空
			{
				pTask = pPool->m_listTasklist.front();//取任务
				pPool->m_listTasklist.pop_front();//如果不在unlock之前删除其他线程有可能执行和本线程一样的任务
				if ((pPool->GetSynLockerObj())->UnLock() == RES_FAILED)
				{
					pPool->m_nCurNum--;
					//中止线程
					_endthreadex(0L);
					return 0L;
				}

				try
				{
					pTask->Run();	//执行任务
					//delete pTask;//结束任务
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
				//中止线程
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
			//中止线程
			_endthreadex(0L);
			return 0L;
			}
			pPool->m_nCurNum--;
			//中止线程
			_endthreadex(0L);
			return 0L;
			}
			pPool->m_nWaitNum--;

			if ((pPool->GetSynLockerObj())->UnLock() == RES_FAILED)
			{
			pPool->m_nCurNum--;
			//中止线程
			_endthreadex(0L);
			return 0L;
			}
			}*/
		}
	}
	catch (...)
	{
	}


	//线程池退出
	(pPool->GetSynLockerObj())->Lock();
	pPool->m_nCurNum--;
	(pPool->GetSynLockerObj())->UnLock();

	//中止线程
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



