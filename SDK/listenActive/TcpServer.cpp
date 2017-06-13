#include"stdafx.h"
#include"TcpServer.h"
#include<errno.h>

#include"Task.h"
//#include"ThreadPool.h"
#include "listenActiveDlg.h"

CTcpServer::CTcpServer()
{
	//��������
	//CIniFileIO ServerConfig;
	//ServerConfig.SetFilePath(TEXT(".\\ServerConfig.ini"));
	//TCHAR szAddr[512] = { 0 };
	//ServerConfig.ReadString(TEXT("ServerConfig"), TEXT("IP"), TEXT(""), szAddr, 512);
	
	//char addr[1024] = { 0 };
	//CWriteDailyLog::UnicodeToAnsi(szAddr, addr, 1024);
	//��ʼ������
	//m_strServerAddr = "122.114.247.193";
	//m_strServerAddr = addr;

	m_ServerPort = 0;
	//m_ServerPort = ServerConfig.ReadInt(TEXT("ServerConfig"), TEXT("port"), 0);
	m_bIsDomainName = false;
	
	//char rizhi[120] = { 0 };
	//sprintf(rizhi, "ip:%s;port:%d", addr, m_ServerPort);
	//CWriteDailyLog::WriteLog(rizhi, "addr.txt");
	
	//������Կ
	SetRsaPublicKey((unsigned char *)szClientPublicKey);
	SetRsaPrivateKey((unsigned char *)szServerPrivateKey);

	//�첽IO����
	m_NumOfClntSock = 0;

	m_pMainWind = NULL;
}

CTcpServer:: ~CTcpServer()
{
}

//����������
BOOL CTcpServer::StartServer()
{
	int iAbort = 0;

	string strErr;
	char rizhi[1024] = { 0 };


	try
	{
		//�쳣1
		iAbort++;

		strErr.append(" CTcpServer::StartServer ��ʼ= StartNetService()=");

		//�����������
		if (!StartNetService()) return FALSE;

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "Bind(addr=%s, addrstrlen=%d, port=%d, AFINET) =\r\n"
			, (char *)m_strServerAddr.c_str(), strlen(m_strServerAddr.c_str()), m_ServerPort);
		strErr.append(rizhi);

		//�쳣2
		iAbort++;


		//��
		if (!Bind((char *)m_strServerAddr.c_str(), strlen(m_strServerAddr.c_str()), m_ServerPort, AF_INET)) return FALSE;

		//�쳣3
		iAbort++;

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "Listen=");
		strErr.append(rizhi);

		//����
		if (!Listen()) return FALSE;

		//�쳣4
		iAbort++;

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "m_nSocket:%d;SetAsynchronousServer=\r\n", m_nSocket);
		strErr.append(rizhi);


		//�����첽IO
		SetAsynchronousServer();

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "WaitForEvent=");
		strErr.append(rizhi);

		//while (1)
	//	{
	//		((CListenActiveDlg *)m_pMainWind)->DispMsg("Accept...\r\n");

			//�쳣5
	//		iAbort++;

	//		SOCKET ClientSock;
//			if (!Accept(&ClientSock)) ((CListenActiveDlg *)m_pMainWind)->DispMsg("Accept Error...\r\n");

	//		((CListenActiveDlg *)m_pMainWind)->DispMsg("�յ�һ������...\r\n");

			//test
		//	char rizhi[100] = { 0 };
			//sprintf(rizhi, "ClientFD:%d\r\n", ClientSock);
		//	((CListenActiveDlg *)m_pMainWind)->DispMsg(rizhi);

			//�쳣6
			//iAbort++;

			//���̳߳���������ȡ�ͻ��˴���������
			//CTaskBase *pReadTask = new CReadDataTask(m_pMainWind, ClientSock);
		//	CThreadPool::GetInstance()->AddTask(pReadTask);
		//}
	
		WaitForEvent();

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "CTcpServer::StartServer ����=");
		strErr.append(rizhi);

		//�쳣5
		iAbort++;

	}
	catch (...)
	{
		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "CTcpServer StartServer:�쳣��%s\r\n", (char *)strErr.c_str());
		if (m_pMainWind)
			((CListenActiveDlg *)m_pMainWind)->DispMsg(rizhi);
	}


	//��������
	//WaitForEvent();

	return TRUE;
}

//�رշ�����
BOOL CTcpServer::CloseServer()
{
	//�����첽io����
	//�Ͽ��׽���
	return CloseNetService();
}

//��������	(�����ȹرշ�����)
BOOL CTcpServer::RefreshConfig()
{
	//if (GetConnectStatus()) return FALSE;

	//��ȡ�����ļ�

	return TRUE;
}

