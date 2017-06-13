#include"stdafx.h"
#include"TcpServer.h"
#include<errno.h>

#include"Task.h"
//#include"ThreadPool.h"
#include "listenActiveDlg.h"

CTcpServer::CTcpServer()
{
	//读入配置
	//CIniFileIO ServerConfig;
	//ServerConfig.SetFilePath(TEXT(".\\ServerConfig.ini"));
	//TCHAR szAddr[512] = { 0 };
	//ServerConfig.ReadString(TEXT("ServerConfig"), TEXT("IP"), TEXT(""), szAddr, 512);
	
	//char addr[1024] = { 0 };
	//CWriteDailyLog::UnicodeToAnsi(szAddr, addr, 1024);
	//初始化配置
	//m_strServerAddr = "122.114.247.193";
	//m_strServerAddr = addr;

	m_ServerPort = 0;
	//m_ServerPort = ServerConfig.ReadInt(TEXT("ServerConfig"), TEXT("port"), 0);
	m_bIsDomainName = false;
	
	//char rizhi[120] = { 0 };
	//sprintf(rizhi, "ip:%s;port:%d", addr, m_ServerPort);
	//CWriteDailyLog::WriteLog(rizhi, "addr.txt");
	
	//设置密钥
	SetRsaPublicKey((unsigned char *)szClientPublicKey);
	SetRsaPrivateKey((unsigned char *)szServerPrivateKey);

	//异步IO参数
	m_NumOfClntSock = 0;

	m_pMainWind = NULL;
}

CTcpServer:: ~CTcpServer()
{
}

//启动服务器
BOOL CTcpServer::StartServer()
{
	int iAbort = 0;

	string strErr;
	char rizhi[1024] = { 0 };


	try
	{
		//异常1
		iAbort++;

		strErr.append(" CTcpServer::StartServer 开始= StartNetService()=");

		//启动网络服务
		if (!StartNetService()) return FALSE;

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "Bind(addr=%s, addrstrlen=%d, port=%d, AFINET) =\r\n"
			, (char *)m_strServerAddr.c_str(), strlen(m_strServerAddr.c_str()), m_ServerPort);
		strErr.append(rizhi);

		//异常2
		iAbort++;


		//绑定
		if (!Bind((char *)m_strServerAddr.c_str(), strlen(m_strServerAddr.c_str()), m_ServerPort, AF_INET)) return FALSE;

		//异常3
		iAbort++;

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "Listen=");
		strErr.append(rizhi);

		//监听
		if (!Listen()) return FALSE;

		//异常4
		iAbort++;

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "m_nSocket:%d;SetAsynchronousServer=\r\n", m_nSocket);
		strErr.append(rizhi);


		//配置异步IO
		SetAsynchronousServer();

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "WaitForEvent=");
		strErr.append(rizhi);

		//while (1)
	//	{
	//		((CListenActiveDlg *)m_pMainWind)->DispMsg("Accept...\r\n");

			//异常5
	//		iAbort++;

	//		SOCKET ClientSock;
//			if (!Accept(&ClientSock)) ((CListenActiveDlg *)m_pMainWind)->DispMsg("Accept Error...\r\n");

	//		((CListenActiveDlg *)m_pMainWind)->DispMsg("收到一个链接...\r\n");

			//test
		//	char rizhi[100] = { 0 };
			//sprintf(rizhi, "ClientFD:%d\r\n", ClientSock);
		//	((CListenActiveDlg *)m_pMainWind)->DispMsg(rizhi);

			//异常6
			//iAbort++;

			//向线程池添加任务读取客户端传来的数据
			//CTaskBase *pReadTask = new CReadDataTask(m_pMainWind, ClientSock);
		//	CThreadPool::GetInstance()->AddTask(pReadTask);
		//}
	
		WaitForEvent();

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "CTcpServer::StartServer 结束=");
		strErr.append(rizhi);

		//异常5
		iAbort++;

	}
	catch (...)
	{
		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "CTcpServer StartServer:异常：%s\r\n", (char *)strErr.c_str());
		if (m_pMainWind)
			((CListenActiveDlg *)m_pMainWind)->DispMsg(rizhi);
	}


	//启动监视
	//WaitForEvent();

	return TRUE;
}

