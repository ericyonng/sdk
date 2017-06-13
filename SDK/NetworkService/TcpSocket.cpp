#include"stdafx.h"
#include"TcpSocket.h"
#include<errno.h>
#pragma comment(lib, "ws2_32.lib")		//对网络API的支持	WinSock2.h文件里的变量和函数的符号链接放在ws2_32.lib里 ws2_32.dll的支持

//#include"SocketProtocol.h"
//#include"Dailylog.h"
//#include"rsa.h"




////////////////////////////////////


/////////////////////////////////////////////////////

///地址信息类
CAddr::CAddr()															//默认构造不初始化
{
	Clear();
}

//普通构造初始化数据
CAddr::CAddr(char *szip, unsigned short usPort, unsigned short usFamily)
{
	Clear();
	m_scktst_len = sizeof(m_stAddr_Info);
	memset(&m_stAddr_Info, 0, m_scktst_len);

	//合法性校验
	if (szip == NULL || strlen(szip) == 0)return;

	m_stAddr_Info.sin_addr.s_addr = inet_addr(szip);
	m_stAddr_Info.sin_family = usFamily;
	m_stAddr_Info.sin_port = htons(usPort);
	m_bSetAddr = TRUE;
}

CAddr::~CAddr()
{
	Clear();
}

//复位
void CAddr::Clear()
{
	m_scktst_len = sizeof(m_stAddr_Info);
	memset(&m_stAddr_Info, 0, sizeof(m_stAddr_Info));
	memset(m_DomainName, 0, HOST_ADDR_LEN);
	m_bSetAddr = FALSE;
}

CAddr & CAddr::operator =(CAddr &addr)							//重载赋值运算符
{
	//合法性校验
	if (addr.GetpAddrInInfo() == NULL || !(addr.GetSetAddrStatus())) return *this;

	//清理
	Clear();

	m_stAddr_Info.sin_addr.s_addr = (addr.GetpAddrInInfo())->sin_addr.s_addr;
	m_stAddr_Info.sin_family = (addr.GetpAddrInInfo())->sin_family;
	m_stAddr_Info.sin_port = (addr.GetpAddrInInfo())->sin_port;
	m_bSetAddr = TRUE;
	return *this;
}

void CAddr::Setip(char *szip)											//设置ip
{
	//合法性校验
	if (szip == NULL || strlen(szip) == 0) return;

	m_stAddr_Info.sin_addr.s_addr = inet_addr(szip);
}

char *CAddr::Getip()													//获取ip
{
	return inet_ntoa(m_stAddr_Info.sin_addr);
}

void CAddr::SetFamily( unsigned short shFamily)							//设置网络协议类型是IPv4还是IPv6
{
	m_stAddr_Info.sin_family = shFamily;
}

unsigned short CAddr::GetFamily()										//获取网络协议类型
{
	return m_stAddr_Info.sin_family;
}

void CAddr::SetPort( unsigned short shPort)							//设置端口号
{
	m_stAddr_Info.sin_port = htons(shPort);
}

unsigned short CAddr::GetPort()										//获取端口号
{
	return ntohs(m_stAddr_Info.sin_port);
}
void CAddr::SetAddrInfo(struct sockaddr_in &stAddr_Info)				//设置地址类结构体
{
	//清理
	Clear();

	m_scktst_len = sizeof(m_stAddr_Info);
	m_stAddr_Info.sin_addr.s_addr = stAddr_Info.sin_addr.s_addr;
	m_stAddr_Info.sin_family = stAddr_Info.sin_family;
	m_stAddr_Info.sin_port = stAddr_Info.sin_port;
	m_bSetAddr = TRUE;
}

void CAddr::SetAddrInfo(char *szip, unsigned short port, unsigned short family)
{
	//合法性校验
	if (szip == NULL || strlen(szip) == 0) return;

	//清理
	Clear();
	m_scktst_len = sizeof(m_stAddr_Info);
	m_stAddr_Info.sin_addr.s_addr = inet_addr(szip);
	m_stAddr_Info.sin_family = family;
	m_stAddr_Info.sin_port = htons(port);
	m_bSetAddr = TRUE;
}

//地址
BOOL CAddr::SetAddrInfoByDomainName(char *domain, int domainlen, unsigned short port, unsigned short family)
{
	//合法性校验
	if (domain == NULL || strlen(domain) == 0 || domainlen <= 0) return FALSE;

	try
	{
		struct hostent* stpRemote = NULL;
		struct in_addr addr;
		stpRemote = gethostbyname(domain);			//获取远程主机信息
		if (stpRemote == NULL) return FALSE;

		//清理
		Clear();

		addr.s_addr = *(u_long *)stpRemote->h_addr_list[0];			//ip地址

		//填写信息
		m_stAddr_Info.sin_family = family;//ipv4协议
		m_stAddr_Info.sin_port = htons((unsigned short)port);						//端口转换成网络字节序
		m_stAddr_Info.sin_addr.s_addr = *((u_long*)*stpRemote->h_addr_list);		//地址
		memset(m_DomainName, 0, HOST_ADDR_LEN);
		memmove(m_DomainName, domain, domainlen);
		m_bSetAddr = TRUE;
	}
	catch (...)
	{
		memset(m_DomainName, 0, HOST_ADDR_LEN);
		return FALSE;
	}

}

//域名
char *CAddr::GetDomainName()
{
	if (strlen(m_DomainName) == 0) return NULL;
	if (!gethostbyname(m_DomainName)) return NULL;
	return m_DomainName;
}

struct sockaddr * CAddr::GetpAddrInfo()									//获取地址结构体（强转为旧版结构体指针)
{
	return (struct sockaddr *)&(m_stAddr_Info);
}
struct sockaddr_in * CAddr::GetpAddrInInfo() 							//获取地址结构体新版
{
	return &(m_stAddr_Info);
}

int CAddr::GetSocketInfoLen()												//获取整型地址结构体空间大小
{
	return (int)(m_scktst_len);
}

socklen_t* CAddr::GetSocketInfoLenPoint() 										//获取 socklen_t 类型地址结构体空间大小指针
{
	return &(m_scktst_len);
}


////////////////////////////////////////////

//tcpsocket
bool CTcpSocket::m_bInitEnvironment = false;

CTcpSocket::CTcpSocket()
{
	memset(&m_wsaData, 0, sizeof(WSADATA));
	m_bConnected = FALSE;
	m_nSocket = INVALID_SOCKET_ID;
	m_bAutoCloseSocket = true;
	m_bSocketReadable = FALSE;
	m_bSocketWriteable = FALSE;
	ResetDataFlowCount();

	//初始化socket环境
	if (!m_bInitEnvironment)
		InitSocketEnvironment();
}

CTcpSocket:: ~CTcpSocket()
{
	if (m_bAutoCloseSocket)
		Close();	
}

//启动socket环境
BOOL CTcpSocket::InitSocketEnvironment()
{
	try
	{
		if (!m_bInitEnvironment)
		{
			//启动socket 初始化环境
			WSADATA wsaData;
			memset(&wsaData, 0, sizeof(WSADATA));
			int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);	//加载winsock DLL 每个进程调用一次就够了 否则无法使用socket
			if (iResult != 0)
			{
				m_bInitEnvironment = false;
				return FALSE;
			}

			m_bInitEnvironment = true;
		}
	}
	catch (...)
	{
		m_bInitEnvironment = false;
	}

	return TRUE;
}

//清理环境
BOOL CTcpSocket::CleanSocketEnvironment()
{
	int iRet = 0;
	try
	{
		if (m_bInitEnvironment)
		{
			if ((iRet = WSACleanup()) == SOCKET_ERROR)
			{
				if (WSACancelBlockingCall() == 0)//取消阻塞套接口
				{
					iRet = WSACleanup();
				}
			}

			if (iRet == 0) m_bInitEnvironment = false;
		}
	}
	catch (...)
	{
		iRet = -1;
	}
	

		//一个进程调用一次就够了若使用过程中被清除则会出现异常

	return iRet == 0 ? true:false;
}

//套接字
BOOL CTcpSocket::CreateSocket()
{
	//先断开
	Close();
	//if (m_bConnected)
	//{
	//	Close();
	//}
	//if (m_nSocket > 0) return TRUE;

	m_nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);			//生成套接字
	if (m_nSocket < 0 || INVALID_SOCKET == m_nSocket)
	{
		m_bConnected = FALSE;
		return FALSE;
	}

	m_bSocketReadable = TRUE;
	m_bSocketWriteable = TRUE;

	return TRUE;
}


//断开链接
bool CTcpSocket::Close()
{
	m_bConnected = FALSE;

	if (m_nSocket <0 || INVALID_SOCKET == m_nSocket) return true;

	if (closesocket(m_nSocket) != 0)
	{
		return false;
	}

	m_bSocketReadable = FALSE;
	m_bSocketWriteable = FALSE;
	m_nSocket = INVALID_SOCKET_ID;
	return true;
}

//优雅的断开连接
BOOL CTcpSocket::GraciousClose(int iHowto)
{
	if (m_nSocket <0 || INVALID_SOCKET == m_nSocket) return TRUE;

	if (shutdown(m_nSocket, iHowto) != 0)
	{
		return FALSE;
	}

	switch (iHowto)
	{
		case SD_RECEIVE:
		{
			m_bSocketReadable = FALSE;
			break;
		}
		case SD_SEND:
		{
			m_bSocketWriteable = FALSE;
			break;
		}
		case SD_BOTH:
		{
			m_bSocketReadable = FALSE;
			m_bSocketWriteable = FALSE;
			break;
		}
	}


	return TRUE;
}

//发送数据
BOOL CTcpSocket::SendData(VOID *pData, long *plDataLen)
{
	//CWriteDailyLog::WriteLog(m_bSocketWriteable ? "可写" : "禁写", strlen("可写"), "可读可写.txt");
	//不可发送
	if (!m_bSocketWriteable) return FALSE;
	

	//TCHAR szRizhi[100] = { 0 };
	//_sntprintf(szRizhi, 100, TEXT("%s %s"), m_nSocket == INVALID_SOCKET_ID ? TEXT("套接字无效") : TEXT("套接字有效"), m_bConnected ? TEXT("已链接") : TEXT("未链接"));

	//合法验证
	if ((m_nSocket == INVALID_SOCKET_ID)) return FALSE;

	if (pData == NULL || plDataLen == NULL) return FALSE;

	//发送数据
	long lSendRet = 0;
	if ((lSendRet = send(m_nSocket, (char *)pData, *plDataLen, 0)) == -1)
	{
		//CWriteDailyLog::WriteLog("SendData 失败 0", strlen("SendData 失败 0"), "可读可写.txt");

		*plDataLen = -1;
		//Close();
		return FALSE;
	}
	else if (lSendRet<*plDataLen)
	{
		//CWriteDailyLog::WriteLog("SendData 失败 1", strlen("SendData 失败 0"), "可读可写.txt");

		*plDataLen = lSendRet;
		return FALSE;
	}

	*plDataLen = lSendRet;

	//流量统计
	m_stDataFlowCount.lSendTotal += lSendRet;

	//CWriteDailyLog::WriteLog("SendData 成功 0", strlen("SendData 成功 0"), "可读可写.txt");

	return TRUE;
}

//接收数据
BOOL CTcpSocket::RecvData(VOID *pData, long *plDataLen)
{
	//CWriteDailyLog::WriteLog(m_bSocketReadable ? "可读" : "禁读", strlen("可写"), "可读可写.txt");
	//不可接收
	if (!m_bSocketReadable) return FALSE;

	//合法验证
	if ((m_nSocket == INVALID_SOCKET_ID)) return FALSE;
	if (pData == NULL || plDataLen == NULL) return FALSE;

	//定义
	long lRecvByte = 0;

	//接收数据
	memset(pData, 0, *plDataLen);
	if ((lRecvByte = recv(m_nSocket, (char *)pData, *plDataLen, 0)) <= 0)
	{
		//Close();
		*plDataLen = lRecvByte;
		return FALSE;
	}

	//输出
	*plDataLen = lRecvByte;

	//流量统计
	m_stDataFlowCount.lRecvTotal += lRecvByte;
	return TRUE;
}


//设置地址
void CTcpSocket::SetDestAddr(void *pAddr)
{
	if (pAddr)
		SetAddrInfo(*((CAddr *)pAddr)); 
}

//设置地址
BOOL CTcpSocket::SetAddrInfo(CAddr &addr)
{
	m_SocketAddr = addr;

	return	TRUE;
}

//设置地址
BOOL CTcpSocket::SetAddrInfo(char *domain, int domainlen, unsigned short port, unsigned short family)
{
	return m_SocketAddr.SetAddrInfoByDomainName(domain, domainlen, port, family);		
}

//设置地址
BOOL CTcpSocket::SetAddrInfo(char *ip, int iplen, unsigned short port, unsigned short family, BOOL bNotDomainName)
{
	m_SocketAddr.SetAddrInfo(ip, port, family);

	return	TRUE;
}

//重置
VOID CTcpSocket::Clear()
{
	if (m_nSocket != INVALID_SOCKET_ID) Close();
	m_nSocket = INVALID_SOCKET_ID;
	m_SocketAddr.Clear();
	m_bConnected = FALSE;
	ResetDataFlowCount();
}

//重置流量统计
void CTcpSocket::ResetDataFlowCount()
{
	memset(&m_stDataFlowCount, 0, S_DATA_FLOW_COUNT_T);
}

