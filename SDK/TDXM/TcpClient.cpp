#include"stdafx.h"
#include"TcpClient.h"
#include"TDXMWnd.h"

CTcpClient::CTcpClient()
{
	//��ʼ������
	//m_strServerAddr = "122.114.247.193";
//	m_ServerPort = 8531;
	m_strServerAddr = CTDXMFrame::GetInstance()->m_strServerAddr;
	m_ServerPort = CTDXMFrame::GetInstance()->m_ServerPort;
	m_bIsDomainName = true;

	//������Կ
	SetRsaPublicKey((unsigned char *)szServerPublicKey);
	SetRsaPrivateKey((unsigned char *)szClientPrivateKey);
}

CTcpClient::~CTcpClient()
{
}


//���ӷ�����
BOOL CTcpClient::Link(char *pServerAddr, unsigned short port, bool bClearDataCache)
{
	//����У��
	if (pServerAddr == NULL) return FALSE;

	//�ж��Ƿ������� �����ӶϿ�����
	//if (GetNetConnectStatus())
	//{
	//	CloseNetService();
	//}

	//��ջ�����
	if (bClearDataCache) ClearAllDataCache();

	//���õ�ַ
	if (!SetAddrInfo(pServerAddr, strlen(pServerAddr), port, AF_INET))
	{
		return FALSE;
	}

	return Connect();
}

//���ӷ�����
BOOL CTcpClient::ConnectServer()
{
	//�����������
	if (!StartNetService()) return FALSE;

	//���ӷ�����
	BOOL bConnectServer = FALSE;
	if (m_bIsDomainName)
	{
		bConnectServer = Link((char *)m_strServerAddr.c_str(), m_ServerPort);

	//	if (bConnectServer) AfxMessageBox(TEXT("���ӳɹ�"));
	//	else
	//		AfxMessageBox(TEXT("����ʧ��"));
	}
	else
	{
		SetAddrInfo((char *)m_strServerAddr.c_str(), strlen((char *)m_strServerAddr.c_str()), m_ServerPort, AF_INET, TRUE);
		bConnectServer = Connect();
	}

	return bConnectServer;
}

//�ر�����
BOOL CTcpClient::CloseLink()
{
	return CloseNetService();
}

//���������������
BOOL CTcpClient::SendDataToServer(void *pData, long lDataLen)
{
	//����У��
	if (!pData) return FALSE;

	if(!AddDataToSend(pData, lDataLen)) return FALSE;
	
	BOOL bSend = SendDataToDest();

//	if (bSend) AfxMessageBox(TEXT("���ͳɹ�"));
//	else
	//	AfxMessageBox(TEXT("����ʧ��"));
	char rizhi[1024] = { 0 };
	sprintf(rizhi, "���͵�������%ld BYTE", m_stDataFlowCount.lSendTotal);
	CWriteDailyLog::WriteLog(rizhi, strlen(rizhi), "��־.txt");
	return bSend;
}

//�ӷ�������ȡ����
BOOL CTcpClient::RecvDataFromServer(void **pData, long *lDataLen)
{
	//����У��
	if ((!pData) || (!lDataLen)) return FALSE;

	if(!RecvDataFromSrc()) return FALSE;

	char rizhi[1024] = { 0 };
	sprintf(rizhi, "���յ�������%ld BYTE", m_stDataFlowCount.lRecvTotal);
	CWriteDailyLog::WriteLog(rizhi, strlen(rizhi), "��־.txt");

	return GetOutRecvData(pData, lDataLen);
}

//�������ã�����Ͽ����ӣ�
BOOL CTcpClient::RefreshConfig()
{
	//if (GetConnectStatus()) return FALSE;

	return TRUE;
}


//����Զ��
BOOL CTcpClient::Connect()
{
	//�Ϸ���֤
	if (m_nSocket < 0) return FALSE;
	//	if (m_bConnected) return TRUE;
	sockaddr_in *sin = m_SocketAddr.GetpAddrInInfo();
	if (sin == NULL) return FALSE;

	//�л��׽���ģʽ
	if (!SetNoBlockMode()) return FALSE;

	//����
	bool bConnectTimeOut = false;
	WORD i = 0;
	for (i = 0; i < TIME_OUT_COUNT; i++)
	{
		if (!(SOCKET_ERROR == connect(m_nSocket, (struct sockaddr *)sin, sizeof(sockaddr_in)) && WSAGetLastError() == WSAEWOULDBLOCK))
		{
		}

		int ret = 1;
		if (DelayMonitorSocket(0, 500000, false, true, &ret, false))
		{
			bConnectTimeOut = true;
			continue;	//��ʱΪ500����
		}
		else
		{
			bConnectTimeOut = false;
		}

		//�ж��Ƿ����ӳɹ�
		int optval = -1;
		int optlen = sizeof(optval);
		int retget = getsockopt(m_nSocket, SOL_SOCKET, SO_ERROR, (char*)(&optval), &optlen);
		if (retget != 0 || optval != 0)
		{
			bConnectTimeOut = true;
		}

		if (ret <= 0 && (i == (TIME_OUT_COUNT - 1)))
		{
			bConnectTimeOut = true;
			break;
		}

		if (ret > 0)
		{
			bConnectTimeOut = false;
			break;
		}
	}

	//�л��׽���ģʽ
	//SetBlockMode();

	//�жϳ�ʱ
	if (bConnectTimeOut)
	{
		if (m_bAutoCloseSocket)
			Close();
		return FALSE;
	}

	m_bConnected = TRUE;

	return m_bConnected;
}