//关闭服务器
BOOL CTcpServer::CloseServer()
{
	//清理异步io环境
	//断开套接字
	return CloseNetService();
}

//更新配置	(必须先关闭服务器)
BOOL CTcpServer::RefreshConfig()
{
	//if (GetConnectStatus()) return FALSE;

	//读取配置文件

	return TRUE;
}

//设置ip
void CTcpServer::SetIp(string ip)
{
	m_strServerAddr.clear();
	m_strServerAddr = ip;
}

//监听端口
BOOL CTcpServer::Listen(int iBacklog)
{
	int iRet = listen(m_nSocket, iBacklog);
	if (iRet == 0)
	{
		return TRUE;
	}

	return FALSE;
}

//确定链接并获取客户端信息
BOOL CTcpServer::Accept(SOCKET *pSocketFDForClient, CAddr *pRemoteAddr)
{
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	memset(&addr, 0, addrlen);
	SOCKET iAccfd = accept(m_nSocket, (struct sockaddr *)&addr, &addrlen);

	if (iAccfd == -1 && errno == EAGAIN)
	{
		return FALSE;
	}
	else if (iAccfd >= 0)
	{
		if (pRemoteAddr)
			pRemoteAddr->SetAddrInfo(addr);
		if (pSocketFDForClient)
		{
			*pSocketFDForClient = iAccfd;
		}

		return TRUE;
	}
	else
	{
		//perror("cTcpServer::Accept -accept error");
	}

	return FALSE;

}

//获取数据
BOOL CTcpServer::ReadDataFromClient(void **pData, long *lDataLen)
{
	if (!RecvDataFromSrc()) return FALSE;

	//if (m_pMainWind)
	//	((CListenActiveDlg *)m_pMainWind)->DispMsg("RecvDataFrom Src 成功\r\n");

	//AfxMessageBox(TEXT(" ReadDataFromClient 01"));
	return GetOutRecvData(pData, lDataLen);
}

//发送数据
BOOL CTcpServer::SendDataToClient(void *pData, long lDataLen)
{
	if (!AddDataToSend(pData, lDataLen)) return FALSE;

	return SendDataToDest();
}

//配置异步IO模型
BOOL CTcpServer::SetAsynchronousServer()
{
	m_newEvent = WSACreateEvent();

	if (WSAEventSelect(m_nSocket, m_newEvent, FD_ACCEPT | FD_CLOSE | FD_READ) == SOCKET_ERROR)
		return FALSE;

	m_hSockArr[m_NumOfClntSock] = m_nSocket;
	m_hEventArr[m_NumOfClntSock] = m_newEvent;

	m_NumOfClntSock++;
}

//添加监控事件
BOOL CTcpServer::AddEvent(SOCKET hClntSock)
{
	m_newEvent = WSACreateEvent();
	WSAEventSelect(hClntSock, m_newEvent, FD_READ | FD_CLOSE);

	m_hEventArr[m_NumOfClntSock] = m_newEvent;
	m_hSockArr[m_NumOfClntSock] = hClntSock;

	m_NumOfClntSock++;
	return TRUE;
}


//绑定地址
BOOL CTcpServer::Bind(char *ip, int iplen, unsigned short port, unsigned short family)
{
	//参数校验
	if (ip == NULL) return FALSE;

	m_SocketAddr.SetAddrInfo(ip, port, family);

	int ret = bind(m_nSocket, m_SocketAddr.GetpAddrInfo(), *(m_SocketAddr.GetSocketInfoLenPoint()));
	if (ret == 0)
	{
		return TRUE;
	}
	return FALSE;

}

