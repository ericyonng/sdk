#include"stdafx.h"
#include"TcpClient.h"
#include"TDXMWnd.h"

CTcpClient::CTcpClient()
{
	//初始化配置
	//m_strServerAddr = "122.114.247.193";
//	m_ServerPort = 8531;
	m_strServerAddr = CTDXMFrame::GetInstance()->m_strServerAddr;
	m_ServerPort = CTDXMFrame::GetInstance()->m_ServerPort;
	m_bIsDomainName = true;

	//设置密钥
	SetRsaPublicKey((unsigned char *)szServerPublicKey);
	SetRsaPrivateKey((unsigned char *)szClientPrivateKey);
}

CTcpClient::~CTcpClient()
{
}


//链接服务器
BOOL CTcpClient::Link(char *pServerAddr, unsigned short port, bool bClearDataCache)
{
	//参数校验
	if (pServerAddr == NULL) return FALSE;

	//判断是否已链接 已连接断开连接
	//if (GetNetConnectStatus())
	//{
	//	CloseNetService();
	//}

	//清空缓冲区
	if (bClearDataCache) ClearAllDataCache();

	//设置地址
	if (!SetAddrInfo(pServerAddr, strlen(pServerAddr), port, AF_INET))
	{
		return FALSE;
	}

	return Connect();
}

//链接服务器
BOOL CTcpClient::ConnectServer()
{
	//启动网络服务
	if (!StartNetService()) return FALSE;

	//链接服务器
	BOOL bConnectServer = FALSE;
	if (m_bIsDomainName)
	{
		bConnectServer = Link((char *)m_strServerAddr.c_str(), m_ServerPort);

	//	if (bConnectServer) AfxMessageBox(TEXT("链接成功"));
	//	else
	//		AfxMessageBox(TEXT("链接失败"));
	}
	else
	{
		SetAddrInfo((char *)m_strServerAddr.c_str(), strlen((char *)m_strServerAddr.c_str()), m_ServerPort, AF_INET, TRUE);
		bConnectServer = Connect();
	}

	return bConnectServer;
}

//关闭链接
BOOL CTcpClient::CloseLink()
{
	return CloseNetService();
}

//向服务器发送数据
BOOL CTcpClient::SendDataToServer(void *pData, long lDataLen)
{
	//参数校验
	if (!pData) return FALSE;

	if(!AddDataToSend(pData, lDataLen)) return FALSE;
	
	BOOL bSend = SendDataToDest();

//	if (bSend) AfxMessageBox(TEXT("发送成功"));
//	else
	//	AfxMessageBox(TEXT("发送失败"));
	char rizhi[1024] = { 0 };
	sprintf(rizhi, "发送的流量：%ld BYTE", m_stDataFlowCount.lSendTotal);
	CWriteDailyLog::WriteLog(rizhi, strlen(rizhi), "日志.txt");
	return bSend;
}

//从服务器获取数据
BOOL CTcpClient::RecvDataFromServer(void **pData, long *lDataLen)
{
	//参数校验
	if ((!pData) || (!lDataLen)) return FALSE;

	if(!RecvDataFromSrc()) return FALSE;

	char rizhi[1024] = { 0 };
	sprintf(rizhi, "接收的流量：%ld BYTE", m_stDataFlowCount.lRecvTotal);
	CWriteDailyLog::WriteLog(rizhi, strlen(rizhi), "日志.txt");

	return GetOutRecvData(pData, lDataLen);
}

//更新配置（必须断开链接）
BOOL CTcpClient::RefreshConfig()
{
	//if (GetConnectStatus()) return FALSE;

	return TRUE;
}


//链接远程
BOOL CTcpClient::Connect()
{
	//合法验证
	if (m_nSocket < 0) return FALSE;
	//	if (m_bConnected) return TRUE;
	sockaddr_in *sin = m_SocketAddr.GetpAddrInInfo();
	if (sin == NULL) return FALSE;

	//切换套接字模式
	if (!SetNoBlockMode()) return FALSE;

	//链接
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
			continue;	//超时为500毫秒
		}
		else
		{
			bConnectTimeOut = false;
		}

		//判断是否链接成功
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

	//切换套接字模式
	//SetBlockMode();

	//判断超时
	if (bConnectTimeOut)
	{
		if (m_bAutoCloseSocket)
			Close();
		return FALSE;
	}

	m_bConnected = TRUE;

	return m_bConnected;
}