//����ip
void CTcpServer::SetIp(string ip)
{
	m_strServerAddr.clear();
	m_strServerAddr = ip;
}

//�����˿�
BOOL CTcpServer::Listen(int iBacklog)
{
	int iRet = listen(m_nSocket, iBacklog);
	if (iRet == 0)
	{
		return TRUE;
	}

	return FALSE;
}

//ȷ�����Ӳ���ȡ�ͻ�����Ϣ
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

//��ȡ����
BOOL CTcpServer::ReadDataFromClient(void **pData, long *lDataLen)
{
	if (!RecvDataFromSrc()) return FALSE;

	//if (m_pMainWind)
	//	((CListenActiveDlg *)m_pMainWind)->DispMsg("RecvDataFrom Src �ɹ�\r\n");

	//AfxMessageBox(TEXT(" ReadDataFromClient 01"));
	return GetOutRecvData(pData, lDataLen);
}

//��������
BOOL CTcpServer::SendDataToClient(void *pData, long lDataLen)
{
	if (!AddDataToSend(pData, lDataLen)) return FALSE;

	return SendDataToDest();
}

//�����첽IOģ��
BOOL CTcpServer::SetAsynchronousServer()
{
	m_newEvent = WSACreateEvent();

	if (WSAEventSelect(m_nSocket, m_newEvent, FD_ACCEPT | FD_CLOSE | FD_READ) == SOCKET_ERROR)
		return FALSE;

	m_hSockArr[m_NumOfClntSock] = m_nSocket;
	m_hEventArr[m_NumOfClntSock] = m_newEvent;

	m_NumOfClntSock++;
}

//��Ӽ���¼�
BOOL CTcpServer::AddEvent(SOCKET hClntSock)
{
	m_newEvent = WSACreateEvent();
	WSAEventSelect(hClntSock, m_newEvent, FD_READ | FD_CLOSE);

	m_hEventArr[m_NumOfClntSock] = m_newEvent;
	m_hSockArr[m_NumOfClntSock] = hClntSock;

	m_NumOfClntSock++;
	return TRUE;
}


//�󶨵�ַ
BOOL CTcpServer::Bind(char *ip, int iplen, unsigned short port, unsigned short family)
{
	//����У��
	if (ip == NULL) return FALSE;

	m_SocketAddr.SetAddrInfo(ip, port, family);

	int ret = bind(m_nSocket, m_SocketAddr.GetpAddrInfo(), *(m_SocketAddr.GetSocketInfoLenPoint()));
	if (ret == 0)
	{
		return TRUE;
	}
	return FALSE;

}

//����socket����
void CTcpServer::CompressSockets(SOCKET hSockArr[], int idx, int total)
{
	int i;
	for (i = idx; i < total; i++)
	{
		hSockArr[i] = hSockArr[i + 1];
	}
}

//�����¼�����
void CTcpServer::CompressEvents(WSAEVENT hEventArr[], int idx, int total)
{
	int i;
	for (i = idx; i < total; i++)
	{
		hEventArr[i] = hEventArr[i + 1];
	}
}