//阻塞模式
BOOL CTcpSocket::SetBlockMode()
{
	//合法验证
	if (m_nSocket < 0) return FALSE;

	//设置阻塞模式
	DWORD ul = 0;
	if (0 != ioctlsocket(m_nSocket, FIONBIO, &ul))
	{
		return FALSE;
	}

	return TRUE;
}

//非阻塞模式
BOOL CTcpSocket::SetNoBlockMode()
{
	//合法验证
	if (m_nSocket < 0) return FALSE;

	//设置非阻塞模式
	DWORD ul = 1;
	if (0 != ioctlsocket(m_nSocket, FIONBIO, &ul))
	{
		return FALSE;
	}

	return TRUE;
}

//套接字超时监控	1s
BOOL CTcpSocket::IsMonitorSocketTimeOut(bool bReadableDetect, bool bWriteableDetect, int *pErrOut, bool bSetOneAtLeast, bool bInfiniteWaiting)
{
	BOOL bTimeOut = FALSE;

	//变量定义
	fd_set *pFd_ReadOpt = NULL;
	fd_set *pFd_WriteOpt = NULL;
	memset(&m_fs_read, 0, sizeof(m_fs_read));
	memset(&m_fs_write, 0, sizeof(m_fs_write));

	//可读检测
	if (bReadableDetect)
	{
		FD_ZERO(&m_fs_read);
		FD_SET(m_nSocket, &m_fs_read);
		pFd_ReadOpt = &m_fs_read;
	}

	//可写检测
	if (bWriteableDetect)
	{
		FD_ZERO(&m_fs_write);
		FD_SET(m_nSocket, &m_fs_write);
		pFd_WriteOpt = &m_fs_write;
	}

	//超时监控参数初始化
	timeval timeout;
	memset(&timeout, 0, sizeof(timeout));
	timeout.tv_sec = 1;	//1s监控
	timeout.tv_usec = 0;

	int ret = SOCKET_ERROR;
	if (bInfiniteWaiting)
	{
		ret = select(m_nSocket+1, pFd_ReadOpt, pFd_WriteOpt, NULL, NULL);	//0表示超时，否则返回SOCKET_ERROR 当返回为-1时，所有描述符集清0。 当返回为正数时，表示已经准备好的描述符数。
	}
	else
	{
		ret = select(m_nSocket+1, pFd_ReadOpt, pFd_WriteOpt, NULL, &timeout);	//0表示超时，否则返回SOCKET_ERROR 当返回为-1时，所有描述符集清0。 当返回为正数时，表示已经准备好的描述符数。
	}

	if (ret == SOCKET_ERROR)
	{
		if (pErrOut) *pErrOut = SOCKET_ERROR;
		if (bReadableDetect) FD_CLR(m_nSocket, pFd_ReadOpt);
		if (bWriteableDetect) FD_CLR(m_nSocket, pFd_WriteOpt);
		return TRUE;
	}
	else
	{
		if (pErrOut) *pErrOut = ret;
	}

	if (!bSetOneAtLeast)
	{
		if (bReadableDetect&&!bWriteableDetect)
		{
			// 判断socket句柄是否可读  
			if (!FD_ISSET(m_nSocket, &m_fs_read))
			{
				if (bReadableDetect) FD_CLR(m_nSocket, pFd_ReadOpt);
				bTimeOut |= TRUE;
			}
		}
		else if (bWriteableDetect&&!bReadableDetect)
		{
			// 判断socket句柄是否可写  
			if (!FD_ISSET(m_nSocket, &m_fs_write))
			{
				if (bWriteableDetect) FD_CLR(m_nSocket, pFd_WriteOpt);
				bTimeOut = TRUE;
			}
		}
		else if (bWriteableDetect&&bWriteableDetect)
		{
			if ((!FD_ISSET(m_nSocket, &m_fs_read)) && (!FD_ISSET(m_nSocket, &m_fs_write)))
			{
				if (bReadableDetect) FD_CLR(m_nSocket, pFd_ReadOpt);
				if (bWriteableDetect) FD_CLR(m_nSocket, pFd_WriteOpt);
				bTimeOut = TRUE;
			}
		}
	}
	else
	{
		if (bWriteableDetect || bReadableDetect)
		{
			bTimeOut = FALSE;
			// 判断socket句柄是否可读  
			if (!FD_ISSET(m_nSocket, &m_fs_read))
			{
				if (bReadableDetect) FD_CLR(m_nSocket, pFd_ReadOpt);
				bTimeOut |= TRUE;
			}

			// 判断socket句柄是否可写  
			if (!FD_ISSET(m_nSocket, &m_fs_write))
			{
				if (bWriteableDetect) FD_CLR(m_nSocket, pFd_WriteOpt);
				bTimeOut |= TRUE;
			}
		}
	}

	return bTimeOut;
}

//套接字超时监控
BOOL CTcpSocket::DelayMonitorSocket(long tv_sec, long tv_usec, bool bReadableDetect, bool bWriteableDetect, int *pErrOut, bool bSetOneAtLeast, bool bInfiniteWaiting )
{
	BOOL bTimeOut = FALSE;

	//变量定义
	fd_set *pFd_ReadOpt = NULL;
	fd_set *pFd_WriteOpt = NULL;
	memset(&m_fs_read, 0, sizeof(m_fs_read));
	memset(&m_fs_write, 0, sizeof(m_fs_write));

	//可读检测
	if (bReadableDetect)
	{
		FD_ZERO(&m_fs_read);
		FD_SET(m_nSocket, &m_fs_read);
		pFd_ReadOpt = &m_fs_read;
	}

	//可写检测
	if (bWriteableDetect)
	{
		FD_ZERO(&m_fs_write);
		FD_SET(m_nSocket, &m_fs_write);
		pFd_WriteOpt = &m_fs_write;
	}

	//超时监控参数初始化
	timeval timeout;
	memset(&timeout, 0, sizeof(timeout));
	timeout.tv_sec = tv_sec;	//监控
	timeout.tv_usec = tv_usec;

	int ret = SOCKET_ERROR;
	if (bInfiniteWaiting)
	{
		ret = select(m_nSocket+1, pFd_ReadOpt, pFd_WriteOpt, NULL, NULL);	//0表示超时，否则返回SOCKET_ERROR 当返回为-1时，所有描述符集清0。 当返回为正数时，表示已经准备好的描述符数。
	}
	else
	{
		ret = select(m_nSocket+1, pFd_ReadOpt, pFd_WriteOpt, NULL, &timeout);	//0表示超时，否则返回SOCKET_ERROR 当返回为-1时，所有描述符集清0。 当返回为正数时，表示已经准备好的描述符数。
	}

	
	if (ret == SOCKET_ERROR)
	{
		if (pErrOut) *pErrOut = SOCKET_ERROR;
		if (bReadableDetect) FD_CLR(m_nSocket, pFd_ReadOpt);
		if (bWriteableDetect) FD_CLR(m_nSocket, pFd_WriteOpt);
		return TRUE;
	}
	else
	{
		if (pErrOut) *pErrOut = ret;
	}

	if (!bSetOneAtLeast)
	{
		if (bReadableDetect&&!bWriteableDetect)
		{
			// 判断socket句柄是否可读  
			if (!FD_ISSET(m_nSocket, &m_fs_read))
			{
				if (bReadableDetect) FD_CLR(m_nSocket, pFd_ReadOpt);
				bTimeOut = TRUE;
			}
		}
		else if (bWriteableDetect&&!bReadableDetect)
		{
			// 判断socket句柄是否可写  
			if (!FD_ISSET(m_nSocket, &m_fs_write))
			{
				if (bWriteableDetect) FD_CLR(m_nSocket, pFd_WriteOpt);
				bTimeOut = TRUE;
			}
		}
		else if (bWriteableDetect&&bWriteableDetect)
		{
			if ((!FD_ISSET(m_nSocket, &m_fs_read)) && (!FD_ISSET(m_nSocket, &m_fs_write)))
			{
				if (bReadableDetect) FD_CLR(m_nSocket, pFd_ReadOpt);
				if (bWriteableDetect) FD_CLR(m_nSocket, pFd_WriteOpt);
				bTimeOut = TRUE;
			}
		}
	}
	else
	{
		if (bWriteableDetect || bReadableDetect)
		{
			bTimeOut = FALSE;
			// 判断socket句柄是否可读  
			if (!FD_ISSET(m_nSocket, &m_fs_read))
			{
				if (bReadableDetect) FD_CLR(m_nSocket, pFd_ReadOpt);
				bTimeOut |= TRUE;
			}

			// 判断socket句柄是否可写  
			if (!FD_ISSET(m_nSocket, &m_fs_write))
			{
				if (bWriteableDetect) FD_CLR(m_nSocket, pFd_WriteOpt);
				bTimeOut |= TRUE;
			}
		}
	}


	return bTimeOut;
}

//清空socket缓冲区
BOOL CTcpSocket::ClearSocketCache()
{
	//变量定义
	char clr[8] = { 0 };
	long lClrLen = 1;
	int iErr = 0;

	while (true)
	{
		memset(clr, 0, sizeof(clr));
		if (IsMonitorSocketTimeOut(true, false, &iErr))
		{
			//异常
			if (iErr < 0) return FALSE;

			break;
		}

		RecvData(clr, &lClrLen);
	}

	return TRUE;
}

//设置套接字
VOID CTcpSocket::SetSocketFD(SOCKET hSock)
{
	if (INVALID_SOCKET_ID != hSock)
	{
		m_bSocketReadable = TRUE;
		m_bSocketWriteable = TRUE;
	}
	else
	{
		m_bSocketReadable = FALSE;
		m_bSocketWriteable = FALSE;
	}

	m_nSocket = hSock;
}

//获取对方ip端口
int CTcpSocket::GetPeerIP(char *peerip, int iPeerIPSize, unsigned short *peerport)
{
	if (!peerip || !peerport || iPeerIPSize <= 0) return -1;

	int ret = 0;
	struct sockaddr_in dstadd_in;
	socklen_t  len = sizeof(struct sockaddr_in);
	memset(&dstadd_in, 0, sizeof(struct sockaddr_in));
	if (ret=getpeername(m_nSocket, (struct sockaddr*)&dstadd_in, &len) < 0)
	{
		return WSAGetLastError();
		//err_err("getpeername()");
	}		
	else
	{
		if (iPeerIPSize < strlen(inet_ntoa(dstadd_in.sin_addr))) return -1;
		sprintf(peerip, "%s", inet_ntoa(dstadd_in.sin_addr));
		*peerport = ntohs(dstadd_in.sin_port);
	}
	return ret;
}

//获取本地地址端口
int CTcpSocket::GetLocalIP(char *localipaddr, int ipaddrsize, unsigned short *localport)
{
	if (!localipaddr || !localport || ipaddrsize <= 0) return -1;

	int ret = 0;
	struct sockaddr_in dstadd_in;
	socklen_t  len = sizeof(struct sockaddr_in);
	memset(&dstadd_in, 0, sizeof(struct sockaddr_in));
	if (ret=getsockname(m_nSocket, (struct sockaddr*)&dstadd_in, &len) < 0)
	{
		return WSAGetLastError();
		//err_err("getpeername()");
	}
	else
	{
		if (ipaddrsize < strlen(inet_ntoa(dstadd_in.sin_addr))) return -1;
		sprintf(localipaddr, "%s", inet_ntoa(dstadd_in.sin_addr));
		*localport = ntohs(dstadd_in.sin_port);
	}
	return ret;
}


///////////////////////////////////////////////////////////////////////////
//初始化socket环境
CNetworkService::CNetworkService()
{
	//m_BusinessLayerCache = NULL;
	m_pRecvDataOut = NULL;
	m_ServerPort = 0;
	m_bIsDomainName = false;
	m_bSetTransferSecurity = false;
	ClearAllDataCache();
}

CNetworkService::~CNetworkService()
{
	//清空队列
	ClearAllDataCache();
}

//启动网络服务
BOOL CNetworkService::StartNetService()
{
	//清空队列
	ClearAllDataCache();

	//创建socket
	return CreateSocket();
}



//断开链接
BOOL CNetworkService::CloseNetService()
{
	Close();

	//清空队列
	ClearAllDataCache();

	return TRUE;
}