//调整socket数组
void CTcpServer::CompressSockets(SOCKET hSockArr[], int idx, int total)
{
	int i;
	for (i = idx; i < total; i++)
	{
		hSockArr[i] = hSockArr[i + 1];
	}
}

//调整事件数组
void CTcpServer::CompressEvents(WSAEVENT hEventArr[], int idx, int total)
{
	int i;
	for (i = idx; i < total; i++)
	{
		hEventArr[i] = hEventArr[i + 1];
	}
}

//监视IO事件
BOOL CTcpServer::WaitForEvent()
{
	//变量定义
	int posInfo, startIdx, i, clntAdrLen;
	SOCKET hClntSock;

	char LogMonitor[256] = { 0 };

	//设置监视套接字
	if (m_pMainWind)
	{
		((CListenActiveDlg *)m_pMainWind)->SetMinitorSocke(m_nSocket);
		((CListenActiveDlg *)m_pMainWind)->SetMonitorEvent(m_newEvent);
	}
		
	memset(LogMonitor, 0, sizeof(LogMonitor));
	sprintf(LogMonitor, "启动异步监视器...\r\n");
	if (m_pMainWind)
		((CListenActiveDlg *)m_pMainWind)->DispMsg(LogMonitor);

	//设置服务器运行使能
	if (m_pMainWind)
		((CListenActiveDlg *)m_pMainWind)->SetServerRunFlag(TRUE);

	//设置服务器运行使能
	if (m_pMainWind)
		((CListenActiveDlg *)m_pMainWind)->RegEnabelAnalys();	//注册开启/关闭

	string strErr;
	char rizhi[1024] = { 0 };

	
	try
	{
		bool bMonitor = true;
		while (((CListenActiveDlg *)m_pMainWind)->GetMonitor())
		{
			strErr.clear();

			memset(rizhi, 0, sizeof(rizhi));
			sprintf(rizhi, "WaitForEvent 开始=WSAWaitForMultipleEvents(m_NumOfClntSock=%d, )=\r\n", m_NumOfClntSock);
			strErr.append(rizhi);

			//if (m_pMainWind)
			//	((CListenActiveDlg *)m_pMainWind)->DispMsg("WaitForEvent WaitforMultipleEvents\r\n");
			posInfo = WSAWaitForMultipleEvents(m_NumOfClntSock, m_hEventArr, FALSE, WSA_INFINITE, FALSE);
			startIdx = posInfo - WSA_WAIT_EVENT_0;

			memset(rizhi, 0, sizeof(rizhi));
			sprintf(rizhi, "startIdx=%d =", startIdx);
			strErr.append(rizhi);

			//判断服务是否中断
			if (((CListenActiveDlg *)m_pMainWind)->GetMonitor() == false) break;

			memset(rizhi, 0, sizeof(rizhi));
			sprintf(rizhi, "GetMonitor:%s =", ((CListenActiveDlg *)m_pMainWind)->GetMonitor()?"监视":"关闭监视");
			strErr.append(rizhi);

			for (i = startIdx; i < m_NumOfClntSock; i++)
			{
				int sigEventIdx = WSAWaitForMultipleEvents(1, &m_hEventArr[i], TRUE, 0, FALSE);

				if ((sigEventIdx == WSA_WAIT_FAILED || sigEventIdx == WSA_WAIT_TIMEOUT))
				{
					continue;
				}
				else
				{
					sigEventIdx = i;
					WSAEnumNetworkEvents(m_hSockArr[sigEventIdx], m_hEventArr[sigEventIdx], &m_netEvents);

					//链接事件
					if (m_netEvents.lNetworkEvents&FD_ACCEPT)//新链接
					{
						if (m_netEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
						{
							//ACCEPT ERROR
							break;
						}

					//	if (m_pMainWind)
					//		((CListenActiveDlg *)m_pMainWind)->DispMsg("WaitForEvent a new accept\r\n");

						Accept(&hClntSock);

						//添加事件
						if (m_NumOfClntSock <= WSA_MAXIMUM_WAIT_EVENTS)
						{

						//	memset(LogMonitor, 0, sizeof(LogMonitor));
						//	sprintf(LogMonitor, "当前连接数：%d\r\n", m_NumOfClntSock);
						//	if (m_pMainWind)
						//		((CListenActiveDlg *)m_pMainWind)->DispMsg(LogMonitor);
							//添加事件监视
							AddEvent(hClntSock);
						}
						else
						{
							memset(LogMonitor, 0, sizeof(LogMonitor));
							sprintf(LogMonitor, "当前连接数：%d\r\n", m_NumOfClntSock);
							if (m_pMainWind)
								((CListenActiveDlg *)m_pMainWind)->DispMsg(LogMonitor);

							if (m_pMainWind)
								((CListenActiveDlg *)m_pMainWind)->DispMsg("WaitForEvent over WSA_MAXIMUM_WAIT_EVENTS\r\n");
							//创建线程添加监视
						}
					}

					//可读事件
					if (m_netEvents.lNetworkEvents&FD_READ)
					{
						if (m_netEvents.iErrorCode[FD_READ_BIT] != 0)
						{
							//READ ERROR
							break;
						}

					//	if (m_pMainWind)
					//		((CListenActiveDlg *)m_pMainWind)->DispMsg("WaitForEvent Read task\r\n");

						//向线程池添加任务读取客户端传来的数据
						CTaskBase *pReadTask = new CReadDataTask(m_pMainWind, m_hSockArr[sigEventIdx]);
						CThreadPool::GetInstance()->AddTask(pReadTask);

						//移除
						WSACloseEvent(m_hEventArr[sigEventIdx]);
						//closesocket(m_hSockArr[sigEventIdx]);

						m_NumOfClntSock--;
						CompressSockets(m_hSockArr, sigEventIdx, m_NumOfClntSock);
						CompressEvents(m_hEventArr, sigEventIdx, m_NumOfClntSock);

					//	if (m_pMainWind)
					//		((CListenActiveDlg *)m_pMainWind)->DispMsg("Read over WSACloseEvent a sock\r\n");
					}

					//关闭事件
					if (m_netEvents.lNetworkEvents&FD_CLOSE)
					{
						if (m_netEvents.iErrorCode[FD_CLOSE_BIT] != 0)
						{
							//CLOSE ERROR
							break;
						}

					//	if (m_pMainWind)
					//		((CListenActiveDlg *)m_pMainWind)->DispMsg("WaitForEvent Close a Sock\r\n");

						//Sleep(2000);

						WSACloseEvent(m_hEventArr[sigEventIdx]);
						//closesocket(m_hSockArr[sigEventIdx]);

						m_NumOfClntSock--;
						CompressSockets(m_hSockArr, sigEventIdx, m_NumOfClntSock);
						CompressEvents(m_hEventArr, sigEventIdx, m_NumOfClntSock);

						if (m_nSocket == m_hSockArr[sigEventIdx])
						{
							bMonitor = false;
							break;
						}
					}//end of 关闭事件

				}//end of if 
			}//end of for

			memset(rizhi, 0, sizeof(rizhi));
			sprintf(rizhi, "结束一个while =\r\n");
			strErr.append(rizhi);
		}//end of while

		//设置服务器运行使能
		if (m_pMainWind)
			((CListenActiveDlg *)m_pMainWind)->SetServerRunFlag(FALSE);

		if (m_pMainWind)
			((CListenActiveDlg *)m_pMainWind)->DispMsg("已停止监视客户端...\r\n");


		if (m_pMainWind)
			((CListenActiveDlg *)m_pMainWind)->DispMsg("服务已停止............\r\n");
	}
	catch (...)
	{
		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, " CTcpServer::WaitForEvent 异常报告：%s\r\n", strErr.c_str());
		if (m_pMainWind)
			((CListenActiveDlg *)m_pMainWind)->DispMsg(rizhi);
	}

	

	return TRUE;
}