//����IO�¼�
BOOL CTcpServer::WaitForEvent()
{
	//��������
	int posInfo, startIdx, i, clntAdrLen;
	SOCKET hClntSock;

	char LogMonitor[256] = { 0 };

	//���ü����׽���
	if (m_pMainWind)
	{
		((CListenActiveDlg *)m_pMainWind)->SetMinitorSocke(m_nSocket);
		((CListenActiveDlg *)m_pMainWind)->SetMonitorEvent(m_newEvent);
	}
		
	memset(LogMonitor, 0, sizeof(LogMonitor));
	sprintf(LogMonitor, "�����첽������...\r\n");
	if (m_pMainWind)
		((CListenActiveDlg *)m_pMainWind)->DispMsg(LogMonitor);

	//���÷���������ʹ��
	if (m_pMainWind)
		((CListenActiveDlg *)m_pMainWind)->SetServerRunFlag(TRUE);

	//���÷���������ʹ��
	if (m_pMainWind)
		((CListenActiveDlg *)m_pMainWind)->RegEnabelAnalys();	//ע�Ὺ��/�ر�

	string strErr;
	char rizhi[1024] = { 0 };

	
	try
	{
		bool bMonitor = true;
		while (((CListenActiveDlg *)m_pMainWind)->GetMonitor())
		{
			strErr.clear();

			memset(rizhi, 0, sizeof(rizhi));
			sprintf(rizhi, "WaitForEvent ��ʼ=WSAWaitForMultipleEvents(m_NumOfClntSock=%d, )=\r\n", m_NumOfClntSock);
			strErr.append(rizhi);

			//if (m_pMainWind)
			//	((CListenActiveDlg *)m_pMainWind)->DispMsg("WaitForEvent WaitforMultipleEvents\r\n");
			posInfo = WSAWaitForMultipleEvents(m_NumOfClntSock, m_hEventArr, FALSE, WSA_INFINITE, FALSE);
			startIdx = posInfo - WSA_WAIT_EVENT_0;

			memset(rizhi, 0, sizeof(rizhi));
			sprintf(rizhi, "startIdx=%d =", startIdx);
			strErr.append(rizhi);

			//�жϷ����Ƿ��ж�
			if (((CListenActiveDlg *)m_pMainWind)->GetMonitor() == false) break;

			memset(rizhi, 0, sizeof(rizhi));
			sprintf(rizhi, "GetMonitor:%s =", ((CListenActiveDlg *)m_pMainWind)->GetMonitor()?"����":"�رռ���");
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

					//�����¼�
					if (m_netEvents.lNetworkEvents&FD_ACCEPT)//������
					{
						if (m_netEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
						{
							//ACCEPT ERROR
							break;
						}

					//	if (m_pMainWind)
					//		((CListenActiveDlg *)m_pMainWind)->DispMsg("WaitForEvent a new accept\r\n");

						Accept(&hClntSock);

						//����¼�
						if (m_NumOfClntSock <= WSA_MAXIMUM_WAIT_EVENTS)
						{

						//	memset(LogMonitor, 0, sizeof(LogMonitor));
						//	sprintf(LogMonitor, "��ǰ��������%d\r\n", m_NumOfClntSock);
						//	if (m_pMainWind)
						//		((CListenActiveDlg *)m_pMainWind)->DispMsg(LogMonitor);
							//����¼�����
							AddEvent(hClntSock);
						}
						else
						{
							memset(LogMonitor, 0, sizeof(LogMonitor));
							sprintf(LogMonitor, "��ǰ��������%d\r\n", m_NumOfClntSock);
							if (m_pMainWind)
								((CListenActiveDlg *)m_pMainWind)->DispMsg(LogMonitor);

							if (m_pMainWind)
								((CListenActiveDlg *)m_pMainWind)->DispMsg("WaitForEvent over WSA_MAXIMUM_WAIT_EVENTS\r\n");
							//�����߳���Ӽ���
						}
					}

					//�ɶ��¼�
					if (m_netEvents.lNetworkEvents&FD_READ)
					{
						if (m_netEvents.iErrorCode[FD_READ_BIT] != 0)
						{
							//READ ERROR
							break;
						}

					//	if (m_pMainWind)
					//		((CListenActiveDlg *)m_pMainWind)->DispMsg("WaitForEvent Read task\r\n");

						//���̳߳���������ȡ�ͻ��˴���������
						CTaskBase *pReadTask = new CReadDataTask(m_pMainWind, m_hSockArr[sigEventIdx]);
						CThreadPool::GetInstance()->AddTask(pReadTask);

						//�Ƴ�
						WSACloseEvent(m_hEventArr[sigEventIdx]);
						//closesocket(m_hSockArr[sigEventIdx]);

						m_NumOfClntSock--;
						CompressSockets(m_hSockArr, sigEventIdx, m_NumOfClntSock);
						CompressEvents(m_hEventArr, sigEventIdx, m_NumOfClntSock);

					//	if (m_pMainWind)
					//		((CListenActiveDlg *)m_pMainWind)->DispMsg("Read over WSACloseEvent a sock\r\n");
					}

					//�ر��¼�
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
					}//end of �ر��¼�

				}//end of if 
			}//end of for

			memset(rizhi, 0, sizeof(rizhi));
			sprintf(rizhi, "����һ��while =\r\n");
			strErr.append(rizhi);
		}//end of while

		//���÷���������ʹ��
		if (m_pMainWind)
			((CListenActiveDlg *)m_pMainWind)->SetServerRunFlag(FALSE);

		if (m_pMainWind)
			((CListenActiveDlg *)m_pMainWind)->DispMsg("��ֹͣ���ӿͻ���...\r\n");


		if (m_pMainWind)
			((CListenActiveDlg *)m_pMainWind)->DispMsg("������ֹͣ............\r\n");
	}
	catch (...)
	{
		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, " CTcpServer::WaitForEvent �쳣���棺%s\r\n", strErr.c_str());
		if (m_pMainWind)
			((CListenActiveDlg *)m_pMainWind)->DispMsg(rizhi);
	}

	

	return TRUE;
}