//业务逻辑层数据添加
bool CNetworkService::AddDataToSend(void *pData, long lDataLen)
{
	//参数校验
	if (!pData) return FALSE;

	//变量定义
	CACHE_T *pCache = new CACHE_T;
	if (!pCache) return FALSE;
	memset(pCache, 0, S_CACHE_T);

	//是否设置安全传输
	if (m_bSetTransferSecurity)
	{
		//加密
		//密钥key
		BYTE pcbKey[DES_KEY_BIT + 1] = { 0 };
		BYTE cbKeyCipher[RSA_KEY_STR_LEN / 8] = { 0 };	//key密文
		BYTE *pKeyCipherTmp = cbKeyCipher;				//key密文
		unsigned long KeyCipherLen = RSA_KEY_STR_LEN / 8;
		DWORD dwKeyLen = DES_KEY_BIT + 1;
		DWORD dwDataLenTmp = lDataLen;		//明文长度
		CDESEngine DesSystem;
		CRsaEngine RsaSystem;
		//设置rsa密钥
		RsaSystem.SetPublicKey((unsigned char *)m_strRsaPublicKey.c_str());
		DWORD dwNeeded = DesSystem.EncrypteMemoryNeeded((BYTE *)pcbKey, &dwKeyLen, (BYTE *)pData, dwDataLenTmp);
		//int iKeyLentmpd = dwKeyLen;
		RsaSystem.Encrypte(pcbKey, dwKeyLen, &pKeyCipherTmp, &KeyCipherLen);
		pCache->pData = new char[dwNeeded + KeyCipherLen];
		if (!pCache->pData)
		{
			delete pCache;
			return FALSE;
		}
		memset(pCache->pData, 0, dwNeeded + KeyCipherLen);
		DesSystem.Encrypte(pcbKey, &dwKeyLen, (BYTE *)pData, dwDataLenTmp, (BYTE *)pCache->pData, &dwNeeded);	//业务层密文
		unsigned char *pDataTmp = (unsigned char *)pCache->pData;
		pDataTmp += dwNeeded;									//偏移
		memmove(pDataTmp, pKeyCipherTmp, KeyCipherLen);			//加密后的密钥
		pCache->lDataLen = dwNeeded + KeyCipherLen;				//实际加密后业务层长度
	}
	else
	{
		//无加密
		pCache->pData = new char[lDataLen];
		if (!pCache->pData)
		{
			delete pCache;
			return FALSE;
		}
		memset(pCache->pData, 0, lDataLen);
		memmove(pCache->pData, pData, lDataLen);
		pCache->lDataLen = lDataLen;
	}
	
	
	//日志
	//char rizhi[8192] = { 0 };
//	sprintf(rizhi, "业务层密文：pCache->pData:%s, 长度：%d;其中业务数据长度：%d\r\n加密后的密钥：%s, 长度：%d\r\n加密前的密钥：%s, 长度：%d;\r\n", pDataTmp, pCache->lDataLen, dwNeeded, pKeyCipherTmp, KeyCipherLen, pcbKey, iKeyLentmpd);
	//CWriteDailyLog::WriteLog(rizhi, "yewuceng.txt");

	//测试明文
	//pCache->pData = new char[lDataLen];
	//if (!pCache->pData)
	//{
	//	delete pCache;
	//	return FALSE;
	//}
	//memset(pCache->pData, 0, lDataLen);
	//memmove(pCache->pData, pData, lDataLen);
	//pCache->lDataLen = lDataLen;

	//TCHAR szRizhi[100] = { 0 };
	//TCHAR szResult[200] = { 0 };
	//CWriteDailyLog::AnsiToUnicode((char *)pCache->pData, szRizhi, 100*sizeof(TCHAR));
	//_sntprintf(szResult, 200, TEXT("%s, len:%d, %d"), szRizhi, pCache->lDataLen, m_lCacheDataSize);
	//MessageBox(NULL, szResult, NULL, 0);
	//CWriteDailyLog::WriteDialyLog((char *)pCache->pData);

	//加密
	//if (!DataEncrypt(pData, &lDataLen, &pCache->pData, &pCache->lDataLen))
	//{
	//	if (pCache) delete pCache;
	//	return FALSE;
	//}

	//插入队列
	//m_BusinessLayerCache = pCache;
	m_CacheDataQueue.push_back(pCache);
	
	//缓冲区长度增加
	m_lCacheDataSize += pCache->lDataLen;

	return TRUE;
}

//包分析
BOOL CNetworkService::CreateSocketPacketToSend(long &lPackCount)
{
	//条件判断
	if (m_CacheDataQueue.empty()) return FALSE;

	//变量定义
	long lPackCountTmp = 0;

	//清空待发送包缓冲区数据
	ClearSocketPackCacheToSend();

	//开空间
	unsigned char *ucpCacheTmp = new unsigned char[m_lCacheDataSize];
	if (!ucpCacheTmp) return FALSE;
	memset(ucpCacheTmp, 0, m_lCacheDataSize);

	//从缓存中转移数据
	unsigned char *pBusinessDataMoveTmp = ucpCacheTmp;
	while (!m_CacheDataQueue.empty())
	{
		memmove(pBusinessDataMoveTmp, m_CacheDataQueue.front()->pData, m_CacheDataQueue.front()->lDataLen);
		pBusinessDataMoveTmp += m_CacheDataQueue.front()->lDataLen;

		//释放空间
		delete m_CacheDataQueue.front()->pData;
		delete m_CacheDataQueue.front();
		//m_CacheDataQueue.front() = NULL;
		m_CacheDataQueue.pop_front();
	}

	//清理空间
	m_CacheDataQueue.clear();
	
	//业务层数据
	pBusinessDataMoveTmp = ucpCacheTmp;
	
	//判断业务层长度确定包数量
	if (m_lCacheDataSize > BUSINESS_DATA_LIMIT)
	{
		lPackCountTmp = m_lCacheDataSize / BUSINESS_DATA_LIMIT + ((m_lCacheDataSize%BUSINESS_DATA_LIMIT > 0) ? 1 : 0);
	}
	else
	{
		lPackCountTmp = 1;
	}

	//变量定义
	long lPackSeq = 0;	//包序号
	long lCount = lPackCountTmp;
	long lCacheDataSizeTmp = m_lCacheDataSize;	//数据大小
	long lSingleDataSize = 0;
	long lPacketTotalSize = 0;
	char *pPacketContainer = NULL;	//包流
	char *pPacketContainerTmp = pPacketContainer;
	CACHE_T *pDataCacheToSend = NULL;

	//fortest
	long lCountLen = 0;

	//循环：
	while (lCount-- >0)
	{
		//业务层长度
		lSingleDataSize = ((lCacheDataSizeTmp > BUSINESS_DATA_LIMIT) ? BUSINESS_DATA_LIMIT : lCacheDataSizeTmp);
		lCacheDataSizeTmp -= lSingleDataSize;

		//包总长
		lPacketTotalSize = S_SOCKET_PACK_HEAD_T + S_ACCESS_LAYER_T + lSingleDataSize + S_SOCKET_PACK_TAIL;

		//fortest
		lCountLen += lPacketTotalSize;

		//开空间存包
		pPacketContainer = new char[lPacketTotalSize];
		if (!pPacketContainer) return FALSE;
		memset(pPacketContainer, 0, lPacketTotalSize);
		pPacketContainerTmp = pPacketContainer;

		//包头
		memset(&m_stPackeHead, 0, S_SOCKET_PACK_HEAD_T);
		m_stPackeHead.usHead = SOCKET_PACK_HEAD_T::HEAD_ID;
		memmove(pPacketContainerTmp, &m_stPackeHead, S_SOCKET_PACK_HEAD_T);
		pPacketContainerTmp += S_SOCKET_PACK_HEAD_T;

		//接入层
		memset(&m_AccessLayer, 0, S_ACCESS_LAYER_T);
		m_AccessLayer.ulPacketNum = lPackCountTmp;
		m_AccessLayer.ulPacketSeq = lPackSeq;
		m_AccessLayer.ulPacketSize = lPacketTotalSize;
		//m_AccessLayer.uTransID = ? ;
		memmove(pPacketContainerTmp, &m_AccessLayer, S_ACCESS_LAYER_T);
		pPacketContainerTmp += S_ACCESS_LAYER_T;		

		//业务层
		memmove(pPacketContainerTmp, pBusinessDataMoveTmp, lSingleDataSize);
		pBusinessDataMoveTmp += lSingleDataSize;
		pPacketContainerTmp += lSingleDataSize;

		//包尾
		memset(&m_stPackeTail, 0, S_SOCKET_PACK_TAIL);
		m_stPackeTail.usTail = m_stPackeTail.TAIL_ID;
		memmove(pPacketContainerTmp, &m_stPackeTail, S_SOCKET_PACK_TAIL);
		pPacketContainerTmp += S_SOCKET_PACK_TAIL;

		//开空间
		pDataCacheToSend = new CACHE_T;
		if (!pDataCacheToSend) return FALSE;
		memset(pDataCacheToSend, 0, S_CACHE_T);
		pDataCacheToSend->pData = pPacketContainer;
		pDataCacheToSend->lDataLen = lPacketTotalSize;

		//一个包完成压入待发包缓冲区
		m_SocketPackCacheToSend.insert(map<long, CACHE_T *>::value_type(lPackSeq, pDataCacheToSend));

		//序号自增
		lPackSeq++;
	}

	//释放业务层空间
	delete[]ucpCacheTmp;

	//业务层数据缓冲区重置
	ClearBusinessLayerToSendCache();

	//包总数
	lPackCount = m_SocketPackCacheToSend.size();

	return TRUE;	
}

//发送数据包
bool CNetworkService::SendDataToDest(BOOL bSocketBlockMode, BOOL bNeedSocketBlockSet, bool bNeedReturnPacket, long lTvPerSec, long lTvPerUsec, int iTimeOutCountSet)
{
	//条件校验
	//是否已经创建socket
	if (m_nSocket < 0) return FALSE;

	//是否已经链接
	//判断是否已链接
	//if (!GetNetConnectStatus()) return FALSE;

	//if (m_CacheDataQueue.empty()) return FALSE;
	long lPackCount=0;

	//日志
	//CWriteDailyLog::WriteLog("SendDataToDest 0", strlen("SendDataToDest 0"), "日志.txt");

	//分析并组装包(加密业务层数据)
	if (!CreateSocketPacketToSend(lPackCount)) return FALSE;

	//日志
	//CWriteDailyLog::WriteLog("SendDataToDest 1", strlen("SendDataToDest 1"), "日志.txt");

	//有无数据要发送
	if (m_SocketPackCacheToSend.empty()) return FALSE;
	
	//日志
	//CWriteDailyLog::WriteLog("SendDataToDest 2", strlen("SendDataToDest 1"), "日志.txt");


	//设置阻塞模式
	if (bNeedSocketBlockSet)
	{
		if (bSocketBlockMode)
		{
			if (!SetBlockMode()) return FALSE;
		}
		else
		{
			if (!SetNoBlockMode()) return FALSE;
		}
	}

	//日志
//	CWriteDailyLog::WriteLog("SendDataToDest 3", strlen("SendDataToDest 1"), "日志.txt");


	//发包过程
	int iTimeOutCount = iTimeOutCountSet;
	char RecvCache[SOCKET_PACK_SIZE_MAX_LIMIT] = { 0 };
	long lRecvCacheSize = sizeof(RecvCache);
	BOOL bRecvReturn = FALSE;
	//fortest
	long lCountSuccess = 0;
	long lCountLen = 0;
	long lCurrentPackSeq=0;
	while (true)
	{
		if (!m_SocketPackCacheToSend.empty())
		{
			//发包
			for (long i = 0; i < m_SocketPackCacheToSend.size(); i++)
			{
				//取包
				CACHE_T *pData = m_SocketPackCacheToSend[i];

				//TCHAR szRizhi[100] = { 0 };
				//TCHAR szResult[200] = { 0 };
				//CWriteDailyLog::AnsiToUnicode((char *)(((char *)pData->pData)+10), szRizhi, 100 * sizeof(TCHAR));
				//_sntprintf(szResult, 200, TEXT("%s, len %d "), szRizhi, pData->lDataLen);
				//MessageBox(NULL, szResult, NULL, 0);

				//超时次数限制
				iTimeOutCount = iTimeOutCountSet;

				//发送一个包			
				while ((iTimeOutCountSet-->0))
				{
					bRecvReturn = FALSE;

					//发包
					if (SendData(pData->pData, &pData->lDataLen) == FALSE) return FALSE;

					//是否需要成功接收包应答
					if (!bNeedReturnPacket) break;
					
					//超时等待	500ms
					//if (IsMonitorSocketTimeOut()) continue;
					if (DelayMonitorSocket(0, 500000)) continue;

					//接收一个接收正确返回包
					//memset(RecvCache, 0, lRecvCacheSize);
					//if (bRecvReturn=RecvData(RecvCache, &lRecvCacheSize))
					//{
						//直接清空掉并继续发下一个包（也可以解析出来，判断功能号RIGHT_RECV）
					//	if (!IsDestRecvSuccess(RecvCache, &lRecvCacheSize)) continue;
						//memset(RecvCache, 0, iRecvCacheSize);
						//完整包判断	半包丢弃重发	再判断功能号 right_Recv 或者其他功能号
					//	iCountSuccess++;
					//	iCountLen += pData->lDataLen;
					//	break;
					//}

					lCurrentPackSeq = ((ACCESS_LAYER_T *)(((char *)pData->pData) + S_SOCKET_PACK_HEAD_T))->ulPacketSeq;
					if (bRecvReturn = IsDestRecvSuccess(lCurrentPackSeq, bSocketBlockMode, !bNeedSocketBlockSet))
					{
						lCountSuccess++;
						lCountLen += pData->lDataLen;
						break;
					}
				}

				//发送失败判断
				if (iTimeOutCount <= 0 && bRecvReturn == FALSE&&bNeedReturnPacket)
				{
					return FALSE;
				}
			}
		}

		break;
	}

	//TCHAR ridd[100] = { 0 };
	//_sntprintf(ridd, 100, TEXT("发送成功包：iCountSuccess:%d, 长度：%d"), iCountSuccess, iCountLen);
	//if (bNeedReturnPacket)
	//	AfxMessageBox(ridd);

	//发送完毕 清空待发送任务队列
	ClearSocketPackCacheToSend();

	return TRUE;

	//(从待发包序列取一个包，并send给服务端，等待服务端发回接收完成应答再发下一个包（若1s内未回复重发）如此循环直到把包发完，最后清空队列)
	//发包结束
}

//对方是否接收成功确认
BOOL CNetworkService::IsDestRecvSuccess(/*待确认的包序号*/long lCurrentPackSeq, BOOL bSocketBlockMode, BOOL bNeedSocketBlockSet)
{
	//成功接收应打包功能号 FUNID_RIGHT_RECV
	RECV_CONFIRM_PACK_T *pSuccessRecvPack = NULL;
	long lRecvPackSize = 0;

	//日志
//	CWriteDailyLog::WriteLog("IsDestRecvSuccess 1", strlen("IsDestRecvSuccess 1"), "日志.txt");

	if (!RecvDataFromSrc(bSocketBlockMode, bNeedSocketBlockSet, FALSE)) return FALSE;

	//日志
//	CWriteDailyLog::WriteLog("IsDestRecvSuccess 2", strlen("IsDestRecvSuccess 1"), "日志.txt");

	if (!GetOutRecvData((void **)&pSuccessRecvPack, &lRecvPackSize))
	{
		ReleaseRecvDataOut();
		pSuccessRecvPack = NULL;
		return FALSE;
	}

	//日志
	//CWriteDailyLog::WriteLog("IsDestRecvSuccess 3", strlen("IsDestRecvSuccess 1"), "日志.txt");

	if (!pSuccessRecvPack) return FALSE;

	//日志
	//char rizhi[100] = { 0 };
	//sprintf(rizhi, "lPackSeq:%ld; RecvSeq:%ld;", lCurrentPackSeq, pSuccessRecvPack->ulPacketSeq);
	//CWriteDailyLog::WriteLog(rizhi, strlen(rizhi), "日志.txt");

	//判断
	if (pSuccessRecvPack->uFunID == RECV_CONFIRM_PACK_T::SUCCESS_RECV
		&&pSuccessRecvPack->ulPacketSeq == lCurrentPackSeq)
	{
		ReleaseRecvDataOut();
		return TRUE;
	}

	ReleaseRecvDataOut();

	return FALSE;
}

//接收数据包
//1.以一个数据包最大限制来接收数据包，
//2.定位包头包尾（一个包的头两个字节为包头，一个包的后两个字节为包尾）
//3.定位包数据长度（包接收的字节数应该与之相符）(断包半包等不予接收，进行超时接收等待,若限定时间内未接收到完整包则断开链接接收失败并清理数据)
//4.定位包序号和总包数（用于计数接收次数）
//5.维护一个去重包序列，已接收的包不需要再接收，但表示接收成功

//压入待接收缓冲区并返回接收成功应答包

//退出while
//检测接收包完整性（包的个数）
//进行拆包并拼接业务层，拼接完成后解密业务层，送入等待接收业务层缓冲区
//完成接收
BOOL CNetworkService::RecvDataFromSrc(BOOL bSocketBlockMode, BOOL bNeedSocketBlockSet, BOOL bNeedSendSuccessRecvReturnPack, long lTvPerSec, long lTvPerUsec, int iTimeOutCountSet)
{
	//MessageBox(NULL, TEXT("RecvDataFromSrc 01"), NULL, 0);
	//条件校验
	//是否已经创建socket
	if (m_nSocket < 0) return FALSE;
	//是否已经链接
	//判断是否已链接
	//if (!GetNetConnectStatus()) return FALSE;
//	if (!m_SocketPackCacheToRecv.empty()) return FALSE;

	//清空待接收数据缓冲区 腾出空间 socket包缓冲
	//ClearBusinessLayerToRecvCache();
	ClearSocketPackCacheToRecv();

	//设置阻塞模式
	if (bNeedSocketBlockSet)
	{
		if (bSocketBlockMode)
		{
			if (!SetBlockMode()) return FALSE;
		}
		else
		{
			if (!SetNoBlockMode()) return FALSE;
		}
	}

	
	//接收包
	char recvdata[SOCKET_PACK_SIZE_MAX_LIMIT] = { 0 };
	long lRecvDataLen = 0;
	BOOL bRecvSuccess = FALSE;
	int iCountTimeOut = iTimeOutCountSet;
//	int iCountSuccessRecv = 0;		//成功接收总数
	unsigned short usHead = 0;
	unsigned short usTail = 0;
	unsigned long ulPacketSize = 0;
	unsigned long ulPacketNum = 0;
	unsigned long ulPacketSeq = 0;
	unsigned long ulTransID = 0;
	ACCESS_LAYER_T *pAccessLayer = NULL;
	//成功接收应打包功能号
	RECV_CONFIRM_PACK_T stRecvConfirm;
	memset(&stRecvConfirm, 0, S_RECV_CONFIRM_PACK_T);
	stRecvConfirm.uFunID = stRecvConfirm.SUCCESS_RECV;
	//unsigned int uSuccessRecvFunID = FUNID_RIGHT_RECV;
	CACHE_T *pDataCacheSave = NULL;
	m_lRecvCacheDataSize = 0;		//待

	//fortest
	long lCountSuccess = 0;
	long lCountLen = 0;

	while (iCountTimeOut-->0)
	{
		//接收
		usHead = 0;
		usTail = 0;
		lRecvDataLen = SOCKET_PACK_SIZE_MAX_LIMIT;
		bRecvSuccess = FALSE;
		ulPacketSize = 0;
		ulPacketNum = 0;
		ulPacketSeq = 0;
		ulTransID = 0;
		pAccessLayer = NULL;
		pDataCacheSave = NULL;
		memset(recvdata, 0, SOCKET_PACK_SIZE_MAX_LIMIT);

		//超时接收
		//if (IsMonitorSocketTimeOut()) continue;
		if (DelayMonitorSocket(lTvPerSec, lTvPerUsec)) continue;
		bRecvSuccess = RecvData(recvdata, &lRecvDataLen);

		if (!bRecvSuccess) continue;



		//包头包尾校验
		usHead = ((SOCKET_PACK_HEAD_T *)recvdata)->usHead;
		usTail = ((SOCKET_PACK_TAIL *)(recvdata + (lRecvDataLen - S_SOCKET_PACK_TAIL)))->usTail;
		if (usHead != SOCKET_PACK_HEAD_T::HEAD_ID || usTail != SOCKET_PACK_TAIL::TAIL_ID) continue;

		//接入层
		pAccessLayer = ((ACCESS_LAYER_T *)(recvdata + S_SOCKET_PACK_HEAD_T));

		//校验包总长
		//usPacketSize = *((unsigned short *)(recvdata + sizeof(unsigned short)));
		ulPacketSize = pAccessLayer->ulPacketSize;
		//不返回成功应打包客户端会超时重发
		if (ulPacketSize != lRecvDataLen) continue;

		//包总数
		ulPacketNum = pAccessLayer->ulPacketNum;

		//包序号
		ulPacketSeq = pAccessLayer->ulPacketSeq;

		//包流水号
		ulTransID = pAccessLayer->ulTransID;

		//检测是否重包
		if (m_SocketPackCacheToRecv.find(ulPacketSeq) != m_SocketPackCacheToRecv.end())
		{
			//发现重包 发回应答包 客户端发送下一个包 不成功的包
			//发送成功接收应打包
			if (bNeedSendSuccessRecvReturnPack)
			{
				stRecvConfirm.ulPacketSeq = ulPacketSeq;
				AddDataToSend(&stRecvConfirm, sizeof(stRecvConfirm));
				SendDataToDest(FALSE, !bNeedSocketBlockSet, false);
			}

			//复位超时次数
			iCountTimeOut = iTimeOutCountSet;
			continue;
		}

		//开空间保存业务逻辑层数据
		pDataCacheSave = new CACHE_T;
		if (!pDataCacheSave) return FALSE;
		memset(pDataCacheSave, 0, S_CACHE_T);
		pDataCacheSave->lDataLen = ulPacketSize - S_SOCKET_PACK_HEAD_T - S_ACCESS_LAYER_T - S_SOCKET_PACK_TAIL;
		pDataCacheSave->pData = new char[pDataCacheSave->lDataLen];
		if (!pDataCacheSave->pData)
		{
			delete pDataCacheSave;
			return FALSE;
		}
		memset(pDataCacheSave->pData, 0, pDataCacheSave->lDataLen);
		memmove(pDataCacheSave->pData, (char *)(recvdata + S_SOCKET_PACK_HEAD_T + S_ACCESS_LAYER_T), pDataCacheSave->lDataLen);
		
		//压入待接收socket缓冲
		if (m_SocketPackCacheToRecv.find(ulPacketSeq) == m_SocketPackCacheToRecv.end())
		{
			//累加包长度用于开空间组装业务层数据
			m_lRecvCacheDataSize += pDataCacheSave->lDataLen;

			m_SocketPackCacheToRecv.insert(map<long, CACHE_T*>::value_type(ulPacketSeq, pDataCacheSave));

			//发送成功接收应打包
			ClearBusinessLayerToSendCache();
			BOOL  bRet = FALSE;
			if (bNeedSendSuccessRecvReturnPack)
			{
				stRecvConfirm.ulPacketSeq = ulPacketSeq;
				AddDataToSend(&stRecvConfirm, sizeof(stRecvConfirm));
				bRet = SendDataToDest(FALSE, !bNeedSocketBlockSet, false);
			}
	
				//TCHAR szRizhi[100] = { 0 };
				//TCHAR szResult[200] = { 0 };
				//CWriteDailyLog::AnsiToUnicode((char *)(((char *)pDataCacheSave->pData)), szRizhi, 100 * sizeof(TCHAR));
				//_sntprintf(szResult, 200, TEXT("%s; %s, 数据：%s, len %d "), bRet ? TEXT("回执发送成功") : TEXT("回执发送失败"), bRecvSuccess ? TEXT("成功recv") : TEXT("失败recv"), szRizhi, m_lRecvCacheDataSize);
			//MessageBox(NULL, szResult, NULL, 0);

			//fortest
			lCountSuccess++;
			lCountLen += ulPacketSize;

			//接收成功重置
			iCountTimeOut = iTimeOutCountSet;

			//检测是否接收完成
			if (m_SocketPackCacheToRecv.size() == ulPacketNum) break;
		}
	}

	if (!bRecvSuccess)
	{
		return FALSE;
	}

	//拼接接收到的业务层数据
	CreateBussnessLayerToGet();

	//清理socket等待接收缓冲区
//	ClearSocketPackCacheToRecv();

	return TRUE;
}

//解包(并解密)将socket待接收缓冲中数据拼接成业务层数据 得到一个业务逻辑层数据
BOOL CNetworkService::CreateBussnessLayerToGet()
{
	//参数校验
	if (m_SocketPackCacheToRecv.empty() || m_lRecvCacheDataSize==0) return FALSE;

	//开空间
	char *pBusinessLayerData = new char[m_lRecvCacheDataSize];
	if (!pBusinessLayerData) return FALSE;
	memset(pBusinessLayerData, 0, m_lRecvCacheDataSize);
	char *pDataTmp = pBusinessLayerData;

	//按顺序取数据 size，从0开始到size
	long lPacketCount = m_SocketPackCacheToRecv.size();
	long lBusinessTotalSize = 0;
	for (long i = 0; i < lPacketCount; i++)
	{
		if (m_SocketPackCacheToRecv[i])
		{
			if (m_SocketPackCacheToRecv[i]->pData)
			{
				memmove(pDataTmp, m_SocketPackCacheToRecv[i]->pData, m_SocketPackCacheToRecv[i]->lDataLen);
				pDataTmp += m_SocketPackCacheToRecv[i]->lDataLen;
				lBusinessTotalSize += m_SocketPackCacheToRecv[i]->lDataLen;
				delete[]m_SocketPackCacheToRecv[i]->pData;
				delete m_SocketPackCacheToRecv[i];
				m_SocketPackCacheToRecv[i] = NULL;
			}
		}		
	}

	//校验数据完整性
	if (lBusinessTotalSize != m_lRecvCacheDataSize)
	{
		delete[]pBusinessLayerData;
		return FALSE;
	}

	//解密数据
	CACHE_T *pBusinessDataCache = new CACHE_T;
	if (!pBusinessDataCache)
	{
		delete[]pBusinessLayerData;
		return FALSE;
	}
	memset(pBusinessDataCache, 0, sizeof(CACHE_T));
	
	if (m_bSetTransferSecurity)
	{
		//解密
		CRsaEngine RsaSystem;
		//设置私钥
		RsaSystem.SetPrivateKey((unsigned char *)m_strRsaPrivateKey.c_str());
		CDESEngine DesSystem;
		//寻找密钥 最后的 CIPHER_TEXT_OUT_ENCODE_LEN 长度
		BYTE *pcbKeyCipher = (BYTE *)(((char *)pBusinessLayerData) + m_lRecvCacheDataSize - CIPHER_TEXT_OUT_ENCODE_LEN);
		BYTE cbKeyPlain[DES_KEY_BIT + 1] = { 0 };
		BYTE *pcbKeyPlainTmp = cbKeyPlain;
		unsigned long ulKeyPlainSize = DES_KEY_BIT + 1;
		//解密密钥
		RsaSystem.Decrypte(pcbKeyCipher, CIPHER_TEXT_OUT_ENCODE_LEN, &pcbKeyPlainTmp, &ulKeyPlainSize);

		//开空间放解密后的明文文
		unsigned char *pPlainDataTmp = new unsigned char[m_lRecvCacheDataSize - CIPHER_TEXT_OUT_ENCODE_LEN];
		if (!pPlainDataTmp)
		{
			delete[]pBusinessLayerData;
			return FALSE;
		}
		memset(pPlainDataTmp, 0, m_lRecvCacheDataSize - CIPHER_TEXT_OUT_ENCODE_LEN);
		DWORD dwPlainDataLen = m_lRecvCacheDataSize - CIPHER_TEXT_OUT_ENCODE_LEN;

		//解密数据
		DesSystem.Decrypte(cbKeyPlain, ulKeyPlainSize, (BYTE *)pBusinessLayerData, m_lRecvCacheDataSize - CIPHER_TEXT_OUT_ENCODE_LEN, pPlainDataTmp, &dwPlainDataLen);

		//开空间存放解密后的数据
		pBusinessDataCache->pData = new char[dwPlainDataLen];
		if (!pBusinessDataCache->pData)
		{
			delete[]pBusinessLayerData;
			delete[]pPlainDataTmp;
			return FALSE;
		}
		memset(pBusinessDataCache->pData, 0, dwPlainDataLen);
		memmove(pBusinessDataCache->pData, pPlainDataTmp, dwPlainDataLen);
		pBusinessDataCache->lDataLen = dwPlainDataLen;
		delete[]pPlainDataTmp;
	}
	else
	{
		//无加密
		pBusinessDataCache->pData = new char[m_lRecvCacheDataSize];
		if (!pBusinessDataCache->pData)
		{
			delete[]pBusinessLayerData;
			return FALSE;
		}
			memset(pBusinessDataCache->pData, 0, m_lRecvCacheDataSize);
			memmove(pBusinessDataCache->pData, pBusinessLayerData, m_lRecvCacheDataSize);
			pBusinessDataCache->lDataLen = m_lRecvCacheDataSize;
	}
	
	
	//test;
	//pBusinessDataCache->pData = new char[m_lRecvCacheDataSize];
	//if (!pBusinessDataCache->pData)
	//{
	//	delete[]pBusinessLayerData;
	//	return FALSE;
	//}
//	memset(pBusinessDataCache->pData, 0, m_lRecvCacheDataSize);
//	memmove(pBusinessDataCache->pData, pBusinessLayerData, m_lRecvCacheDataSize);
//	pBusinessDataCache->lDataLen = m_lRecvCacheDataSize;

	//解密
	//DataDecrypt(pBusinessLayerData, &m_lRecvCacheDataSize, &pBusinessDataCache->pData, &pBusinessDataCache->lDataLen);
	m_lRecvCacheDataSize = pBusinessDataCache->lDataLen;

	//清空数据腾出空间
	ClearBusinessLayerToRecvCache();

	//将数据送入业务层缓冲区
	if (pBusinessDataCache->pData)
	{
		m_RecvCacheDataQueue.push_back(pBusinessDataCache);
	}

	//清理空间
	if (pBusinessLayerData) delete[]pBusinessLayerData;

	//清除待接收数据缓冲区socket
	ClearSocketPackCacheToRecv();

	return TRUE;
}

//取出接收到的数据
BOOL CNetworkService::GetOutRecvData(void **pData, long *lDataLen)
{
	//参数校验
	if (pData == NULL || lDataLen == NULL) return FALSE;
	if (m_RecvCacheDataQueue.empty()) return FALSE;

	//取数据
	//开空间
	//数据迁移
	//清理空间
	//完成
	while(!m_RecvCacheDataQueue.empty())
	{
		CACHE_T *pCacheData = m_RecvCacheDataQueue.front();
		if (pCacheData)
		{
			*pData = new char[pCacheData->lDataLen];
			if (NULL == *pData) return FALSE;
			memset(*pData, 0, pCacheData->lDataLen);
			memmove(*pData, pCacheData->pData, pCacheData->lDataLen);
			*lDataLen = pCacheData->lDataLen;
			delete[]pCacheData->pData;
			delete pCacheData;
			pCacheData = NULL;
			m_RecvCacheDataQueue.pop_front();
			
			//保存用于释放
			m_pRecvDataOut = pData;

		//	delete[] * pData;
			break;
		}
		else
		{
			m_RecvCacheDataQueue.pop_front();
		}		
	}

	return TRUE;

}

//释放取出来的数据
BOOL CNetworkService::ReleaseRecvDataOut()
{
	if (m_pRecvDataOut)
	{
		delete[]m_pRecvDataOut;
		m_pRecvDataOut = NULL;
	}

	return TRUE;
}

//设置公钥
void CNetworkService::SetRsaPublicKey(unsigned char *ucKey)
{
	if (!ucKey) return;
	m_bSetTransferSecurity = true;
	m_strRsaPublicKey.clear();
	m_strRsaPublicKey = (char *)ucKey;
}

//设置私钥
void CNetworkService::SetRsaPrivateKey(unsigned char *ucKey)
{
	if (!ucKey) return;
	m_bSetTransferSecurity = true;
	m_strRsaPrivateKey.clear();
	m_strRsaPrivateKey = (char *)ucKey;
}

//清空待发包缓冲区  3区中的send
BOOL CNetworkService::ClearSocketPackCacheToSend()
{
	//变量定义
	map<long, CACHE_T*>::iterator itCacheTmp;
	CACHE_T* pCacheTmpValue = NULL;

	//清空待发包序列
	if (!m_SocketPackCacheToSend.empty())
	{
		for (itCacheTmp = m_SocketPackCacheToSend.begin(); itCacheTmp != m_SocketPackCacheToSend.end(); itCacheTmp++)
		{
			pCacheTmpValue = itCacheTmp->second;
			if (pCacheTmpValue)
			{
				if (pCacheTmpValue->pData) delete pCacheTmpValue->pData;
				delete pCacheTmpValue;
				pCacheTmpValue = NULL;
			}
		}

		m_SocketPackCacheToSend.clear();
	}

	return TRUE;
}


//清空待接收包缓冲区 3区中的recv
BOOL CNetworkService::ClearSocketPackCacheToRecv()
{
	//变量定义
	map<long, CACHE_T*>::iterator itCacheTmp;
	CACHE_T* pCacheTmpValue = NULL;

	//清空待发包序列
	if (!m_SocketPackCacheToRecv.empty())
	{
		for (itCacheTmp = m_SocketPackCacheToRecv.begin(); itCacheTmp != m_SocketPackCacheToRecv.end(); itCacheTmp++)
		{
			pCacheTmpValue = itCacheTmp->second;
			if (pCacheTmpValue)
			{
				if (pCacheTmpValue->pData) delete pCacheTmpValue->pData;
				delete pCacheTmpValue;
				pCacheTmpValue = NULL;
			}
		}

		m_SocketPackCacheToRecv.clear();
	}

	return TRUE;
}

//socket业务层数据缓冲区待发送清理 1区
BOOL CNetworkService::ClearBusinessLayerToSendCache()
{
	m_lCacheDataSize = 0;
	memset(&m_stPackeHead, 0, S_SOCKET_PACK_HEAD_T);
	memset(&m_AccessLayer, 0, S_ACCESS_LAYER_T);
	memset(&m_stPackeTail, 0, S_SOCKET_PACK_TAIL);

	//变量定义
	list<CACHE_T *>::iterator itList;
	CACHE_T * pBusinessData = NULL;

	if (!m_CacheDataQueue.empty())
	{
		for (itList = m_CacheDataQueue.begin(); itList != m_CacheDataQueue.end(); itList++)
		{
			pBusinessData = *itList;
			if (pBusinessData)
			{
				if (pBusinessData->pData)
				{
					delete pBusinessData->pData;
				}
				delete pBusinessData;
				pBusinessData = NULL;
			}
		}

		m_CacheDataQueue.clear();
	}

	return TRUE; 
}

//socket业务层数据缓冲区待接收清理 2区
BOOL CNetworkService::ClearBusinessLayerToRecvCache()
{ 
	m_lRecvCacheDataSize = 0;


	//变量定义
	list<CACHE_T *>::iterator itList;
	CACHE_T * pBusinessData = NULL;

	if (!m_RecvCacheDataQueue.empty())
	{
		for (itList = m_RecvCacheDataQueue.begin(); itList != m_RecvCacheDataQueue.end(); itList++)
		{
			pBusinessData = *itList;
			if (pBusinessData)
			{
				if (pBusinessData->pData)
				{
					delete pBusinessData->pData;
				}
				delete pBusinessData;
				pBusinessData = NULL;
			}
		}

		m_RecvCacheDataQueue.clear();
	}

	if (m_pRecvDataOut) delete[]m_pRecvDataOut;

	m_pRecvDataOut = NULL;

	return TRUE;
}

//清空缓冲区数据队列 清除所有区数据
BOOL CNetworkService::ClearAllDataCache()
{
	ReleaseRecvDataOut();
	ResetDataFlowCount();
	m_bIsDomainName = true;

	if (ClearSocketPackCacheToSend()
		&& ClearSocketPackCacheToRecv()
		&& ClearBusinessLayerToSendCache()
		&& ClearBusinessLayerToRecvCache())
	{
		return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////
//tcp客户端

CNetworkTcpClient::CNetworkTcpClient()
{
}

CNetworkTcpClient::~CNetworkTcpClient()
{
}

//链接服务器
bool CNetworkTcpClient::Start()
{
	//启动网络服务
	if (!StartNetService()) return FALSE;

	//链接服务器
	BOOL bConnectServer = FALSE;
	if (m_bIsDomainName)
	{
		bConnectServer = Link((char *)m_strServerAddr.c_str(), m_ServerPort);
	}
	else
	{
		SetAddrInfo((char *)m_strServerAddr.c_str(), strlen((char *)m_strServerAddr.c_str()), m_ServerPort, AF_INET, TRUE);
		bConnectServer = Connect();
	}

	return bConnectServer;
}


//链接服务器
BOOL CNetworkTcpClient::Link(char *pServerAddr, unsigned short port, bool bClearDataCache)
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

//关闭链接
BOOL CNetworkTcpClient::CloseLink()
{
	return CloseNetService();
}

//向服务器发送数据
bool CNetworkTcpClient::SendDataTo(void *pData, long lDataLen)
{
	//参数校验
	if (!pData) return FALSE;

	if (!AddDataToSend(pData, lDataLen)) return FALSE;

	bool bSend = SendDataToDest(FALSE, TRUE, true);

	return bSend;
}

//从服务器获取数据
bool CNetworkTcpClient::RecvDataFrom(void **pData, long *lDataLen)
{
	//参数校验
	if ((!pData) || (!lDataLen)) return FALSE;

	if (!RecvDataFromSrc()) return FALSE;

	return GetOutRecvData(pData, lDataLen);
}

//更新配置（必须断开链接）
BOOL CNetworkTcpClient::RefreshConfig(char *host/*主机*/, int hostlen/*长度*/, unsigned short port/*端口*/, bool bIsDomainName/*是否域名*/)
{
	if (!host || hostlen <= 0) return FALSE;

	if (!m_strServerAddr.empty()) m_strServerAddr.clear();

	m_strServerAddr = host;
	m_ServerPort = port;
	m_bIsDomainName = bIsDomainName;

	return TRUE;
}


//链接远程
BOOL CNetworkTcpClient::Connect()
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

/////////////////////////////////////////////////////////////

//服务器
CNetworkTcpServer::CNetworkTcpServer()
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
	//SetRsaPublicKey((unsigned char *)szClientPublicKey);
	//SetRsaPrivateKey((unsigned char *)szServerPrivateKey);

	//异步IO参数
	m_NumOfClntSock = 0;

	//m_pMainWind = NULL;
}

CNetworkTcpServer:: ~CNetworkTcpServer()
{
}

//启动服务器
bool CNetworkTcpServer::Start()
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

		//启动监视
		WaitForEvent();

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "CTcpServer::StartServer 结束=");
		strErr.append(rizhi);

		//异常5
		iAbort++;

	}
	catch (...)
	{
		//日志
		//memset(rizhi, 0, sizeof(rizhi));
		//sprintf(rizhi, "CTcpServer StartServer:异常：%s\r\n", (char *)strErr.c_str());
	}


	//启动监视
	//WaitForEvent();

	return TRUE;
}

//关闭服务器
BOOL CNetworkTcpServer::CloseServer()
{
	//清理异步io环境
	//断开套接字
	return CloseNetService();
}

//更新配置	(必须先关闭服务器)
BOOL CNetworkTcpServer::RefreshConfig(char *host/*主机*/, int hostlen/*长度*/, unsigned short port/*端口*/, bool bIsDomainName/*是否域名*/)
{
	if (!host || hostlen <= 0) return FALSE;

	if (!m_strServerAddr.empty()) m_strServerAddr.clear();

	m_strServerAddr = host;
	m_ServerPort = port;
	m_bIsDomainName = bIsDomainName;

	return TRUE;
}


//绑定地址
BOOL CNetworkTcpServer::Bind(char *ip, int iplen, unsigned short port, unsigned short family)
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

//监听端口
BOOL CNetworkTcpServer::Listen(int iBacklog)
{
	int iRet = listen(m_nSocket, iBacklog);
	if (iRet == 0)
	{
		return TRUE;
	}

	return FALSE;
}

//确定链接并获取客户端信息
BOOL CNetworkTcpServer::Accept(SOCKET *pSocketFDForClient, CAddr *pRemoteAddr)
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
bool CNetworkTcpServer::RecvDataFrom(void **pData, long *lDataLen)
{
	if (!RecvDataFromSrc()) return FALSE;

	return GetOutRecvData(pData, lDataLen);
}

//发送数据
bool CNetworkTcpServer::SendDataTo(void *pData, long lDataLen)
{
	if (!AddDataToSend(pData, lDataLen)) return FALSE;

	return SendDataToDest(FALSE, TRUE, true);
}

//配置异步IO模型
BOOL CNetworkTcpServer::SetAsynchronousServer()
{
	m_newEvent = WSACreateEvent();

	if (WSAEventSelect(m_nSocket, m_newEvent, FD_ACCEPT | FD_CLOSE | FD_READ) == SOCKET_ERROR)
		return FALSE;

	m_hSockArr[m_NumOfClntSock] = m_nSocket;
	m_hEventArr[m_NumOfClntSock] = m_newEvent;

	m_NumOfClntSock++;
}

//添加监控事件
BOOL CNetworkTcpServer::AddEvent(SOCKET hClntSock)
{
	m_newEvent = WSACreateEvent();
	WSAEventSelect(hClntSock, m_newEvent, FD_READ | FD_CLOSE);

	m_hEventArr[m_NumOfClntSock] = m_newEvent;
	m_hSockArr[m_NumOfClntSock] = hClntSock;

	m_NumOfClntSock++;
	return TRUE;
}

//调整socket数组
void CNetworkTcpServer::CompressSockets(SOCKET hSockArr[], int idx, int total)
{
	int i;
	for (i = idx; i < total; i++)
	{
		hSockArr[i] = hSockArr[i + 1];
	}
}

//调整事件数组
void CNetworkTcpServer::CompressEvents(WSAEVENT hEventArr[], int idx, int total)
{
	int i;
	for (i = idx; i < total; i++)
	{
		hEventArr[i] = hEventArr[i + 1];
	}
}

//监视IO事件 不完善 有可读事件即移除，与客户端保持短链接 需要一个线程去执行可读事件 监听的套接字有限需要开辟多个线程执行监听任务 建议扩展成IOCP
BOOL CNetworkTcpServer::WaitForEvent()
{
	//变量定义
	int posInfo, startIdx, i, clntAdrLen;
	SOCKET hClntSock;

	char LogMonitor[256] = { 0 };

	string strErr;
	char rizhi[1024] = { 0 };


	try
	{
		//监视 可以通过改变bMonitor来开启与关闭监视
		bool bMonitor = true;
		while (bMonitor)
		{
			strErr.clear();

			memset(rizhi, 0, sizeof(rizhi));
			sprintf(rizhi, "WaitForEvent 开始=WSAWaitForMultipleEvents(m_NumOfClntSock=%d, )=\r\n", m_NumOfClntSock);
			strErr.append(rizhi);

			posInfo = WSAWaitForMultipleEvents(m_NumOfClntSock, m_hEventArr, FALSE, WSA_INFINITE, FALSE);
			startIdx = posInfo - WSA_WAIT_EVENT_0;

			memset(rizhi, 0, sizeof(rizhi));
			sprintf(rizhi, "startIdx=%d =", startIdx);
			strErr.append(rizhi);

			//判断服务是否中断
			if (bMonitor == false) break;

			memset(rizhi, 0, sizeof(rizhi));
			sprintf(rizhi, "GetMonitor:%s =", bMonitor ? "监视" : "关闭监视");
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

						Accept(&hClntSock);

						//添加事件
						if (m_NumOfClntSock <= WSA_MAXIMUM_WAIT_EVENTS)
						{
							//添加事件监视
							AddEvent(hClntSock);
						}
						else
						{
							memset(LogMonitor, 0, sizeof(LogMonitor));
							sprintf(LogMonitor, "当前连接数：%d\r\n", m_NumOfClntSock);

							//创建线程添加监视 超过监视上限
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

						//向线程池添加任务读取客户端传来的数据

						//短链接	移除监视
						WSACloseEvent(m_hEventArr[sigEventIdx]);

						m_NumOfClntSock--;
						CompressSockets(m_hSockArr, sigEventIdx, m_NumOfClntSock);
						CompressEvents(m_hEventArr, sigEventIdx, m_NumOfClntSock);
					}

					//关闭事件
					if (m_netEvents.lNetworkEvents&FD_CLOSE)
					{
						if (m_netEvents.iErrorCode[FD_CLOSE_BIT] != 0)
						{
							//CLOSE ERROR
							break;
						}

						WSACloseEvent(m_hEventArr[sigEventIdx]);
	
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
	}
	catch (...)
	{
		//异常日志
		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, " CTcpServer::WaitForEvent 异常报告：%s\r\n", strErr.c_str());
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////
//http服务
///////////////////////////////////////////////////////////////////////////
//初始化socket环境
CHttpService::CHttpService(LPTSTR lpszRecvLockerName, bool bLockAtOnce, bool bInitiallyOwn, bool bManualReset)
{
	//m_BusinessLayerCache = NULL;
	m_pRecvDataOut = NULL;
	m_ServerPort = 0;
	m_bIsDomainName = true;
	ClearAllDataCache();
	m_HttpBackStatusCode = -1;		//成功的代码

	//初始化锁
	//m_pRecvCacheLocker = new CSynLocker(lpszRecvLockerName, bLockAtOnce, bInitiallyOwn, bManualReset);
}

CHttpService::~CHttpService()
{
	//清空队列
	ClearAllDataCache();

	//if (m_pRecvCacheLocker)
	//{
	//	m_pRecvCacheLocker->Release();
	//}
}

//启动网络服务
BOOL CHttpService::StartService()
{
	//清空队列
	ClearAllDataCache();

	//创建socket
	return CreateSocket();
}


//链接服务器
BOOL CHttpService::Link(char *pServerAddr, unsigned short port, bool bClearDataCache)
{
	//参数校验
	if (pServerAddr == NULL) return FALSE;

	//清空缓冲区
	if (bClearDataCache) ClearAllDataCache();

	//设置地址
	if (!SetAddrInfo(pServerAddr, strlen(pServerAddr), port, AF_INET))
	{
		return FALSE;
	}

	return Connect();
}

//断开链接
BOOL CHttpService::CloseService()
{
	Close();

	//清空队列
	ClearAllDataCache();

	return TRUE;
}


//链接远程
BOOL CHttpService::Connect()
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


//业务逻辑层数据添加
BOOL CHttpService::AddDataToSend(void *pData, long lDataLen)
{
	//参数校验
	if (!pData) return FALSE;

	//变量定义
	CACHE_T *pCache = new CACHE_T;
	if (!pCache) return FALSE;
	memset(pCache, 0, S_CACHE_T);

	//测试明文
	pCache->pData = new char[lDataLen];
	if (!pCache->pData)
	{
		delete pCache;
		return FALSE;
	}
	memset(pCache->pData, 0, lDataLen);
	memmove(pCache->pData, pData, lDataLen);
	pCache->lDataLen = lDataLen;

	//加密
	//if (!DataEncrypt(pData, &lDataLen, &pCache->pData, &pCache->lDataLen))
	//{
	//	if (pCache) delete pCache;
	//	return FALSE;
	//}

	//插入队列
	//m_BusinessLayerCache = pCache;
	m_CacheDataQueue.push_back(pCache);

	//缓冲区长度增加
	m_lCacheDataSize += pCache->lDataLen;

	return TRUE;
}

//包分析
BOOL CHttpService::CreateSocketPacketToSend(long &lPackCount)
{
	//条件判断
	if (m_CacheDataQueue.empty()) return FALSE;

	//变量定义
//	long lPackCountTmp = 0;

	//清空待发送包缓冲区数据
	ClearSocketPackCacheToSend();

	//开空间
	unsigned char *ucpCacheTmp = new unsigned char[m_lCacheDataSize];
	if (!ucpCacheTmp) return FALSE;
	memset(ucpCacheTmp, 0, m_lCacheDataSize);

	//从缓存中转移数据
	unsigned char *pBusinessDataMoveTmp = ucpCacheTmp;
	while (!m_CacheDataQueue.empty())
	{
		memmove(pBusinessDataMoveTmp, m_CacheDataQueue.front()->pData, m_CacheDataQueue.front()->lDataLen);
		pBusinessDataMoveTmp += m_CacheDataQueue.front()->lDataLen;



		//释放空间
		delete m_CacheDataQueue.front()->pData;
		delete m_CacheDataQueue.front();
		//m_CacheDataQueue.front() = NULL;
		m_CacheDataQueue.pop_front();
	}

	//清理空间
	m_CacheDataQueue.clear();
	
	//业务层数据
	pBusinessDataMoveTmp = ucpCacheTmp;

	//变量定义
	long lCacheDataSizeTmp = m_lCacheDataSize;	//数据大小
	long lSingleDataSize = 0;
	long lPacketTotalSize = 0;
	char *pPacketContainer = NULL;	//包流
	char *pPacketContainerTmp = pPacketContainer;
	CACHE_T *pDataCacheToSend = NULL;

	//fortest
	long lCountLen = 0;

	{
		//业务层长度
		lSingleDataSize = ((lCacheDataSizeTmp > HTTP_MAX_DATA_LIMIT) ? HTTP_MAX_DATA_LIMIT : lCacheDataSizeTmp);
		lCacheDataSizeTmp -= lSingleDataSize;

		//包总长
		lPacketTotalSize =  lSingleDataSize;

		//fortest
		lCountLen += lPacketTotalSize;

		//开空间存包
		pPacketContainer = new char[lPacketTotalSize];
		if (!pPacketContainer) return FALSE;
		memset(pPacketContainer, 0, lPacketTotalSize);
		pPacketContainerTmp = pPacketContainer;

		
		//业务层
		memmove(pPacketContainerTmp, pBusinessDataMoveTmp, lSingleDataSize);
		pBusinessDataMoveTmp += lSingleDataSize;
		pPacketContainerTmp += lSingleDataSize;

		//开空间
		pDataCacheToSend = new CACHE_T;
		if (!pDataCacheToSend) return FALSE;
		memset(pDataCacheToSend, 0, S_CACHE_T);
		pDataCacheToSend->pData = pPacketContainer;
		pDataCacheToSend->lDataLen = lPacketTotalSize;

		//一个包完成压入待发包缓冲区
		m_SocketPackCacheToSend.insert(map<long, CACHE_T *>::value_type(0, pDataCacheToSend));
	}

	//释放业务层空间
	delete[]ucpCacheTmp;

	//业务层数据缓冲区重置
	ClearBusinessLayerToSendCache();

	//包总数
	lPackCount = m_SocketPackCacheToSend.size();

	return TRUE;

}

//发送数据包
BOOL CHttpService::SendDataToDest(BOOL bSocketBlockMode)
{
	//条件校验
	//是否已经创建socket
	//	AfxMessageBox(TEXT("SendDataToDest 0"));
	if (m_nSocket < 0) return FALSE;
	//AfxMessageBox(TEXT("SendDataToDest 1"));
	//是否已经链接
	//判断是否已链接
	//if (!GetNetConnectStatus()) return FALSE;
	//	AfxMessageBox(TEXT("SendDataToDest 2"));
	//if (m_CacheDataQueue.empty()) return FALSE;

	//	AfxMessageBox(TEXT("SendDataToDest 3"));

	long lPackCount = 0;

	//分析并组装包(加密业务层数据)
	if (!CreateSocketPacketToSend(lPackCount)) return FALSE;

	//有无数据要发送
	if (m_SocketPackCacheToSend.empty()) return FALSE;
	//AfxMessageBox(TEXT("SendDataToDest 4"));

	//设置阻塞模式
	if (bSocketBlockMode)
	{
		if (!SetBlockMode()) return FALSE;
	}
	else
	{
		if (!SetNoBlockMode()) return FALSE;
	}

	//发包过程
	char RecvCache[SOCKET_PACK_SIZE_MAX_LIMIT] = { 0 };
	long lRecvCacheSize = sizeof(RecvCache);
	BOOL bRecvReturn = FALSE;
	//fortest
	long lCountSuccess = 0;
	long lCountLen = 0;
	if (!m_SocketPackCacheToSend.empty())
	{
		//发包
		for (long i = 0; i < m_SocketPackCacheToSend.size(); i++)
		{
			//取包
			CACHE_T *pData = m_SocketPackCacheToSend[i];

			//TCHAR szRizhi[100] = { 0 };
			//TCHAR szResult[200] = { 0 };
			//CWriteDailyLog::AnsiToUnicode((char *)(((char *)pData->pData)), szRizhi, 100 * sizeof(TCHAR));
			//_sntprintf(szResult, 200, TEXT("%s, len %d "), szRizhi, pData->lDataLen);

			//发送一个包
			if (SendData(pData->pData, &pData->lDataLen) == FALSE) return FALSE;

		}
	}


	//发送完毕 清空待发送任务队列
	ClearSocketPackCacheToSend();

	return TRUE;

	//(从待发包序列取一个包，并send给服务端，等待服务端发回接收完成应答再发下一个包（若1s内未回复重发）如此循环直到把包发完，最后清空队列)
	//发包结束
}

//接收数据包
//1.以一个数据包最大限制来接收数据包，
//2.定位包头包尾（一个包的头两个字节为包头，一个包的后两个字节为包尾）
//3.定位包数据长度（包接收的字节数应该与之相符）(断包半包等不予接收，进行超时接收等待,若限定时间内未接收到完整包则断开链接接收失败并清理数据)
//4.定位包序号和总包数（用于计数接收次数）
//5.维护一个去重包序列，已接收的包不需要再接收，但表示接收成功

//压入待接收缓冲区并返回接收成功应答包

//退出while
//检测接收包完整性（包的个数）
//进行拆包并拼接业务层，拼接完成后解密业务层，送入等待接收业务层缓冲区
//完成接收
BOOL CHttpService::RecvDataFromSrc(long lTvPerSec, long lTvPerUsec, int iTimeOutCount,  BOOL bSocketBlockMode)
{
	//MessageBox(NULL, TEXT("RecvDataFromSrc 01"), NULL, 0);
	//条件校验
	//是否已经创建socket
	if (m_nSocket < 0) return FALSE;
	//是否已经链接
	//判断是否已链接
	//if (!GetNetConnectStatus()) return FALSE;
	//	if (!m_SocketPackCacheToRecv.empty()) return FALSE;

	//清空待接收数据缓冲区 腾出空间 socket包缓冲
	//ClearBusinessLayerToRecvCache();
	ClearSocketPackCacheToRecv();
	//MessageBox(NULL, TEXT("RecvDataFromSrc 02"), NULL, 0);
	//设置阻塞模式
	if (bSocketBlockMode)
	{
		if (!SetBlockMode()) return FALSE;
	}
	else
	{
		if (!SetNoBlockMode()) return FALSE;
	}

	//	MessageBox(NULL, TEXT("RecvDataFromSrc 03"), NULL, 0);


	//接收包
	char recvdata[HTTP_MAX_DATA_LIMIT] = { 0 };
	long lRecvDataLen = 0;
	lRecvDataLen = HTTP_MAX_DATA_LIMIT;
	BOOL bRecvSuccess = FALSE;
	int iCountTimeOut = iTimeOutCount;
	//	int iCountSuccessRecv = 0;		//成功接收总数
	long lHttpDataLen = 0;				//包总长
	long lEffectDataLen = 0;			//有效数据长度
	long lUnEffectDataLen = 0;			//无效数据 http返回的header 非有效内容
	long lPacketSeq = -1;
	//ACCESS_LAYER_T *pAccessLayer = NULL;

	CACHE_T *pDataCacheSave = NULL;
	m_lRecvCacheDataSize = 0;		//待

	//fortest
	long lCountSuccess = 0;
	long lCountLen = 0;

	//http参数
	string strHttpResult;

	while (iCountTimeOut-->0)
	{
		//接收

		
		//ucPacketNum = 1;
		lPacketSeq++;
		//pAccessLayer = NULL;
		pDataCacheSave = NULL;
		memset(recvdata, 0, HTTP_MAX_DATA_LIMIT);

		//超时接收 100ms 超时监控
		if (DelayMonitorSocket(lTvPerSec, lTvPerUsec))
		{
			lPacketSeq--;
			continue;
		}

	//	if (lHttpDataLen > 0)
	//	{
	//		lRecvDataLen = lHttpDataLen - m_lRecvCacheDataSize;
	//	}
	//	else
		//{
	//		lRecvDataLen = HTTP_MAX_DATA_LIMIT;
	//	}
		lRecvDataLen = HTTP_MAX_DATA_LIMIT;
		bRecvSuccess = FALSE;
		bRecvSuccess = RecvData(recvdata, &lRecvDataLen);

		//	TCHAR szRizhi[100] = { 0 };
		//	TCHAR szResult[200] = { 0 };
		//CWriteDailyLog::AnsiToUnicode((char *)(((char *)pData->pData) + 10), szRizhi, 100 * sizeof(TCHAR));
		//	_sntprintf(szResult, 200, TEXT("%s, len %d "), bRecvSuccess ? TEXT("成功recv") : TEXT("失败recv"), lRecvDataLen);
		//MessageBox(NULL, szResult, NULL, 0);

		//写日志
		//if (lRecvDataLen < 0)
		//{
		//	CWriteDailyLog::WriteLog("接收小于0", strlen("接收小于0"), "Recv.txt");
		//}
		//else if (lRecvDataLen==0)
		//{
		//	CWriteDailyLog::WriteLog("接收等于0", strlen("接收等于0"), "Recv.txt");
		//}
		

		if (!bRecvSuccess) continue;

		//缓存超标处理

		iCountTimeOut = iTimeOutCount;

		//写日志
		//CWriteDailyLog::WriteLog("接收到的数据:", strlen("接收到的数据:"), "Recv.txt");
		//CWriteDailyLog::WriteLog(recvdata, lRecvDataLen, "Recv.txt");

		//获取数据	http返回header处理
		if (lPacketSeq == 0)
		{
			strHttpResult = recvdata;

			//请求成功返回代码
			long lCodePos = strHttpResult.find("HTTP/", 0);
			if (lCodePos == string::npos)
			{
				lPacketSeq--;
				iCountTimeOut = iTimeOutCount;
				continue;
			}
			long lCodeEndPos = strHttpResult.find("\r\n", lCodePos + strlen("HTTP/"));
			if (lCodeEndPos == string::npos)
			{
				lPacketSeq--;
				iCountTimeOut = iTimeOutCount;
				continue;
			}
			string strCode;
			strCode.assign(strHttpResult, lCodePos, lCodeEndPos - lCodePos);	//取得包含code的返回代码
			long lCodeNumPos = strCode.find(" ", 0);
			if (lCodeNumPos == string::npos)
			{
				lPacketSeq--;
				iCountTimeOut = iTimeOutCount;
				continue;
			}
			long lCodeNumEndPos = strCode.find(" ", lCodeNumPos + 1);
			if (lCodeNumEndPos == string::npos)
			{
				lPacketSeq--;
				iCountTimeOut = iTimeOutCount;
				continue;
			}
			string strCodeNum;
			strCodeNum.assign(strCode, lCodeNumPos + 1, lCodeNumEndPos - lCodeNumPos - 1);
			m_HttpBackStatusCode = atol((char *)strCodeNum.c_str());		//状态码

			/*int iPos=strHttpResult.find("Content-Length: ", 0);	//有些没有返回Content-Length
			if (iPos == string::npos)
			{
				//写日志
				CWriteDailyLog::WriteLog("找不到正文长度", strlen("找不到正文长度"), "Recv.txt");
				ucPacketSeq--;
				iCountTimeOut = iTimeOutCount;
				continue;
			}
			int iStartPos = strHttpResult.find(" ", iPos + 1);
			int iEndPos = strHttpResult.find("\r\n", iStartPos + 1);
			if (iEndPos == string::npos)
			{
				ucPacketSeq--;
				iCountTimeOut = iTimeOutCount;
				continue;
			}

			char HttpDataLen[128] = { 0 };
			memmove(HttpDataLen, (char *)strHttpResult.substr(iStartPos + 1, iEndPos).c_str(), iEndPos - iStartPos);
			//lHttpDataLen = atol(HttpDataLen) + iEndPos+4;//将前面的数据加起来 4是2个换行符
			lEffectDataLen = atol(HttpDataLen);				//有效正文长度*/

			//寻找返回的有效正文
			long lEffectContentPos = strHttpResult.find("\r\n\r\n", lCodeNumEndPos);
			if (lEffectContentPos == string::npos)
			{
				lPacketSeq--;
				iCountTimeOut = iTimeOutCount;
				continue;
			}

			lUnEffectDataLen = lEffectContentPos + strlen("\r\n\r\n");
			//lHttpDataLen = lEffectDataLen + lUnEffectDataLen;			//无效头信息与正文总长
			lEffectDataLen = lRecvDataLen - lUnEffectDataLen;			//得到当前有效数据长度

			//保存http头信息
			if (!m_strHttpReturnHeader.empty()) m_strHttpReturnHeader.clear();
			m_strHttpReturnHeader.assign(strHttpResult, 0, lEffectContentPos);

			//寻找有效内容大小
			lCodePos = m_strHttpReturnHeader.find("Content-Length:", 0);
			if (lCodePos != string::npos)
			{
				lCodeEndPos = m_strHttpReturnHeader.find("\r\n", lCodePos + strlen("Content-Length:"));
				if (lCodeEndPos != string::npos)
				{
					string strEffectDataLen;
					strEffectDataLen.assign(m_strHttpReturnHeader, lCodePos + strlen("Content-Length:"), lCodeEndPos - (lCodePos + strlen("Content-Length:")));
					//去首尾空格
					int iSpacePos = 0, iSpaceEndPos=string::npos;
					if((iSpacePos = strEffectDataLen.find_first_not_of(' ', iSpacePos) != string::npos))
					{
						iSpaceEndPos = strEffectDataLen.length() - 1;
						m_stContentFlowCount.lTotalFlowSize = atol((char *)strEffectDataLen.substr(iSpacePos, iSpaceEndPos - iSpacePos + 1).c_str());
					}
				}
			}

			//if (!m_strHttpReturnHeader.empty())
		//	{
		//		CWriteDailyLog::WriteLog((char *)m_strHttpReturnHeader.c_str(), m_strHttpReturnHeader.length(), "httpHeadInfo.txt");
		//	}

			//char dd[100] = { 0 }; 
			//sprintf(dd, "lHttpDataLen 长度：%ld; lEffectDataLen:%d; lUnEffectDataLen:%d", lHttpDataLen, lEffectDataLen, lUnEffectDataLen);
		//	CWriteDailyLog::WriteLog(dd, strlen(dd), "Recv长度.txt");
			
		}


		//接入层
		//pAccessLayer = ((ACCESS_LAYER_T *)(recvdata + sizeof(unsigned short)));

		//校验包总长
		//usPacketSize = *((unsigned short *)(recvdata + sizeof(unsigned short)));
		//usPacketSize = pAccessLayer->usPacketSize;
		//不返回成功应打包客户端会超时重发
		//if (usPacketSize != lRecvDataLen) continue;

		//包总数
	//	ucPacketNum = pAccessLayer->ucPacketNum;

		//包序号
	//	ucPacketSeq = pAccessLayer->ucPacketSeq;

		//包流水号
		//uTransID = pAccessLayer->uTransID;


		//开空间保存业务逻辑层数据
		pDataCacheSave = new CACHE_T;
		if (!pDataCacheSave) return FALSE;
		memset(pDataCacheSave, 0, S_CACHE_T);
		if (lPacketSeq == 0)
		{
			pDataCacheSave->lDataLen = lRecvDataLen - lUnEffectDataLen;	//第一个包包含了无效数据
		}			
		else
		{
			pDataCacheSave->lDataLen = lRecvDataLen;
		}

		//写日志
		//char len[100] = { 0 };
		//sprintf(len, "lRecvDataLen:%d; 有效：%d", lRecvDataLen, pDataCacheSave->lDataLen);
		//CWriteDailyLog::WriteLog(len, strlen(len), "Recv.txt");
		//CWriteDailyLog::WriteLog(recvdata, lRecvDataLen, "Recv.txt");

		pDataCacheSave->pData = new char[pDataCacheSave->lDataLen];
		if (!pDataCacheSave->pData)
		{
			delete pDataCacheSave;
			return FALSE;
		}
		memset(pDataCacheSave->pData, 0, pDataCacheSave->lDataLen);
		if (lPacketSeq == 0)
		{
			memmove(pDataCacheSave->pData, (char *)(recvdata + lUnEffectDataLen), pDataCacheSave->lDataLen);
		}
		else
		{
			memmove(pDataCacheSave->pData, (char *)(recvdata), pDataCacheSave->lDataLen);
		}
		
		//当前有效数据流量
		m_stContentFlowCount.lCurrentFlowCount += pDataCacheSave->lDataLen;

		//压入待接收socket缓冲
		if (m_SocketPackCacheToRecv.find(lPacketSeq) == m_SocketPackCacheToRecv.end())
		{
			//累加包长度用于开空间组装业务层数据
			m_lRecvCacheDataSize += pDataCacheSave->lDataLen;

			m_SocketPackCacheToRecv.insert(map<long, CACHE_T*>::value_type(lPacketSeq, pDataCacheSave));

			//发送成功接收应打包
			ClearBusinessLayerToSendCache();
			
			//TCHAR szRizhi[100] = { 0 };
			//TCHAR szResult[200] = { 0 };
			//CWriteDailyLog::AnsiToUnicode((char *)(((char *)pDataCacheSave->pData)), szRizhi, 100 * sizeof(TCHAR));
			//_sntprintf(szResult, 200, TEXT("%s; %s, 数据：%s, len %d "), bRet ? TEXT("回执发送成功") : TEXT("回执发送失败"), bRecvSuccess ? TEXT("成功recv") : TEXT("失败recv"), szRizhi, m_lRecvCacheDataSize);
			//MessageBox(NULL, szResult, NULL, 0);

			//fortest
			lCountSuccess++;


			//接收成功重置
			iCountTimeOut = iTimeOutCount;

			//检测是否接收完成
			//if (m_lRecvCacheDataSize >= lEffectDataLen) break;
			//if (m_SocketPackCacheToRecv.size() == ucPacketNum) break;
		}
	}

	if (!bRecvSuccess&&lRecvDataLen<0)
	{
		//AfxMessageBox(TEXT("接收失败"));
		return FALSE;
	}
	//AfxMessageBox(ridd);

	//拼接接收到的业务层数据
	CreateBussnessLayerToGet();

	//清理socket等待接收缓冲区
	//	ClearSocketPackCacheToRecv();

	return TRUE;
}

//解包(并解密)将socket待接收缓冲中数据拼接成业务层数据 得到一个业务逻辑层数据
BOOL CHttpService::CreateBussnessLayerToGet()
{
	//参数校验
	if (m_SocketPackCacheToRecv.empty() || m_lRecvCacheDataSize == 0) return FALSE;

	//开空间
	char *pBusinessLayerData = new char[m_lRecvCacheDataSize];
	if (!pBusinessLayerData) return FALSE;
	memset(pBusinessLayerData, 0, m_lRecvCacheDataSize);
	char *pDataTmp = pBusinessLayerData;

	//按顺序取数据 size，从0开始到size
	long lPacketCount = m_SocketPackCacheToRecv.size();
	long lBusinessTotalSize = 0;
	for (long i = 0; i < lPacketCount; i++)
	{
		if (m_SocketPackCacheToRecv[i])
		{
			if (m_SocketPackCacheToRecv[i]->pData)
			{
				memmove(pDataTmp, m_SocketPackCacheToRecv[i]->pData, m_SocketPackCacheToRecv[i]->lDataLen);
				pDataTmp += m_SocketPackCacheToRecv[i]->lDataLen;
				lBusinessTotalSize += m_SocketPackCacheToRecv[i]->lDataLen;
				delete[]m_SocketPackCacheToRecv[i]->pData;
				delete m_SocketPackCacheToRecv[i];
				m_SocketPackCacheToRecv[i] = NULL;
			}
		}
	}

	//校验数据完整性
	if (lBusinessTotalSize != m_lRecvCacheDataSize)
	{
		delete[]pBusinessLayerData;
		return FALSE;
	}

	//解密数据
	CACHE_T *pBusinessDataCache = new CACHE_T;
	if (!pBusinessDataCache)
	{
		delete[]pBusinessLayerData;
		return FALSE;
	}
	memset(pBusinessDataCache, 0, sizeof(CACHE_T));

	

	//test;
	pBusinessDataCache->pData = new char[m_lRecvCacheDataSize];
	if (!pBusinessDataCache->pData)
	{
		delete[]pBusinessLayerData;
		return FALSE;
	}
		memset(pBusinessDataCache->pData, 0, m_lRecvCacheDataSize);
		memmove(pBusinessDataCache->pData, pBusinessLayerData, m_lRecvCacheDataSize);
		pBusinessDataCache->lDataLen = m_lRecvCacheDataSize;

	m_lRecvCacheDataSize = pBusinessDataCache->lDataLen;

	//清空数据腾出空间
	ClearBusinessLayerToRecvCache();

	//将数据送入业务层缓冲区
	if (pBusinessDataCache->pData)
	{
		m_RecvCacheDataQueue.push_back(pBusinessDataCache);
	}

	//清理空间
	if (pBusinessLayerData) delete[]pBusinessLayerData;

	//清除待接收数据缓冲区socket
	ClearSocketPackCacheToRecv();

	return TRUE;
}

//取出接收到的数据
BOOL CHttpService::GetOutRecvData(void **pData, long *lDataLen)
{
	//参数校验
	if (pData == NULL || lDataLen == NULL) return FALSE;
	if (m_RecvCacheDataQueue.empty()) return FALSE;

	char **pDataTmp = (char **)pData;

	//AfxMessageBox(TEXT("GetOutRecvData 01"));

	//取数据
	//开空间
	//数据迁移
	//清理空间
	//完成
	while (!m_RecvCacheDataQueue.empty())
	{
		//AfxMessageBox(TEXT("GetOutRecvData DD"));
		CACHE_T *pCacheData = m_RecvCacheDataQueue.front();
		if (pCacheData)
		{
			*pDataTmp = new char[pCacheData->lDataLen];
			if (NULL == *pDataTmp) return FALSE;
			memset(*pDataTmp, 0, pCacheData->lDataLen);
			memmove(*pDataTmp, pCacheData->pData, pCacheData->lDataLen);
			*lDataLen = pCacheData->lDataLen;
			delete[]pCacheData->pData;
			delete pCacheData;
			pCacheData = NULL;
			m_RecvCacheDataQueue.pop_front();

			//保存用于释放
			m_pRecvDataOut = *pDataTmp;

			//	delete[] * pData;
			//	AfxMessageBox(TEXT("GetOutRecvData DD 2"));

			break;
		}
		else
		{
			m_RecvCacheDataQueue.pop_front();
		}
	}

	//AfxMessageBox(TEXT("GetOutRecvData 02"));
	return TRUE;

}

//释放取出来的数据
BOOL CHttpService::ReleaseRecvDataOut()
{
	if (m_pRecvDataOut)
	{
		delete[] m_pRecvDataOut;
		m_pRecvDataOut = NULL;
	}

	return TRUE;
}


//清空待发包缓冲区  3区中的send
BOOL CHttpService::ClearSocketPackCacheToSend()
{
	//变量定义
	map<long, CACHE_T*>::iterator itCacheTmp;
	CACHE_T* pCacheTmpValue = NULL;

	//清空待发包序列
	if (!m_SocketPackCacheToSend.empty())
	{
		for (itCacheTmp = m_SocketPackCacheToSend.begin(); itCacheTmp != m_SocketPackCacheToSend.end(); itCacheTmp++)
		{
			pCacheTmpValue = itCacheTmp->second;
			if (pCacheTmpValue)
			{
				if (pCacheTmpValue->pData) delete pCacheTmpValue->pData;
				delete pCacheTmpValue;
				pCacheTmpValue = NULL;
			}
		}

		m_SocketPackCacheToSend.clear();
	}

	return TRUE;
}


//清空待接收包缓冲区 3区中的recv
BOOL CHttpService::ClearSocketPackCacheToRecv()
{
	//变量定义
	map<long, CACHE_T*>::iterator itCacheTmp;
	CACHE_T* pCacheTmpValue = NULL;

	//清空待发包序列
	if (!m_SocketPackCacheToRecv.empty())
	{
		for (itCacheTmp = m_SocketPackCacheToRecv.begin(); itCacheTmp != m_SocketPackCacheToRecv.end(); itCacheTmp++)
		{
			pCacheTmpValue = itCacheTmp->second;
			if (pCacheTmpValue)
			{
				if (pCacheTmpValue->pData) delete pCacheTmpValue->pData;
				delete pCacheTmpValue;
				pCacheTmpValue = NULL;
			}
		}

		m_SocketPackCacheToRecv.clear();
	}

	return TRUE;
}

//socket业务层数据缓冲区待发送清理 1区
BOOL CHttpService::ClearBusinessLayerToSendCache()
{
	m_lCacheDataSize = 0;

	//变量定义
	list<CACHE_T *>::iterator itList;
	CACHE_T * pBusinessData = NULL;

	if (!m_CacheDataQueue.empty())
	{
		for (itList = m_CacheDataQueue.begin(); itList != m_CacheDataQueue.end(); itList++)
		{
			pBusinessData = *itList;
			if (pBusinessData)
			{
				if (pBusinessData->pData)
				{
					delete pBusinessData->pData;
				}
				delete pBusinessData;
				pBusinessData = NULL;
			}
		}

		m_CacheDataQueue.clear();
	}

	return TRUE;
}

//socket业务层数据缓冲区待接收清理 2区
BOOL CHttpService::ClearBusinessLayerToRecvCache()
{
	m_lRecvCacheDataSize = 0;


	//变量定义
	list<CACHE_T *>::iterator itList;
	CACHE_T * pBusinessData = NULL;

	if (!m_RecvCacheDataQueue.empty())
	{
		for (itList = m_RecvCacheDataQueue.begin(); itList != m_RecvCacheDataQueue.end(); itList++)
		{
			pBusinessData = *itList;
			if (pBusinessData)
			{
				if (pBusinessData->pData)
				{
					delete pBusinessData->pData;
				}
				delete pBusinessData;
				pBusinessData = NULL;
			}
		}

		m_RecvCacheDataQueue.clear();
	}

	if (m_pRecvDataOut) delete[]m_pRecvDataOut;

	m_pRecvDataOut = NULL;

	return TRUE;
}

//清理http头信息
BOOL CHttpService::ClearHttpHeader()
{
	m_strHttpReturnHeader.clear();
	m_HttpBackStatusCode = -1;		//成功的代码
	return TRUE;
}

//清理流量统计
void CHttpService::ResetEffectFlowCount()
{
	memset(&m_stContentFlowCount, 0, sizeof(m_stContentFlowCount));
}

//清空缓冲区数据队列 清除所有区数据
BOOL CHttpService::ClearAllDataCache()
{
	ReleaseRecvDataOut();
	ResetEffectFlowCount();

	if (ClearSocketPackCacheToSend()
		&& ClearSocketPackCacheToRecv()
		&& ClearBusinessLayerToSendCache()
		&& ClearBusinessLayerToRecvCache()
		&& ClearHttpHeader())
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////

#define CLIENT_HTTP_GET_API_STR				("GET %s HTTP/1.1\r\nHost:%s\r\nAccept:*/*\r\n"\
	"User-Agent:Mozilla/4.0 (compatible; MSIE 6.00; Windows NT 5.1)\r\nPragma: no-cache\r\nCache-Control: no-cache\r\nConnection:Keep-Alive\r\n\r\n")

//http客户端
CHttpClient::CHttpClient()
{
	m_pHttpDataCache = NULL;
	m_ulHttpDataSize = 0;
	m_bUseDataCache = false;
}
CHttpClient:: ~CHttpClient()
{
	ReleaseHttpDataCache();
}

//链接服务器
BOOL CHttpClient::ConnectServer()
{
	//启动网络服务
	if (!StartService()) return FALSE;

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
BOOL CHttpClient::CloseLink()
{
	return CloseService();
}

//向服务器发送数据
BOOL CHttpClient::SendDataToServer(void *pData, long lDataLen)
{
	//参数校验
	if (!pData) return FALSE;

	if (!AddDataToSend(pData, lDataLen)) return FALSE;

	BOOL bSend = SendDataToDest();

	return bSend;
}

//从服务器获取数据
BOOL CHttpClient::RecvDataFromServer(void **pData, long *lDataLen, long lTvPerSec, long lTvPerUsec, int iTimeOutCount, BOOL bSocketBlockMode )
{
	//参数校验
	if ((!pData) || (!lDataLen)) return FALSE;

	if (!RecvDataFromSrc(lTvPerSec, lTvPerUsec, iTimeOutCount, bSocketBlockMode)) return FALSE;

	return GetOutRecvData(pData, lDataLen);
}

//更新配置（必须断开链接）
BOOL CHttpClient::RefreshConfig(char *host/*主机*/, int hostlen/*长度*/, unsigned short port/*端口*/, bool bIsDomainName/*是否域名*/)
{
	if (!host || hostlen <= 0) return FALSE;

	if (!m_strServerAddr.empty()) m_strServerAddr.clear();

	m_strServerAddr = host;
	m_ServerPort = port;
	m_bIsDomainName = bIsDomainName;

	return TRUE;
}

//下载文件
BOOL CHttpClient::DownLoadFile(char *szFileLink, long lLinkLen, char *szSaveAsFileName, long lFileNameLen, unsigned short port, bool bDownLoadToMemory, long lSec, long lUSec, int iTimeOutCount, BOOL bSocketBlockMode)
{
	//校验参数
	if (!szFileLink || lLinkLen <= 0 || !szSaveAsFileName || lFileNameLen <= 0) return FALSE;

	string strLink = szFileLink;
	string strHost;

	//初始化
	if (!m_strHttpReturnHeader.empty()) m_strHttpReturnHeader.clear();
	m_HttpBackStatusCode = -1;

	//取出域名
	long lLinkHeadPos = strLink.find("http://", 0);			//找http头
	if (lLinkHeadPos == string::npos) return FALSE;

	long	lHostEndPos = strLink.find("/", lLinkHeadPos + strlen("http://"));		//找域名结尾斜杠/
	if (lHostEndPos == string::npos) return FALSE;

	//获取域名
	strHost.assign(strLink, lLinkHeadPos + strlen("http://"), lHostEndPos - (lLinkHeadPos + strlen("http://")));

	char szGetRequestStr[2048] = { 0 };
	sprintf(szGetRequestStr, CLIENT_HTTP_GET_API_STR, szFileLink, strHost.c_str());

	//缓存
	char *pRecvData = NULL;
	long lRecvDataLen = 0;

	//配置地址端口等
	RefreshConfig((char *)strHost.c_str(), strlen((char *)strHost.c_str()), port, true);

	//连接
	if (!ConnectServer())
	{
		return FALSE;
	}

	//发送请求
	if (SendDataToServer(szGetRequestStr, strlen(szGetRequestStr)))
	{
		BOOL bRet = RecvDataFromServer((void **)&pRecvData, &lRecvDataLen, lSec, lUSec, iTimeOutCount, bSocketBlockMode);

		if (!bRet) return FALSE;

		if (pRecvData&&m_HttpBackStatusCode == HTTP_REQUEST_SUCCESS_RET_CODE)
		{
			if (bDownLoadToMemory)
			{
				//下载到内存
				ReleaseHttpDataCache();

				if (!m_strHttpData.empty()) m_strHttpData.clear();

				if (m_bUseDataCache)
				{
					m_pHttpDataCache = new char[lRecvDataLen];
					memset(m_pHttpDataCache, 0, lRecvDataLen);
					memmove(m_pHttpDataCache, pRecvData, lRecvDataLen);
					m_ulHttpDataSize = lRecvDataLen;
				}
				else
				{
					m_strHttpData = pRecvData;
				}
			}
			else
			{
				//保存文件
				CWriteDailyLog::SaveDataAsFile(pRecvData, lRecvDataLen, szSaveAsFileName);
			}


			//释放缓存
			ReleaseRecvDataOut();
		}
		else
		{
			//释放缓存
			ReleaseRecvDataOut();

			//下载失败
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

//清楚数据缓存
bool CHttpClient::ReleaseHttpDataCache()
{
	try
	{
		if (m_pHttpDataCache)
		{
			delete[]m_pHttpDataCache;
			m_pHttpDataCache = NULL;
			m_ulHttpDataSize = 0;
		}
	}
	catch (...)
	{
		m_pHttpDataCache = NULL;
		m_ulHttpDataSize = 0;
	}

	return true;

}

//使用内存缓存块
bool CHttpClient::UseDataCache(bool bUse)
{
	m_bUseDataCache = bUse;
	return true;
}