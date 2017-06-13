#include"stdafx.h"
#include"TcpSocket.h"
#include<errno.h>
#pragma comment(lib, "ws2_32.lib")		//������API��֧��	WinSock2.h�ļ���ı����ͺ����ķ������ӷ���ws2_32.lib�� ws2_32.dll��֧��

//#include"SocketProtocol.h"
//#include"Dailylog.h"
//#include"rsa.h"




////////////////////////////////////


/////////////////////////////////////////////////////

///��ַ��Ϣ��
CAddr::CAddr()															//Ĭ�Ϲ��첻��ʼ��
{
	Clear();
}

//��ͨ�����ʼ������
CAddr::CAddr(char *szip, unsigned short usPort, unsigned short usFamily)
{
	Clear();
	m_scktst_len = sizeof(m_stAddr_Info);
	memset(&m_stAddr_Info, 0, m_scktst_len);

	//�Ϸ���У��
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

//��λ
void CAddr::Clear()
{
	m_scktst_len = sizeof(m_stAddr_Info);
	memset(&m_stAddr_Info, 0, sizeof(m_stAddr_Info));
	memset(m_DomainName, 0, HOST_ADDR_LEN);
	m_bSetAddr = FALSE;
}

CAddr & CAddr::operator =(CAddr &addr)							//���ظ�ֵ�����
{
	//�Ϸ���У��
	if (addr.GetpAddrInInfo() == NULL || !(addr.GetSetAddrStatus())) return *this;

	//����
	Clear();

	m_stAddr_Info.sin_addr.s_addr = (addr.GetpAddrInInfo())->sin_addr.s_addr;
	m_stAddr_Info.sin_family = (addr.GetpAddrInInfo())->sin_family;
	m_stAddr_Info.sin_port = (addr.GetpAddrInInfo())->sin_port;
	m_bSetAddr = TRUE;
	return *this;
}

void CAddr::Setip(char *szip)											//����ip
{
	//�Ϸ���У��
	if (szip == NULL || strlen(szip) == 0) return;

	m_stAddr_Info.sin_addr.s_addr = inet_addr(szip);
}

char *CAddr::Getip()													//��ȡip
{
	return inet_ntoa(m_stAddr_Info.sin_addr);
}

void CAddr::SetFamily( unsigned short shFamily)							//��������Э��������IPv4����IPv6
{
	m_stAddr_Info.sin_family = shFamily;
}

unsigned short CAddr::GetFamily()										//��ȡ����Э������
{
	return m_stAddr_Info.sin_family;
}

void CAddr::SetPort( unsigned short shPort)							//���ö˿ں�
{
	m_stAddr_Info.sin_port = htons(shPort);
}

unsigned short CAddr::GetPort()										//��ȡ�˿ں�
{
	return ntohs(m_stAddr_Info.sin_port);
}
void CAddr::SetAddrInfo(struct sockaddr_in &stAddr_Info)				//���õ�ַ��ṹ��
{
	//����
	Clear();

	m_scktst_len = sizeof(m_stAddr_Info);
	m_stAddr_Info.sin_addr.s_addr = stAddr_Info.sin_addr.s_addr;
	m_stAddr_Info.sin_family = stAddr_Info.sin_family;
	m_stAddr_Info.sin_port = stAddr_Info.sin_port;
	m_bSetAddr = TRUE;
}

void CAddr::SetAddrInfo(char *szip, unsigned short port, unsigned short family)
{
	//�Ϸ���У��
	if (szip == NULL || strlen(szip) == 0) return;

	//����
	Clear();
	m_scktst_len = sizeof(m_stAddr_Info);
	m_stAddr_Info.sin_addr.s_addr = inet_addr(szip);
	m_stAddr_Info.sin_family = family;
	m_stAddr_Info.sin_port = htons(port);
	m_bSetAddr = TRUE;
}

//��ַ
BOOL CAddr::SetAddrInfoByDomainName(char *domain, int domainlen, unsigned short port, unsigned short family)
{
	//�Ϸ���У��
	if (domain == NULL || strlen(domain) == 0 || domainlen <= 0) return FALSE;

	try
	{
		struct hostent* stpRemote = NULL;
		struct in_addr addr;
		stpRemote = gethostbyname(domain);			//��ȡԶ��������Ϣ
		if (stpRemote == NULL) return FALSE;

		//����
		Clear();

		addr.s_addr = *(u_long *)stpRemote->h_addr_list[0];			//ip��ַ

		//��д��Ϣ
		m_stAddr_Info.sin_family = family;//ipv4Э��
		m_stAddr_Info.sin_port = htons((unsigned short)port);						//�˿�ת���������ֽ���
		m_stAddr_Info.sin_addr.s_addr = *((u_long*)*stpRemote->h_addr_list);		//��ַ
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

//����
char *CAddr::GetDomainName()
{
	if (strlen(m_DomainName) == 0) return NULL;
	if (!gethostbyname(m_DomainName)) return NULL;
	return m_DomainName;
}

struct sockaddr * CAddr::GetpAddrInfo()									//��ȡ��ַ�ṹ�壨ǿתΪ�ɰ�ṹ��ָ��)
{
	return (struct sockaddr *)&(m_stAddr_Info);
}
struct sockaddr_in * CAddr::GetpAddrInInfo() 							//��ȡ��ַ�ṹ���°�
{
	return &(m_stAddr_Info);
}

int CAddr::GetSocketInfoLen()												//��ȡ���͵�ַ�ṹ��ռ��С
{
	return (int)(m_scktst_len);
}

socklen_t* CAddr::GetSocketInfoLenPoint() 										//��ȡ socklen_t ���͵�ַ�ṹ��ռ��Сָ��
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

	//��ʼ��socket����
	if (!m_bInitEnvironment)
		InitSocketEnvironment();
}

CTcpSocket:: ~CTcpSocket()
{
	if (m_bAutoCloseSocket)
		Close();	
}

//����socket����
BOOL CTcpSocket::InitSocketEnvironment()
{
	try
	{
		if (!m_bInitEnvironment)
		{
			//����socket ��ʼ������
			WSADATA wsaData;
			memset(&wsaData, 0, sizeof(WSADATA));
			int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);	//����winsock DLL ÿ�����̵���һ�ξ͹��� �����޷�ʹ��socket
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

//������
BOOL CTcpSocket::CleanSocketEnvironment()
{
	int iRet = 0;
	try
	{
		if (m_bInitEnvironment)
		{
			if ((iRet = WSACleanup()) == SOCKET_ERROR)
			{
				if (WSACancelBlockingCall() == 0)//ȡ�������׽ӿ�
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
	

		//һ�����̵���һ�ξ͹�����ʹ�ù����б�����������쳣

	return iRet == 0 ? true:false;
}

//�׽���
BOOL CTcpSocket::CreateSocket()
{
	//�ȶϿ�
	Close();
	//if (m_bConnected)
	//{
	//	Close();
	//}
	//if (m_nSocket > 0) return TRUE;

	m_nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);			//�����׽���
	if (m_nSocket < 0 || INVALID_SOCKET == m_nSocket)
	{
		m_bConnected = FALSE;
		return FALSE;
	}

	m_bSocketReadable = TRUE;
	m_bSocketWriteable = TRUE;

	return TRUE;
}


//�Ͽ�����
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

//���ŵĶϿ�����
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

//��������
BOOL CTcpSocket::SendData(VOID *pData, long *plDataLen)
{
	//CWriteDailyLog::WriteLog(m_bSocketWriteable ? "��д" : "��д", strlen("��д"), "�ɶ���д.txt");
	//���ɷ���
	if (!m_bSocketWriteable) return FALSE;
	

	//TCHAR szRizhi[100] = { 0 };
	//_sntprintf(szRizhi, 100, TEXT("%s %s"), m_nSocket == INVALID_SOCKET_ID ? TEXT("�׽�����Ч") : TEXT("�׽�����Ч"), m_bConnected ? TEXT("������") : TEXT("δ����"));

	//�Ϸ���֤
	if ((m_nSocket == INVALID_SOCKET_ID)) return FALSE;

	if (pData == NULL || plDataLen == NULL) return FALSE;

	//��������
	long lSendRet = 0;
	if ((lSendRet = send(m_nSocket, (char *)pData, *plDataLen, 0)) == -1)
	{
		//CWriteDailyLog::WriteLog("SendData ʧ�� 0", strlen("SendData ʧ�� 0"), "�ɶ���д.txt");

		*plDataLen = -1;
		//Close();
		return FALSE;
	}
	else if (lSendRet<*plDataLen)
	{
		//CWriteDailyLog::WriteLog("SendData ʧ�� 1", strlen("SendData ʧ�� 0"), "�ɶ���д.txt");

		*plDataLen = lSendRet;
		return FALSE;
	}

	*plDataLen = lSendRet;

	//����ͳ��
	m_stDataFlowCount.lSendTotal += lSendRet;

	//CWriteDailyLog::WriteLog("SendData �ɹ� 0", strlen("SendData �ɹ� 0"), "�ɶ���д.txt");

	return TRUE;
}

//��������
BOOL CTcpSocket::RecvData(VOID *pData, long *plDataLen)
{
	//CWriteDailyLog::WriteLog(m_bSocketReadable ? "�ɶ�" : "����", strlen("��д"), "�ɶ���д.txt");
	//���ɽ���
	if (!m_bSocketReadable) return FALSE;

	//�Ϸ���֤
	if ((m_nSocket == INVALID_SOCKET_ID)) return FALSE;
	if (pData == NULL || plDataLen == NULL) return FALSE;

	//����
	long lRecvByte = 0;

	//��������
	memset(pData, 0, *plDataLen);
	if ((lRecvByte = recv(m_nSocket, (char *)pData, *plDataLen, 0)) <= 0)
	{
		//Close();
		*plDataLen = lRecvByte;
		return FALSE;
	}

	//���
	*plDataLen = lRecvByte;

	//����ͳ��
	m_stDataFlowCount.lRecvTotal += lRecvByte;
	return TRUE;
}


//���õ�ַ
void CTcpSocket::SetDestAddr(void *pAddr)
{
	if (pAddr)
		SetAddrInfo(*((CAddr *)pAddr)); 
}

//���õ�ַ
BOOL CTcpSocket::SetAddrInfo(CAddr &addr)
{
	m_SocketAddr = addr;

	return	TRUE;
}

//���õ�ַ
BOOL CTcpSocket::SetAddrInfo(char *domain, int domainlen, unsigned short port, unsigned short family)
{
	return m_SocketAddr.SetAddrInfoByDomainName(domain, domainlen, port, family);		
}

//���õ�ַ
BOOL CTcpSocket::SetAddrInfo(char *ip, int iplen, unsigned short port, unsigned short family, BOOL bNotDomainName)
{
	m_SocketAddr.SetAddrInfo(ip, port, family);

	return	TRUE;
}

//����
VOID CTcpSocket::Clear()
{
	if (m_nSocket != INVALID_SOCKET_ID) Close();
	m_nSocket = INVALID_SOCKET_ID;
	m_SocketAddr.Clear();
	m_bConnected = FALSE;
	ResetDataFlowCount();
}

//��������ͳ��
void CTcpSocket::ResetDataFlowCount()
{
	memset(&m_stDataFlowCount, 0, S_DATA_FLOW_COUNT_T);
}

//����ģʽ
BOOL CTcpSocket::SetBlockMode()
{
	//�Ϸ���֤
	if (m_nSocket < 0) return FALSE;

	//��������ģʽ
	DWORD ul = 0;
	if (0 != ioctlsocket(m_nSocket, FIONBIO, &ul))
	{
		return FALSE;
	}

	return TRUE;
}

//������ģʽ
BOOL CTcpSocket::SetNoBlockMode()
{
	//�Ϸ���֤
	if (m_nSocket < 0) return FALSE;

	//���÷�����ģʽ
	DWORD ul = 1;
	if (0 != ioctlsocket(m_nSocket, FIONBIO, &ul))
	{
		return FALSE;
	}

	return TRUE;
}

//�׽��ֳ�ʱ���	1s
BOOL CTcpSocket::IsMonitorSocketTimeOut(bool bReadableDetect, bool bWriteableDetect, int *pErrOut, bool bSetOneAtLeast, bool bInfiniteWaiting)
{
	BOOL bTimeOut = FALSE;

	//��������
	fd_set *pFd_ReadOpt = NULL;
	fd_set *pFd_WriteOpt = NULL;
	memset(&m_fs_read, 0, sizeof(m_fs_read));
	memset(&m_fs_write, 0, sizeof(m_fs_write));

	//�ɶ����
	if (bReadableDetect)
	{
		FD_ZERO(&m_fs_read);
		FD_SET(m_nSocket, &m_fs_read);
		pFd_ReadOpt = &m_fs_read;
	}

	//��д���
	if (bWriteableDetect)
	{
		FD_ZERO(&m_fs_write);
		FD_SET(m_nSocket, &m_fs_write);
		pFd_WriteOpt = &m_fs_write;
	}

	//��ʱ��ز�����ʼ��
	timeval timeout;
	memset(&timeout, 0, sizeof(timeout));
	timeout.tv_sec = 1;	//1s���
	timeout.tv_usec = 0;

	int ret = SOCKET_ERROR;
	if (bInfiniteWaiting)
	{
		ret = select(m_nSocket+1, pFd_ReadOpt, pFd_WriteOpt, NULL, NULL);	//0��ʾ��ʱ�����򷵻�SOCKET_ERROR ������Ϊ-1ʱ����������������0�� ������Ϊ����ʱ����ʾ�Ѿ�׼���õ�����������
	}
	else
	{
		ret = select(m_nSocket+1, pFd_ReadOpt, pFd_WriteOpt, NULL, &timeout);	//0��ʾ��ʱ�����򷵻�SOCKET_ERROR ������Ϊ-1ʱ����������������0�� ������Ϊ����ʱ����ʾ�Ѿ�׼���õ�����������
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
			// �ж�socket����Ƿ�ɶ�  
			if (!FD_ISSET(m_nSocket, &m_fs_read))
			{
				if (bReadableDetect) FD_CLR(m_nSocket, pFd_ReadOpt);
				bTimeOut |= TRUE;
			}
		}
		else if (bWriteableDetect&&!bReadableDetect)
		{
			// �ж�socket����Ƿ��д  
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
			// �ж�socket����Ƿ�ɶ�  
			if (!FD_ISSET(m_nSocket, &m_fs_read))
			{
				if (bReadableDetect) FD_CLR(m_nSocket, pFd_ReadOpt);
				bTimeOut |= TRUE;
			}

			// �ж�socket����Ƿ��д  
			if (!FD_ISSET(m_nSocket, &m_fs_write))
			{
				if (bWriteableDetect) FD_CLR(m_nSocket, pFd_WriteOpt);
				bTimeOut |= TRUE;
			}
		}
	}

	return bTimeOut;
}

//�׽��ֳ�ʱ���
BOOL CTcpSocket::DelayMonitorSocket(long tv_sec, long tv_usec, bool bReadableDetect, bool bWriteableDetect, int *pErrOut, bool bSetOneAtLeast, bool bInfiniteWaiting )
{
	BOOL bTimeOut = FALSE;

	//��������
	fd_set *pFd_ReadOpt = NULL;
	fd_set *pFd_WriteOpt = NULL;
	memset(&m_fs_read, 0, sizeof(m_fs_read));
	memset(&m_fs_write, 0, sizeof(m_fs_write));

	//�ɶ����
	if (bReadableDetect)
	{
		FD_ZERO(&m_fs_read);
		FD_SET(m_nSocket, &m_fs_read);
		pFd_ReadOpt = &m_fs_read;
	}

	//��д���
	if (bWriteableDetect)
	{
		FD_ZERO(&m_fs_write);
		FD_SET(m_nSocket, &m_fs_write);
		pFd_WriteOpt = &m_fs_write;
	}

	//��ʱ��ز�����ʼ��
	timeval timeout;
	memset(&timeout, 0, sizeof(timeout));
	timeout.tv_sec = tv_sec;	//���
	timeout.tv_usec = tv_usec;

	int ret = SOCKET_ERROR;
	if (bInfiniteWaiting)
	{
		ret = select(m_nSocket+1, pFd_ReadOpt, pFd_WriteOpt, NULL, NULL);	//0��ʾ��ʱ�����򷵻�SOCKET_ERROR ������Ϊ-1ʱ����������������0�� ������Ϊ����ʱ����ʾ�Ѿ�׼���õ�����������
	}
	else
	{
		ret = select(m_nSocket+1, pFd_ReadOpt, pFd_WriteOpt, NULL, &timeout);	//0��ʾ��ʱ�����򷵻�SOCKET_ERROR ������Ϊ-1ʱ����������������0�� ������Ϊ����ʱ����ʾ�Ѿ�׼���õ�����������
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
			// �ж�socket����Ƿ�ɶ�  
			if (!FD_ISSET(m_nSocket, &m_fs_read))
			{
				if (bReadableDetect) FD_CLR(m_nSocket, pFd_ReadOpt);
				bTimeOut = TRUE;
			}
		}
		else if (bWriteableDetect&&!bReadableDetect)
		{
			// �ж�socket����Ƿ��д  
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
			// �ж�socket����Ƿ�ɶ�  
			if (!FD_ISSET(m_nSocket, &m_fs_read))
			{
				if (bReadableDetect) FD_CLR(m_nSocket, pFd_ReadOpt);
				bTimeOut |= TRUE;
			}

			// �ж�socket����Ƿ��д  
			if (!FD_ISSET(m_nSocket, &m_fs_write))
			{
				if (bWriteableDetect) FD_CLR(m_nSocket, pFd_WriteOpt);
				bTimeOut |= TRUE;
			}
		}
	}


	return bTimeOut;
}

//���socket������
BOOL CTcpSocket::ClearSocketCache()
{
	//��������
	char clr[8] = { 0 };
	long lClrLen = 1;
	int iErr = 0;

	while (true)
	{
		memset(clr, 0, sizeof(clr));
		if (IsMonitorSocketTimeOut(true, false, &iErr))
		{
			//�쳣
			if (iErr < 0) return FALSE;

			break;
		}

		RecvData(clr, &lClrLen);
	}

	return TRUE;
}

//�����׽���
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

//��ȡ�Է�ip�˿�
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

//��ȡ���ص�ַ�˿�
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
//��ʼ��socket����
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
	//��ն���
	ClearAllDataCache();
}

//�����������
BOOL CNetworkService::StartNetService()
{
	//��ն���
	ClearAllDataCache();

	//����socket
	return CreateSocket();
}



//�Ͽ�����
BOOL CNetworkService::CloseNetService()
{
	Close();

	//��ն���
	ClearAllDataCache();

	return TRUE;
}

//ҵ���߼����������
bool CNetworkService::AddDataToSend(void *pData, long lDataLen)
{
	//����У��
	if (!pData) return FALSE;

	//��������
	CACHE_T *pCache = new CACHE_T;
	if (!pCache) return FALSE;
	memset(pCache, 0, S_CACHE_T);

	//�Ƿ����ð�ȫ����
	if (m_bSetTransferSecurity)
	{
		//����
		//��Կkey
		BYTE pcbKey[DES_KEY_BIT + 1] = { 0 };
		BYTE cbKeyCipher[RSA_KEY_STR_LEN / 8] = { 0 };	//key����
		BYTE *pKeyCipherTmp = cbKeyCipher;				//key����
		unsigned long KeyCipherLen = RSA_KEY_STR_LEN / 8;
		DWORD dwKeyLen = DES_KEY_BIT + 1;
		DWORD dwDataLenTmp = lDataLen;		//���ĳ���
		CDESEngine DesSystem;
		CRsaEngine RsaSystem;
		//����rsa��Կ
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
		DesSystem.Encrypte(pcbKey, &dwKeyLen, (BYTE *)pData, dwDataLenTmp, (BYTE *)pCache->pData, &dwNeeded);	//ҵ�������
		unsigned char *pDataTmp = (unsigned char *)pCache->pData;
		pDataTmp += dwNeeded;									//ƫ��
		memmove(pDataTmp, pKeyCipherTmp, KeyCipherLen);			//���ܺ����Կ
		pCache->lDataLen = dwNeeded + KeyCipherLen;				//ʵ�ʼ��ܺ�ҵ��㳤��
	}
	else
	{
		//�޼���
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
	
	
	//��־
	//char rizhi[8192] = { 0 };
//	sprintf(rizhi, "ҵ������ģ�pCache->pData:%s, ���ȣ�%d;����ҵ�����ݳ��ȣ�%d\r\n���ܺ����Կ��%s, ���ȣ�%d\r\n����ǰ����Կ��%s, ���ȣ�%d;\r\n", pDataTmp, pCache->lDataLen, dwNeeded, pKeyCipherTmp, KeyCipherLen, pcbKey, iKeyLentmpd);
	//CWriteDailyLog::WriteLog(rizhi, "yewuceng.txt");

	//��������
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

	//����
	//if (!DataEncrypt(pData, &lDataLen, &pCache->pData, &pCache->lDataLen))
	//{
	//	if (pCache) delete pCache;
	//	return FALSE;
	//}

	//�������
	//m_BusinessLayerCache = pCache;
	m_CacheDataQueue.push_back(pCache);
	
	//��������������
	m_lCacheDataSize += pCache->lDataLen;

	return TRUE;
}

//������
BOOL CNetworkService::CreateSocketPacketToSend(long &lPackCount)
{
	//�����ж�
	if (m_CacheDataQueue.empty()) return FALSE;

	//��������
	long lPackCountTmp = 0;

	//��մ����Ͱ�����������
	ClearSocketPackCacheToSend();

	//���ռ�
	unsigned char *ucpCacheTmp = new unsigned char[m_lCacheDataSize];
	if (!ucpCacheTmp) return FALSE;
	memset(ucpCacheTmp, 0, m_lCacheDataSize);

	//�ӻ�����ת������
	unsigned char *pBusinessDataMoveTmp = ucpCacheTmp;
	while (!m_CacheDataQueue.empty())
	{
		memmove(pBusinessDataMoveTmp, m_CacheDataQueue.front()->pData, m_CacheDataQueue.front()->lDataLen);
		pBusinessDataMoveTmp += m_CacheDataQueue.front()->lDataLen;

		//�ͷſռ�
		delete m_CacheDataQueue.front()->pData;
		delete m_CacheDataQueue.front();
		//m_CacheDataQueue.front() = NULL;
		m_CacheDataQueue.pop_front();
	}

	//����ռ�
	m_CacheDataQueue.clear();
	
	//ҵ�������
	pBusinessDataMoveTmp = ucpCacheTmp;
	
	//�ж�ҵ��㳤��ȷ��������
	if (m_lCacheDataSize > BUSINESS_DATA_LIMIT)
	{
		lPackCountTmp = m_lCacheDataSize / BUSINESS_DATA_LIMIT + ((m_lCacheDataSize%BUSINESS_DATA_LIMIT > 0) ? 1 : 0);
	}
	else
	{
		lPackCountTmp = 1;
	}

	//��������
	long lPackSeq = 0;	//�����
	long lCount = lPackCountTmp;
	long lCacheDataSizeTmp = m_lCacheDataSize;	//���ݴ�С
	long lSingleDataSize = 0;
	long lPacketTotalSize = 0;
	char *pPacketContainer = NULL;	//����
	char *pPacketContainerTmp = pPacketContainer;
	CACHE_T *pDataCacheToSend = NULL;

	//fortest
	long lCountLen = 0;

	//ѭ����
	while (lCount-- >0)
	{
		//ҵ��㳤��
		lSingleDataSize = ((lCacheDataSizeTmp > BUSINESS_DATA_LIMIT) ? BUSINESS_DATA_LIMIT : lCacheDataSizeTmp);
		lCacheDataSizeTmp -= lSingleDataSize;

		//���ܳ�
		lPacketTotalSize = S_SOCKET_PACK_HEAD_T + S_ACCESS_LAYER_T + lSingleDataSize + S_SOCKET_PACK_TAIL;

		//fortest
		lCountLen += lPacketTotalSize;

		//���ռ���
		pPacketContainer = new char[lPacketTotalSize];
		if (!pPacketContainer) return FALSE;
		memset(pPacketContainer, 0, lPacketTotalSize);
		pPacketContainerTmp = pPacketContainer;

		//��ͷ
		memset(&m_stPackeHead, 0, S_SOCKET_PACK_HEAD_T);
		m_stPackeHead.usHead = SOCKET_PACK_HEAD_T::HEAD_ID;
		memmove(pPacketContainerTmp, &m_stPackeHead, S_SOCKET_PACK_HEAD_T);
		pPacketContainerTmp += S_SOCKET_PACK_HEAD_T;

		//�����
		memset(&m_AccessLayer, 0, S_ACCESS_LAYER_T);
		m_AccessLayer.ulPacketNum = lPackCountTmp;
		m_AccessLayer.ulPacketSeq = lPackSeq;
		m_AccessLayer.ulPacketSize = lPacketTotalSize;
		//m_AccessLayer.uTransID = ? ;
		memmove(pPacketContainerTmp, &m_AccessLayer, S_ACCESS_LAYER_T);
		pPacketContainerTmp += S_ACCESS_LAYER_T;		

		//ҵ���
		memmove(pPacketContainerTmp, pBusinessDataMoveTmp, lSingleDataSize);
		pBusinessDataMoveTmp += lSingleDataSize;
		pPacketContainerTmp += lSingleDataSize;

		//��β
		memset(&m_stPackeTail, 0, S_SOCKET_PACK_TAIL);
		m_stPackeTail.usTail = m_stPackeTail.TAIL_ID;
		memmove(pPacketContainerTmp, &m_stPackeTail, S_SOCKET_PACK_TAIL);
		pPacketContainerTmp += S_SOCKET_PACK_TAIL;

		//���ռ�
		pDataCacheToSend = new CACHE_T;
		if (!pDataCacheToSend) return FALSE;
		memset(pDataCacheToSend, 0, S_CACHE_T);
		pDataCacheToSend->pData = pPacketContainer;
		pDataCacheToSend->lDataLen = lPacketTotalSize;

		//һ�������ѹ�������������
		m_SocketPackCacheToSend.insert(map<long, CACHE_T *>::value_type(lPackSeq, pDataCacheToSend));

		//�������
		lPackSeq++;
	}

	//�ͷ�ҵ���ռ�
	delete[]ucpCacheTmp;

	//ҵ������ݻ���������
	ClearBusinessLayerToSendCache();

	//������
	lPackCount = m_SocketPackCacheToSend.size();

	return TRUE;	
}

//�������ݰ�
bool CNetworkService::SendDataToDest(BOOL bSocketBlockMode, BOOL bNeedSocketBlockSet, bool bNeedReturnPacket, long lTvPerSec, long lTvPerUsec, int iTimeOutCountSet)
{
	//����У��
	//�Ƿ��Ѿ�����socket
	if (m_nSocket < 0) return FALSE;

	//�Ƿ��Ѿ�����
	//�ж��Ƿ�������
	//if (!GetNetConnectStatus()) return FALSE;

	//if (m_CacheDataQueue.empty()) return FALSE;
	long lPackCount=0;

	//��־
	//CWriteDailyLog::WriteLog("SendDataToDest 0", strlen("SendDataToDest 0"), "��־.txt");

	//��������װ��(����ҵ�������)
	if (!CreateSocketPacketToSend(lPackCount)) return FALSE;

	//��־
	//CWriteDailyLog::WriteLog("SendDataToDest 1", strlen("SendDataToDest 1"), "��־.txt");

	//��������Ҫ����
	if (m_SocketPackCacheToSend.empty()) return FALSE;
	
	//��־
	//CWriteDailyLog::WriteLog("SendDataToDest 2", strlen("SendDataToDest 1"), "��־.txt");


	//��������ģʽ
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

	//��־
//	CWriteDailyLog::WriteLog("SendDataToDest 3", strlen("SendDataToDest 1"), "��־.txt");


	//��������
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
			//����
			for (long i = 0; i < m_SocketPackCacheToSend.size(); i++)
			{
				//ȡ��
				CACHE_T *pData = m_SocketPackCacheToSend[i];

				//TCHAR szRizhi[100] = { 0 };
				//TCHAR szResult[200] = { 0 };
				//CWriteDailyLog::AnsiToUnicode((char *)(((char *)pData->pData)+10), szRizhi, 100 * sizeof(TCHAR));
				//_sntprintf(szResult, 200, TEXT("%s, len %d "), szRizhi, pData->lDataLen);
				//MessageBox(NULL, szResult, NULL, 0);

				//��ʱ��������
				iTimeOutCount = iTimeOutCountSet;

				//����һ����			
				while ((iTimeOutCountSet-->0))
				{
					bRecvReturn = FALSE;

					//����
					if (SendData(pData->pData, &pData->lDataLen) == FALSE) return FALSE;

					//�Ƿ���Ҫ�ɹ����հ�Ӧ��
					if (!bNeedReturnPacket) break;
					
					//��ʱ�ȴ�	500ms
					//if (IsMonitorSocketTimeOut()) continue;
					if (DelayMonitorSocket(0, 500000)) continue;

					//����һ��������ȷ���ذ�
					//memset(RecvCache, 0, lRecvCacheSize);
					//if (bRecvReturn=RecvData(RecvCache, &lRecvCacheSize))
					//{
						//ֱ����յ�����������һ������Ҳ���Խ����������жϹ��ܺ�RIGHT_RECV��
					//	if (!IsDestRecvSuccess(RecvCache, &lRecvCacheSize)) continue;
						//memset(RecvCache, 0, iRecvCacheSize);
						//�������ж�	��������ط�	���жϹ��ܺ� right_Recv �����������ܺ�
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

				//����ʧ���ж�
				if (iTimeOutCount <= 0 && bRecvReturn == FALSE&&bNeedReturnPacket)
				{
					return FALSE;
				}
			}
		}

		break;
	}

	//TCHAR ridd[100] = { 0 };
	//_sntprintf(ridd, 100, TEXT("���ͳɹ�����iCountSuccess:%d, ���ȣ�%d"), iCountSuccess, iCountLen);
	//if (bNeedReturnPacket)
	//	AfxMessageBox(ridd);

	//������� ��մ������������
	ClearSocketPackCacheToSend();

	return TRUE;

	//(�Ӵ���������ȡһ��������send������ˣ��ȴ�����˷��ؽ������Ӧ���ٷ���һ��������1s��δ�ظ��ط������ѭ��ֱ���Ѱ����꣬�����ն���)
	//��������
}

//�Է��Ƿ���ճɹ�ȷ��
BOOL CNetworkService::IsDestRecvSuccess(/*��ȷ�ϵİ����*/long lCurrentPackSeq, BOOL bSocketBlockMode, BOOL bNeedSocketBlockSet)
{
	//�ɹ�����Ӧ������ܺ� FUNID_RIGHT_RECV
	RECV_CONFIRM_PACK_T *pSuccessRecvPack = NULL;
	long lRecvPackSize = 0;

	//��־
//	CWriteDailyLog::WriteLog("IsDestRecvSuccess 1", strlen("IsDestRecvSuccess 1"), "��־.txt");

	if (!RecvDataFromSrc(bSocketBlockMode, bNeedSocketBlockSet, FALSE)) return FALSE;

	//��־
//	CWriteDailyLog::WriteLog("IsDestRecvSuccess 2", strlen("IsDestRecvSuccess 1"), "��־.txt");

	if (!GetOutRecvData((void **)&pSuccessRecvPack, &lRecvPackSize))
	{
		ReleaseRecvDataOut();
		pSuccessRecvPack = NULL;
		return FALSE;
	}

	//��־
	//CWriteDailyLog::WriteLog("IsDestRecvSuccess 3", strlen("IsDestRecvSuccess 1"), "��־.txt");

	if (!pSuccessRecvPack) return FALSE;

	//��־
	//char rizhi[100] = { 0 };
	//sprintf(rizhi, "lPackSeq:%ld; RecvSeq:%ld;", lCurrentPackSeq, pSuccessRecvPack->ulPacketSeq);
	//CWriteDailyLog::WriteLog(rizhi, strlen(rizhi), "��־.txt");

	//�ж�
	if (pSuccessRecvPack->uFunID == RECV_CONFIRM_PACK_T::SUCCESS_RECV
		&&pSuccessRecvPack->ulPacketSeq == lCurrentPackSeq)
	{
		ReleaseRecvDataOut();
		return TRUE;
	}

	ReleaseRecvDataOut();

	return FALSE;
}

//�������ݰ�
//1.��һ�����ݰ�����������������ݰ���
//2.��λ��ͷ��β��һ������ͷ�����ֽ�Ϊ��ͷ��һ�����ĺ������ֽ�Ϊ��β��
//3.��λ�����ݳ��ȣ������յ��ֽ���Ӧ����֮�����(�ϰ�����Ȳ�����գ����г�ʱ���յȴ�,���޶�ʱ����δ���յ���������Ͽ����ӽ���ʧ�ܲ���������)
//4.��λ����ź��ܰ��������ڼ������մ�����
//5.ά��һ��ȥ�ذ����У��ѽ��յİ�����Ҫ�ٽ��գ�����ʾ���ճɹ�

//ѹ������ջ����������ؽ��ճɹ�Ӧ���

//�˳�while
//�����հ������ԣ����ĸ�����
//���в����ƴ��ҵ��㣬ƴ����ɺ����ҵ��㣬����ȴ�����ҵ��㻺����
//��ɽ���
BOOL CNetworkService::RecvDataFromSrc(BOOL bSocketBlockMode, BOOL bNeedSocketBlockSet, BOOL bNeedSendSuccessRecvReturnPack, long lTvPerSec, long lTvPerUsec, int iTimeOutCountSet)
{
	//MessageBox(NULL, TEXT("RecvDataFromSrc 01"), NULL, 0);
	//����У��
	//�Ƿ��Ѿ�����socket
	if (m_nSocket < 0) return FALSE;
	//�Ƿ��Ѿ�����
	//�ж��Ƿ�������
	//if (!GetNetConnectStatus()) return FALSE;
//	if (!m_SocketPackCacheToRecv.empty()) return FALSE;

	//��մ��������ݻ����� �ڳ��ռ� socket������
	//ClearBusinessLayerToRecvCache();
	ClearSocketPackCacheToRecv();

	//��������ģʽ
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

	
	//���հ�
	char recvdata[SOCKET_PACK_SIZE_MAX_LIMIT] = { 0 };
	long lRecvDataLen = 0;
	BOOL bRecvSuccess = FALSE;
	int iCountTimeOut = iTimeOutCountSet;
//	int iCountSuccessRecv = 0;		//�ɹ���������
	unsigned short usHead = 0;
	unsigned short usTail = 0;
	unsigned long ulPacketSize = 0;
	unsigned long ulPacketNum = 0;
	unsigned long ulPacketSeq = 0;
	unsigned long ulTransID = 0;
	ACCESS_LAYER_T *pAccessLayer = NULL;
	//�ɹ�����Ӧ������ܺ�
	RECV_CONFIRM_PACK_T stRecvConfirm;
	memset(&stRecvConfirm, 0, S_RECV_CONFIRM_PACK_T);
	stRecvConfirm.uFunID = stRecvConfirm.SUCCESS_RECV;
	//unsigned int uSuccessRecvFunID = FUNID_RIGHT_RECV;
	CACHE_T *pDataCacheSave = NULL;
	m_lRecvCacheDataSize = 0;		//��

	//fortest
	long lCountSuccess = 0;
	long lCountLen = 0;

	while (iCountTimeOut-->0)
	{
		//����
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

		//��ʱ����
		//if (IsMonitorSocketTimeOut()) continue;
		if (DelayMonitorSocket(lTvPerSec, lTvPerUsec)) continue;
		bRecvSuccess = RecvData(recvdata, &lRecvDataLen);

		if (!bRecvSuccess) continue;



		//��ͷ��βУ��
		usHead = ((SOCKET_PACK_HEAD_T *)recvdata)->usHead;
		usTail = ((SOCKET_PACK_TAIL *)(recvdata + (lRecvDataLen - S_SOCKET_PACK_TAIL)))->usTail;
		if (usHead != SOCKET_PACK_HEAD_T::HEAD_ID || usTail != SOCKET_PACK_TAIL::TAIL_ID) continue;

		//�����
		pAccessLayer = ((ACCESS_LAYER_T *)(recvdata + S_SOCKET_PACK_HEAD_T));

		//У����ܳ�
		//usPacketSize = *((unsigned short *)(recvdata + sizeof(unsigned short)));
		ulPacketSize = pAccessLayer->ulPacketSize;
		//�����سɹ�Ӧ����ͻ��˻ᳬʱ�ط�
		if (ulPacketSize != lRecvDataLen) continue;

		//������
		ulPacketNum = pAccessLayer->ulPacketNum;

		//�����
		ulPacketSeq = pAccessLayer->ulPacketSeq;

		//����ˮ��
		ulTransID = pAccessLayer->ulTransID;

		//����Ƿ��ذ�
		if (m_SocketPackCacheToRecv.find(ulPacketSeq) != m_SocketPackCacheToRecv.end())
		{
			//�����ذ� ����Ӧ��� �ͻ��˷�����һ���� ���ɹ��İ�
			//���ͳɹ�����Ӧ���
			if (bNeedSendSuccessRecvReturnPack)
			{
				stRecvConfirm.ulPacketSeq = ulPacketSeq;
				AddDataToSend(&stRecvConfirm, sizeof(stRecvConfirm));
				SendDataToDest(FALSE, !bNeedSocketBlockSet, false);
			}

			//��λ��ʱ����
			iCountTimeOut = iTimeOutCountSet;
			continue;
		}

		//���ռ䱣��ҵ���߼�������
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
		
		//ѹ�������socket����
		if (m_SocketPackCacheToRecv.find(ulPacketSeq) == m_SocketPackCacheToRecv.end())
		{
			//�ۼӰ��������ڿ��ռ���װҵ�������
			m_lRecvCacheDataSize += pDataCacheSave->lDataLen;

			m_SocketPackCacheToRecv.insert(map<long, CACHE_T*>::value_type(ulPacketSeq, pDataCacheSave));

			//���ͳɹ�����Ӧ���
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
				//_sntprintf(szResult, 200, TEXT("%s; %s, ���ݣ�%s, len %d "), bRet ? TEXT("��ִ���ͳɹ�") : TEXT("��ִ����ʧ��"), bRecvSuccess ? TEXT("�ɹ�recv") : TEXT("ʧ��recv"), szRizhi, m_lRecvCacheDataSize);
			//MessageBox(NULL, szResult, NULL, 0);

			//fortest
			lCountSuccess++;
			lCountLen += ulPacketSize;

			//���ճɹ�����
			iCountTimeOut = iTimeOutCountSet;

			//����Ƿ�������
			if (m_SocketPackCacheToRecv.size() == ulPacketNum) break;
		}
	}

	if (!bRecvSuccess)
	{
		return FALSE;
	}

	//ƴ�ӽ��յ���ҵ�������
	CreateBussnessLayerToGet();

	//����socket�ȴ����ջ�����
//	ClearSocketPackCacheToRecv();

	return TRUE;
}

//���(������)��socket�����ջ���������ƴ�ӳ�ҵ������� �õ�һ��ҵ���߼�������
BOOL CNetworkService::CreateBussnessLayerToGet()
{
	//����У��
	if (m_SocketPackCacheToRecv.empty() || m_lRecvCacheDataSize==0) return FALSE;

	//���ռ�
	char *pBusinessLayerData = new char[m_lRecvCacheDataSize];
	if (!pBusinessLayerData) return FALSE;
	memset(pBusinessLayerData, 0, m_lRecvCacheDataSize);
	char *pDataTmp = pBusinessLayerData;

	//��˳��ȡ���� size����0��ʼ��size
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

	//У������������
	if (lBusinessTotalSize != m_lRecvCacheDataSize)
	{
		delete[]pBusinessLayerData;
		return FALSE;
	}

	//��������
	CACHE_T *pBusinessDataCache = new CACHE_T;
	if (!pBusinessDataCache)
	{
		delete[]pBusinessLayerData;
		return FALSE;
	}
	memset(pBusinessDataCache, 0, sizeof(CACHE_T));
	
	if (m_bSetTransferSecurity)
	{
		//����
		CRsaEngine RsaSystem;
		//����˽Կ
		RsaSystem.SetPrivateKey((unsigned char *)m_strRsaPrivateKey.c_str());
		CDESEngine DesSystem;
		//Ѱ����Կ ���� CIPHER_TEXT_OUT_ENCODE_LEN ����
		BYTE *pcbKeyCipher = (BYTE *)(((char *)pBusinessLayerData) + m_lRecvCacheDataSize - CIPHER_TEXT_OUT_ENCODE_LEN);
		BYTE cbKeyPlain[DES_KEY_BIT + 1] = { 0 };
		BYTE *pcbKeyPlainTmp = cbKeyPlain;
		unsigned long ulKeyPlainSize = DES_KEY_BIT + 1;
		//������Կ
		RsaSystem.Decrypte(pcbKeyCipher, CIPHER_TEXT_OUT_ENCODE_LEN, &pcbKeyPlainTmp, &ulKeyPlainSize);

		//���ռ�Ž��ܺ��������
		unsigned char *pPlainDataTmp = new unsigned char[m_lRecvCacheDataSize - CIPHER_TEXT_OUT_ENCODE_LEN];
		if (!pPlainDataTmp)
		{
			delete[]pBusinessLayerData;
			return FALSE;
		}
		memset(pPlainDataTmp, 0, m_lRecvCacheDataSize - CIPHER_TEXT_OUT_ENCODE_LEN);
		DWORD dwPlainDataLen = m_lRecvCacheDataSize - CIPHER_TEXT_OUT_ENCODE_LEN;

		//��������
		DesSystem.Decrypte(cbKeyPlain, ulKeyPlainSize, (BYTE *)pBusinessLayerData, m_lRecvCacheDataSize - CIPHER_TEXT_OUT_ENCODE_LEN, pPlainDataTmp, &dwPlainDataLen);

		//���ռ��Ž��ܺ������
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
		//�޼���
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

	//����
	//DataDecrypt(pBusinessLayerData, &m_lRecvCacheDataSize, &pBusinessDataCache->pData, &pBusinessDataCache->lDataLen);
	m_lRecvCacheDataSize = pBusinessDataCache->lDataLen;

	//��������ڳ��ռ�
	ClearBusinessLayerToRecvCache();

	//����������ҵ��㻺����
	if (pBusinessDataCache->pData)
	{
		m_RecvCacheDataQueue.push_back(pBusinessDataCache);
	}

	//����ռ�
	if (pBusinessLayerData) delete[]pBusinessLayerData;

	//������������ݻ�����socket
	ClearSocketPackCacheToRecv();

	return TRUE;
}

//ȡ�����յ�������
BOOL CNetworkService::GetOutRecvData(void **pData, long *lDataLen)
{
	//����У��
	if (pData == NULL || lDataLen == NULL) return FALSE;
	if (m_RecvCacheDataQueue.empty()) return FALSE;

	//ȡ����
	//���ռ�
	//����Ǩ��
	//����ռ�
	//���
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
			
			//���������ͷ�
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

//�ͷ�ȡ����������
BOOL CNetworkService::ReleaseRecvDataOut()
{
	if (m_pRecvDataOut)
	{
		delete[]m_pRecvDataOut;
		m_pRecvDataOut = NULL;
	}

	return TRUE;
}

//���ù�Կ
void CNetworkService::SetRsaPublicKey(unsigned char *ucKey)
{
	if (!ucKey) return;
	m_bSetTransferSecurity = true;
	m_strRsaPublicKey.clear();
	m_strRsaPublicKey = (char *)ucKey;
}

//����˽Կ
void CNetworkService::SetRsaPrivateKey(unsigned char *ucKey)
{
	if (!ucKey) return;
	m_bSetTransferSecurity = true;
	m_strRsaPrivateKey.clear();
	m_strRsaPrivateKey = (char *)ucKey;
}

//��մ�����������  3���е�send
BOOL CNetworkService::ClearSocketPackCacheToSend()
{
	//��������
	map<long, CACHE_T*>::iterator itCacheTmp;
	CACHE_T* pCacheTmpValue = NULL;

	//��մ���������
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


//��մ����հ������� 3���е�recv
BOOL CNetworkService::ClearSocketPackCacheToRecv()
{
	//��������
	map<long, CACHE_T*>::iterator itCacheTmp;
	CACHE_T* pCacheTmpValue = NULL;

	//��մ���������
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

//socketҵ������ݻ��������������� 1��
BOOL CNetworkService::ClearBusinessLayerToSendCache()
{
	m_lCacheDataSize = 0;
	memset(&m_stPackeHead, 0, S_SOCKET_PACK_HEAD_T);
	memset(&m_AccessLayer, 0, S_ACCESS_LAYER_T);
	memset(&m_stPackeTail, 0, S_SOCKET_PACK_TAIL);

	//��������
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

//socketҵ������ݻ��������������� 2��
BOOL CNetworkService::ClearBusinessLayerToRecvCache()
{ 
	m_lRecvCacheDataSize = 0;


	//��������
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

//��ջ��������ݶ��� �������������
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
//tcp�ͻ���

CNetworkTcpClient::CNetworkTcpClient()
{
}

CNetworkTcpClient::~CNetworkTcpClient()
{
}

//���ӷ�����
bool CNetworkTcpClient::Start()
{
	//�����������
	if (!StartNetService()) return FALSE;

	//���ӷ�����
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


//���ӷ�����
BOOL CNetworkTcpClient::Link(char *pServerAddr, unsigned short port, bool bClearDataCache)
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

//�ر�����
BOOL CNetworkTcpClient::CloseLink()
{
	return CloseNetService();
}

//���������������
bool CNetworkTcpClient::SendDataTo(void *pData, long lDataLen)
{
	//����У��
	if (!pData) return FALSE;

	if (!AddDataToSend(pData, lDataLen)) return FALSE;

	bool bSend = SendDataToDest(FALSE, TRUE, true);

	return bSend;
}

//�ӷ�������ȡ����
bool CNetworkTcpClient::RecvDataFrom(void **pData, long *lDataLen)
{
	//����У��
	if ((!pData) || (!lDataLen)) return FALSE;

	if (!RecvDataFromSrc()) return FALSE;

	return GetOutRecvData(pData, lDataLen);
}

//�������ã�����Ͽ����ӣ�
BOOL CNetworkTcpClient::RefreshConfig(char *host/*����*/, int hostlen/*����*/, unsigned short port/*�˿�*/, bool bIsDomainName/*�Ƿ�����*/)
{
	if (!host || hostlen <= 0) return FALSE;

	if (!m_strServerAddr.empty()) m_strServerAddr.clear();

	m_strServerAddr = host;
	m_ServerPort = port;
	m_bIsDomainName = bIsDomainName;

	return TRUE;
}


//����Զ��
BOOL CNetworkTcpClient::Connect()
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

/////////////////////////////////////////////////////////////

//������
CNetworkTcpServer::CNetworkTcpServer()
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
	//SetRsaPublicKey((unsigned char *)szClientPublicKey);
	//SetRsaPrivateKey((unsigned char *)szServerPrivateKey);

	//�첽IO����
	m_NumOfClntSock = 0;

	//m_pMainWind = NULL;
}

CNetworkTcpServer:: ~CNetworkTcpServer()
{
}

//����������
bool CNetworkTcpServer::Start()
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

		//��������
		WaitForEvent();

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "CTcpServer::StartServer ����=");
		strErr.append(rizhi);

		//�쳣5
		iAbort++;

	}
	catch (...)
	{
		//��־
		//memset(rizhi, 0, sizeof(rizhi));
		//sprintf(rizhi, "CTcpServer StartServer:�쳣��%s\r\n", (char *)strErr.c_str());
	}


	//��������
	//WaitForEvent();

	return TRUE;
}

//�رշ�����
BOOL CNetworkTcpServer::CloseServer()
{
	//�����첽io����
	//�Ͽ��׽���
	return CloseNetService();
}

//��������	(�����ȹرշ�����)
BOOL CNetworkTcpServer::RefreshConfig(char *host/*����*/, int hostlen/*����*/, unsigned short port/*�˿�*/, bool bIsDomainName/*�Ƿ�����*/)
{
	if (!host || hostlen <= 0) return FALSE;

	if (!m_strServerAddr.empty()) m_strServerAddr.clear();

	m_strServerAddr = host;
	m_ServerPort = port;
	m_bIsDomainName = bIsDomainName;

	return TRUE;
}


//�󶨵�ַ
BOOL CNetworkTcpServer::Bind(char *ip, int iplen, unsigned short port, unsigned short family)
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

//�����˿�
BOOL CNetworkTcpServer::Listen(int iBacklog)
{
	int iRet = listen(m_nSocket, iBacklog);
	if (iRet == 0)
	{
		return TRUE;
	}

	return FALSE;
}

//ȷ�����Ӳ���ȡ�ͻ�����Ϣ
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

//��ȡ����
bool CNetworkTcpServer::RecvDataFrom(void **pData, long *lDataLen)
{
	if (!RecvDataFromSrc()) return FALSE;

	return GetOutRecvData(pData, lDataLen);
}

//��������
bool CNetworkTcpServer::SendDataTo(void *pData, long lDataLen)
{
	if (!AddDataToSend(pData, lDataLen)) return FALSE;

	return SendDataToDest(FALSE, TRUE, true);
}

//�����첽IOģ��
BOOL CNetworkTcpServer::SetAsynchronousServer()
{
	m_newEvent = WSACreateEvent();

	if (WSAEventSelect(m_nSocket, m_newEvent, FD_ACCEPT | FD_CLOSE | FD_READ) == SOCKET_ERROR)
		return FALSE;

	m_hSockArr[m_NumOfClntSock] = m_nSocket;
	m_hEventArr[m_NumOfClntSock] = m_newEvent;

	m_NumOfClntSock++;
}

//��Ӽ���¼�
BOOL CNetworkTcpServer::AddEvent(SOCKET hClntSock)
{
	m_newEvent = WSACreateEvent();
	WSAEventSelect(hClntSock, m_newEvent, FD_READ | FD_CLOSE);

	m_hEventArr[m_NumOfClntSock] = m_newEvent;
	m_hSockArr[m_NumOfClntSock] = hClntSock;

	m_NumOfClntSock++;
	return TRUE;
}

//����socket����
void CNetworkTcpServer::CompressSockets(SOCKET hSockArr[], int idx, int total)
{
	int i;
	for (i = idx; i < total; i++)
	{
		hSockArr[i] = hSockArr[i + 1];
	}
}

//�����¼�����
void CNetworkTcpServer::CompressEvents(WSAEVENT hEventArr[], int idx, int total)
{
	int i;
	for (i = idx; i < total; i++)
	{
		hEventArr[i] = hEventArr[i + 1];
	}
}

//����IO�¼� ������ �пɶ��¼����Ƴ�����ͻ��˱��ֶ����� ��Ҫһ���߳�ȥִ�пɶ��¼� �������׽���������Ҫ���ٶ���߳�ִ�м������� ������չ��IOCP
BOOL CNetworkTcpServer::WaitForEvent()
{
	//��������
	int posInfo, startIdx, i, clntAdrLen;
	SOCKET hClntSock;

	char LogMonitor[256] = { 0 };

	string strErr;
	char rizhi[1024] = { 0 };


	try
	{
		//���� ����ͨ���ı�bMonitor��������رռ���
		bool bMonitor = true;
		while (bMonitor)
		{
			strErr.clear();

			memset(rizhi, 0, sizeof(rizhi));
			sprintf(rizhi, "WaitForEvent ��ʼ=WSAWaitForMultipleEvents(m_NumOfClntSock=%d, )=\r\n", m_NumOfClntSock);
			strErr.append(rizhi);

			posInfo = WSAWaitForMultipleEvents(m_NumOfClntSock, m_hEventArr, FALSE, WSA_INFINITE, FALSE);
			startIdx = posInfo - WSA_WAIT_EVENT_0;

			memset(rizhi, 0, sizeof(rizhi));
			sprintf(rizhi, "startIdx=%d =", startIdx);
			strErr.append(rizhi);

			//�жϷ����Ƿ��ж�
			if (bMonitor == false) break;

			memset(rizhi, 0, sizeof(rizhi));
			sprintf(rizhi, "GetMonitor:%s =", bMonitor ? "����" : "�رռ���");
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

						Accept(&hClntSock);

						//����¼�
						if (m_NumOfClntSock <= WSA_MAXIMUM_WAIT_EVENTS)
						{
							//����¼�����
							AddEvent(hClntSock);
						}
						else
						{
							memset(LogMonitor, 0, sizeof(LogMonitor));
							sprintf(LogMonitor, "��ǰ��������%d\r\n", m_NumOfClntSock);

							//�����߳���Ӽ��� ������������
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

						//���̳߳���������ȡ�ͻ��˴���������

						//������	�Ƴ�����
						WSACloseEvent(m_hEventArr[sigEventIdx]);

						m_NumOfClntSock--;
						CompressSockets(m_hSockArr, sigEventIdx, m_NumOfClntSock);
						CompressEvents(m_hEventArr, sigEventIdx, m_NumOfClntSock);
					}

					//�ر��¼�
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
					}//end of �ر��¼�

				}//end of if 
			}//end of for

			memset(rizhi, 0, sizeof(rizhi));
			sprintf(rizhi, "����һ��while =\r\n");
			strErr.append(rizhi);

		}//end of while	
	}
	catch (...)
	{
		//�쳣��־
		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, " CTcpServer::WaitForEvent �쳣���棺%s\r\n", strErr.c_str());
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////
//http����
///////////////////////////////////////////////////////////////////////////
//��ʼ��socket����
CHttpService::CHttpService(LPTSTR lpszRecvLockerName, bool bLockAtOnce, bool bInitiallyOwn, bool bManualReset)
{
	//m_BusinessLayerCache = NULL;
	m_pRecvDataOut = NULL;
	m_ServerPort = 0;
	m_bIsDomainName = true;
	ClearAllDataCache();
	m_HttpBackStatusCode = -1;		//�ɹ��Ĵ���

	//��ʼ����
	//m_pRecvCacheLocker = new CSynLocker(lpszRecvLockerName, bLockAtOnce, bInitiallyOwn, bManualReset);
}

CHttpService::~CHttpService()
{
	//��ն���
	ClearAllDataCache();

	//if (m_pRecvCacheLocker)
	//{
	//	m_pRecvCacheLocker->Release();
	//}
}

//�����������
BOOL CHttpService::StartService()
{
	//��ն���
	ClearAllDataCache();

	//����socket
	return CreateSocket();
}


//���ӷ�����
BOOL CHttpService::Link(char *pServerAddr, unsigned short port, bool bClearDataCache)
{
	//����У��
	if (pServerAddr == NULL) return FALSE;

	//��ջ�����
	if (bClearDataCache) ClearAllDataCache();

	//���õ�ַ
	if (!SetAddrInfo(pServerAddr, strlen(pServerAddr), port, AF_INET))
	{
		return FALSE;
	}

	return Connect();
}

//�Ͽ�����
BOOL CHttpService::CloseService()
{
	Close();

	//��ն���
	ClearAllDataCache();

	return TRUE;
}


//����Զ��
BOOL CHttpService::Connect()
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


//ҵ���߼����������
BOOL CHttpService::AddDataToSend(void *pData, long lDataLen)
{
	//����У��
	if (!pData) return FALSE;

	//��������
	CACHE_T *pCache = new CACHE_T;
	if (!pCache) return FALSE;
	memset(pCache, 0, S_CACHE_T);

	//��������
	pCache->pData = new char[lDataLen];
	if (!pCache->pData)
	{
		delete pCache;
		return FALSE;
	}
	memset(pCache->pData, 0, lDataLen);
	memmove(pCache->pData, pData, lDataLen);
	pCache->lDataLen = lDataLen;

	//����
	//if (!DataEncrypt(pData, &lDataLen, &pCache->pData, &pCache->lDataLen))
	//{
	//	if (pCache) delete pCache;
	//	return FALSE;
	//}

	//�������
	//m_BusinessLayerCache = pCache;
	m_CacheDataQueue.push_back(pCache);

	//��������������
	m_lCacheDataSize += pCache->lDataLen;

	return TRUE;
}

//������
BOOL CHttpService::CreateSocketPacketToSend(long &lPackCount)
{
	//�����ж�
	if (m_CacheDataQueue.empty()) return FALSE;

	//��������
//	long lPackCountTmp = 0;

	//��մ����Ͱ�����������
	ClearSocketPackCacheToSend();

	//���ռ�
	unsigned char *ucpCacheTmp = new unsigned char[m_lCacheDataSize];
	if (!ucpCacheTmp) return FALSE;
	memset(ucpCacheTmp, 0, m_lCacheDataSize);

	//�ӻ�����ת������
	unsigned char *pBusinessDataMoveTmp = ucpCacheTmp;
	while (!m_CacheDataQueue.empty())
	{
		memmove(pBusinessDataMoveTmp, m_CacheDataQueue.front()->pData, m_CacheDataQueue.front()->lDataLen);
		pBusinessDataMoveTmp += m_CacheDataQueue.front()->lDataLen;



		//�ͷſռ�
		delete m_CacheDataQueue.front()->pData;
		delete m_CacheDataQueue.front();
		//m_CacheDataQueue.front() = NULL;
		m_CacheDataQueue.pop_front();
	}

	//����ռ�
	m_CacheDataQueue.clear();
	
	//ҵ�������
	pBusinessDataMoveTmp = ucpCacheTmp;

	//��������
	long lCacheDataSizeTmp = m_lCacheDataSize;	//���ݴ�С
	long lSingleDataSize = 0;
	long lPacketTotalSize = 0;
	char *pPacketContainer = NULL;	//����
	char *pPacketContainerTmp = pPacketContainer;
	CACHE_T *pDataCacheToSend = NULL;

	//fortest
	long lCountLen = 0;

	{
		//ҵ��㳤��
		lSingleDataSize = ((lCacheDataSizeTmp > HTTP_MAX_DATA_LIMIT) ? HTTP_MAX_DATA_LIMIT : lCacheDataSizeTmp);
		lCacheDataSizeTmp -= lSingleDataSize;

		//���ܳ�
		lPacketTotalSize =  lSingleDataSize;

		//fortest
		lCountLen += lPacketTotalSize;

		//���ռ���
		pPacketContainer = new char[lPacketTotalSize];
		if (!pPacketContainer) return FALSE;
		memset(pPacketContainer, 0, lPacketTotalSize);
		pPacketContainerTmp = pPacketContainer;

		
		//ҵ���
		memmove(pPacketContainerTmp, pBusinessDataMoveTmp, lSingleDataSize);
		pBusinessDataMoveTmp += lSingleDataSize;
		pPacketContainerTmp += lSingleDataSize;

		//���ռ�
		pDataCacheToSend = new CACHE_T;
		if (!pDataCacheToSend) return FALSE;
		memset(pDataCacheToSend, 0, S_CACHE_T);
		pDataCacheToSend->pData = pPacketContainer;
		pDataCacheToSend->lDataLen = lPacketTotalSize;

		//һ�������ѹ�������������
		m_SocketPackCacheToSend.insert(map<long, CACHE_T *>::value_type(0, pDataCacheToSend));
	}

	//�ͷ�ҵ���ռ�
	delete[]ucpCacheTmp;

	//ҵ������ݻ���������
	ClearBusinessLayerToSendCache();

	//������
	lPackCount = m_SocketPackCacheToSend.size();

	return TRUE;

}

//�������ݰ�
BOOL CHttpService::SendDataToDest(BOOL bSocketBlockMode)
{
	//����У��
	//�Ƿ��Ѿ�����socket
	//	AfxMessageBox(TEXT("SendDataToDest 0"));
	if (m_nSocket < 0) return FALSE;
	//AfxMessageBox(TEXT("SendDataToDest 1"));
	//�Ƿ��Ѿ�����
	//�ж��Ƿ�������
	//if (!GetNetConnectStatus()) return FALSE;
	//	AfxMessageBox(TEXT("SendDataToDest 2"));
	//if (m_CacheDataQueue.empty()) return FALSE;

	//	AfxMessageBox(TEXT("SendDataToDest 3"));

	long lPackCount = 0;

	//��������װ��(����ҵ�������)
	if (!CreateSocketPacketToSend(lPackCount)) return FALSE;

	//��������Ҫ����
	if (m_SocketPackCacheToSend.empty()) return FALSE;
	//AfxMessageBox(TEXT("SendDataToDest 4"));

	//��������ģʽ
	if (bSocketBlockMode)
	{
		if (!SetBlockMode()) return FALSE;
	}
	else
	{
		if (!SetNoBlockMode()) return FALSE;
	}

	//��������
	char RecvCache[SOCKET_PACK_SIZE_MAX_LIMIT] = { 0 };
	long lRecvCacheSize = sizeof(RecvCache);
	BOOL bRecvReturn = FALSE;
	//fortest
	long lCountSuccess = 0;
	long lCountLen = 0;
	if (!m_SocketPackCacheToSend.empty())
	{
		//����
		for (long i = 0; i < m_SocketPackCacheToSend.size(); i++)
		{
			//ȡ��
			CACHE_T *pData = m_SocketPackCacheToSend[i];

			//TCHAR szRizhi[100] = { 0 };
			//TCHAR szResult[200] = { 0 };
			//CWriteDailyLog::AnsiToUnicode((char *)(((char *)pData->pData)), szRizhi, 100 * sizeof(TCHAR));
			//_sntprintf(szResult, 200, TEXT("%s, len %d "), szRizhi, pData->lDataLen);

			//����һ����
			if (SendData(pData->pData, &pData->lDataLen) == FALSE) return FALSE;

		}
	}


	//������� ��մ������������
	ClearSocketPackCacheToSend();

	return TRUE;

	//(�Ӵ���������ȡһ��������send������ˣ��ȴ�����˷��ؽ������Ӧ���ٷ���һ��������1s��δ�ظ��ط������ѭ��ֱ���Ѱ����꣬�����ն���)
	//��������
}

//�������ݰ�
//1.��һ�����ݰ�����������������ݰ���
//2.��λ��ͷ��β��һ������ͷ�����ֽ�Ϊ��ͷ��һ�����ĺ������ֽ�Ϊ��β��
//3.��λ�����ݳ��ȣ������յ��ֽ���Ӧ����֮�����(�ϰ�����Ȳ�����գ����г�ʱ���յȴ�,���޶�ʱ����δ���յ���������Ͽ����ӽ���ʧ�ܲ���������)
//4.��λ����ź��ܰ��������ڼ������մ�����
//5.ά��һ��ȥ�ذ����У��ѽ��յİ�����Ҫ�ٽ��գ�����ʾ���ճɹ�

//ѹ������ջ����������ؽ��ճɹ�Ӧ���

//�˳�while
//�����հ������ԣ����ĸ�����
//���в����ƴ��ҵ��㣬ƴ����ɺ����ҵ��㣬����ȴ�����ҵ��㻺����
//��ɽ���
BOOL CHttpService::RecvDataFromSrc(long lTvPerSec, long lTvPerUsec, int iTimeOutCount,  BOOL bSocketBlockMode)
{
	//MessageBox(NULL, TEXT("RecvDataFromSrc 01"), NULL, 0);
	//����У��
	//�Ƿ��Ѿ�����socket
	if (m_nSocket < 0) return FALSE;
	//�Ƿ��Ѿ�����
	//�ж��Ƿ�������
	//if (!GetNetConnectStatus()) return FALSE;
	//	if (!m_SocketPackCacheToRecv.empty()) return FALSE;

	//��մ��������ݻ����� �ڳ��ռ� socket������
	//ClearBusinessLayerToRecvCache();
	ClearSocketPackCacheToRecv();
	//MessageBox(NULL, TEXT("RecvDataFromSrc 02"), NULL, 0);
	//��������ģʽ
	if (bSocketBlockMode)
	{
		if (!SetBlockMode()) return FALSE;
	}
	else
	{
		if (!SetNoBlockMode()) return FALSE;
	}

	//	MessageBox(NULL, TEXT("RecvDataFromSrc 03"), NULL, 0);


	//���հ�
	char recvdata[HTTP_MAX_DATA_LIMIT] = { 0 };
	long lRecvDataLen = 0;
	lRecvDataLen = HTTP_MAX_DATA_LIMIT;
	BOOL bRecvSuccess = FALSE;
	int iCountTimeOut = iTimeOutCount;
	//	int iCountSuccessRecv = 0;		//�ɹ���������
	long lHttpDataLen = 0;				//���ܳ�
	long lEffectDataLen = 0;			//��Ч���ݳ���
	long lUnEffectDataLen = 0;			//��Ч���� http���ص�header ����Ч����
	long lPacketSeq = -1;
	//ACCESS_LAYER_T *pAccessLayer = NULL;

	CACHE_T *pDataCacheSave = NULL;
	m_lRecvCacheDataSize = 0;		//��

	//fortest
	long lCountSuccess = 0;
	long lCountLen = 0;

	//http����
	string strHttpResult;

	while (iCountTimeOut-->0)
	{
		//����

		
		//ucPacketNum = 1;
		lPacketSeq++;
		//pAccessLayer = NULL;
		pDataCacheSave = NULL;
		memset(recvdata, 0, HTTP_MAX_DATA_LIMIT);

		//��ʱ���� 100ms ��ʱ���
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
		//	_sntprintf(szResult, 200, TEXT("%s, len %d "), bRecvSuccess ? TEXT("�ɹ�recv") : TEXT("ʧ��recv"), lRecvDataLen);
		//MessageBox(NULL, szResult, NULL, 0);

		//д��־
		//if (lRecvDataLen < 0)
		//{
		//	CWriteDailyLog::WriteLog("����С��0", strlen("����С��0"), "Recv.txt");
		//}
		//else if (lRecvDataLen==0)
		//{
		//	CWriteDailyLog::WriteLog("���յ���0", strlen("���յ���0"), "Recv.txt");
		//}
		

		if (!bRecvSuccess) continue;

		//���泬�괦��

		iCountTimeOut = iTimeOutCount;

		//д��־
		//CWriteDailyLog::WriteLog("���յ�������:", strlen("���յ�������:"), "Recv.txt");
		//CWriteDailyLog::WriteLog(recvdata, lRecvDataLen, "Recv.txt");

		//��ȡ����	http����header����
		if (lPacketSeq == 0)
		{
			strHttpResult = recvdata;

			//����ɹ����ش���
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
			strCode.assign(strHttpResult, lCodePos, lCodeEndPos - lCodePos);	//ȡ�ð���code�ķ��ش���
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
			m_HttpBackStatusCode = atol((char *)strCodeNum.c_str());		//״̬��

			/*int iPos=strHttpResult.find("Content-Length: ", 0);	//��Щû�з���Content-Length
			if (iPos == string::npos)
			{
				//д��־
				CWriteDailyLog::WriteLog("�Ҳ������ĳ���", strlen("�Ҳ������ĳ���"), "Recv.txt");
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
			//lHttpDataLen = atol(HttpDataLen) + iEndPos+4;//��ǰ������ݼ����� 4��2�����з�
			lEffectDataLen = atol(HttpDataLen);				//��Ч���ĳ���*/

			//Ѱ�ҷ��ص���Ч����
			long lEffectContentPos = strHttpResult.find("\r\n\r\n", lCodeNumEndPos);
			if (lEffectContentPos == string::npos)
			{
				lPacketSeq--;
				iCountTimeOut = iTimeOutCount;
				continue;
			}

			lUnEffectDataLen = lEffectContentPos + strlen("\r\n\r\n");
			//lHttpDataLen = lEffectDataLen + lUnEffectDataLen;			//��Чͷ��Ϣ�������ܳ�
			lEffectDataLen = lRecvDataLen - lUnEffectDataLen;			//�õ���ǰ��Ч���ݳ���

			//����httpͷ��Ϣ
			if (!m_strHttpReturnHeader.empty()) m_strHttpReturnHeader.clear();
			m_strHttpReturnHeader.assign(strHttpResult, 0, lEffectContentPos);

			//Ѱ����Ч���ݴ�С
			lCodePos = m_strHttpReturnHeader.find("Content-Length:", 0);
			if (lCodePos != string::npos)
			{
				lCodeEndPos = m_strHttpReturnHeader.find("\r\n", lCodePos + strlen("Content-Length:"));
				if (lCodeEndPos != string::npos)
				{
					string strEffectDataLen;
					strEffectDataLen.assign(m_strHttpReturnHeader, lCodePos + strlen("Content-Length:"), lCodeEndPos - (lCodePos + strlen("Content-Length:")));
					//ȥ��β�ո�
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
			//sprintf(dd, "lHttpDataLen ���ȣ�%ld; lEffectDataLen:%d; lUnEffectDataLen:%d", lHttpDataLen, lEffectDataLen, lUnEffectDataLen);
		//	CWriteDailyLog::WriteLog(dd, strlen(dd), "Recv����.txt");
			
		}


		//�����
		//pAccessLayer = ((ACCESS_LAYER_T *)(recvdata + sizeof(unsigned short)));

		//У����ܳ�
		//usPacketSize = *((unsigned short *)(recvdata + sizeof(unsigned short)));
		//usPacketSize = pAccessLayer->usPacketSize;
		//�����سɹ�Ӧ����ͻ��˻ᳬʱ�ط�
		//if (usPacketSize != lRecvDataLen) continue;

		//������
	//	ucPacketNum = pAccessLayer->ucPacketNum;

		//�����
	//	ucPacketSeq = pAccessLayer->ucPacketSeq;

		//����ˮ��
		//uTransID = pAccessLayer->uTransID;


		//���ռ䱣��ҵ���߼�������
		pDataCacheSave = new CACHE_T;
		if (!pDataCacheSave) return FALSE;
		memset(pDataCacheSave, 0, S_CACHE_T);
		if (lPacketSeq == 0)
		{
			pDataCacheSave->lDataLen = lRecvDataLen - lUnEffectDataLen;	//��һ������������Ч����
		}			
		else
		{
			pDataCacheSave->lDataLen = lRecvDataLen;
		}

		//д��־
		//char len[100] = { 0 };
		//sprintf(len, "lRecvDataLen:%d; ��Ч��%d", lRecvDataLen, pDataCacheSave->lDataLen);
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
		
		//��ǰ��Ч��������
		m_stContentFlowCount.lCurrentFlowCount += pDataCacheSave->lDataLen;

		//ѹ�������socket����
		if (m_SocketPackCacheToRecv.find(lPacketSeq) == m_SocketPackCacheToRecv.end())
		{
			//�ۼӰ��������ڿ��ռ���װҵ�������
			m_lRecvCacheDataSize += pDataCacheSave->lDataLen;

			m_SocketPackCacheToRecv.insert(map<long, CACHE_T*>::value_type(lPacketSeq, pDataCacheSave));

			//���ͳɹ�����Ӧ���
			ClearBusinessLayerToSendCache();
			
			//TCHAR szRizhi[100] = { 0 };
			//TCHAR szResult[200] = { 0 };
			//CWriteDailyLog::AnsiToUnicode((char *)(((char *)pDataCacheSave->pData)), szRizhi, 100 * sizeof(TCHAR));
			//_sntprintf(szResult, 200, TEXT("%s; %s, ���ݣ�%s, len %d "), bRet ? TEXT("��ִ���ͳɹ�") : TEXT("��ִ����ʧ��"), bRecvSuccess ? TEXT("�ɹ�recv") : TEXT("ʧ��recv"), szRizhi, m_lRecvCacheDataSize);
			//MessageBox(NULL, szResult, NULL, 0);

			//fortest
			lCountSuccess++;


			//���ճɹ�����
			iCountTimeOut = iTimeOutCount;

			//����Ƿ�������
			//if (m_lRecvCacheDataSize >= lEffectDataLen) break;
			//if (m_SocketPackCacheToRecv.size() == ucPacketNum) break;
		}
	}

	if (!bRecvSuccess&&lRecvDataLen<0)
	{
		//AfxMessageBox(TEXT("����ʧ��"));
		return FALSE;
	}
	//AfxMessageBox(ridd);

	//ƴ�ӽ��յ���ҵ�������
	CreateBussnessLayerToGet();

	//����socket�ȴ����ջ�����
	//	ClearSocketPackCacheToRecv();

	return TRUE;
}

//���(������)��socket�����ջ���������ƴ�ӳ�ҵ������� �õ�һ��ҵ���߼�������
BOOL CHttpService::CreateBussnessLayerToGet()
{
	//����У��
	if (m_SocketPackCacheToRecv.empty() || m_lRecvCacheDataSize == 0) return FALSE;

	//���ռ�
	char *pBusinessLayerData = new char[m_lRecvCacheDataSize];
	if (!pBusinessLayerData) return FALSE;
	memset(pBusinessLayerData, 0, m_lRecvCacheDataSize);
	char *pDataTmp = pBusinessLayerData;

	//��˳��ȡ���� size����0��ʼ��size
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

	//У������������
	if (lBusinessTotalSize != m_lRecvCacheDataSize)
	{
		delete[]pBusinessLayerData;
		return FALSE;
	}

	//��������
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

	//��������ڳ��ռ�
	ClearBusinessLayerToRecvCache();

	//����������ҵ��㻺����
	if (pBusinessDataCache->pData)
	{
		m_RecvCacheDataQueue.push_back(pBusinessDataCache);
	}

	//����ռ�
	if (pBusinessLayerData) delete[]pBusinessLayerData;

	//������������ݻ�����socket
	ClearSocketPackCacheToRecv();

	return TRUE;
}

//ȡ�����յ�������
BOOL CHttpService::GetOutRecvData(void **pData, long *lDataLen)
{
	//����У��
	if (pData == NULL || lDataLen == NULL) return FALSE;
	if (m_RecvCacheDataQueue.empty()) return FALSE;

	char **pDataTmp = (char **)pData;

	//AfxMessageBox(TEXT("GetOutRecvData 01"));

	//ȡ����
	//���ռ�
	//����Ǩ��
	//����ռ�
	//���
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

			//���������ͷ�
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

//�ͷ�ȡ����������
BOOL CHttpService::ReleaseRecvDataOut()
{
	if (m_pRecvDataOut)
	{
		delete[] m_pRecvDataOut;
		m_pRecvDataOut = NULL;
	}

	return TRUE;
}


//��մ�����������  3���е�send
BOOL CHttpService::ClearSocketPackCacheToSend()
{
	//��������
	map<long, CACHE_T*>::iterator itCacheTmp;
	CACHE_T* pCacheTmpValue = NULL;

	//��մ���������
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


//��մ����հ������� 3���е�recv
BOOL CHttpService::ClearSocketPackCacheToRecv()
{
	//��������
	map<long, CACHE_T*>::iterator itCacheTmp;
	CACHE_T* pCacheTmpValue = NULL;

	//��մ���������
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

//socketҵ������ݻ��������������� 1��
BOOL CHttpService::ClearBusinessLayerToSendCache()
{
	m_lCacheDataSize = 0;

	//��������
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

//socketҵ������ݻ��������������� 2��
BOOL CHttpService::ClearBusinessLayerToRecvCache()
{
	m_lRecvCacheDataSize = 0;


	//��������
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

//����httpͷ��Ϣ
BOOL CHttpService::ClearHttpHeader()
{
	m_strHttpReturnHeader.clear();
	m_HttpBackStatusCode = -1;		//�ɹ��Ĵ���
	return TRUE;
}

//��������ͳ��
void CHttpService::ResetEffectFlowCount()
{
	memset(&m_stContentFlowCount, 0, sizeof(m_stContentFlowCount));
}

//��ջ��������ݶ��� �������������
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

//http�ͻ���
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

//���ӷ�����
BOOL CHttpClient::ConnectServer()
{
	//�����������
	if (!StartService()) return FALSE;

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
BOOL CHttpClient::CloseLink()
{
	return CloseService();
}

//���������������
BOOL CHttpClient::SendDataToServer(void *pData, long lDataLen)
{
	//����У��
	if (!pData) return FALSE;

	if (!AddDataToSend(pData, lDataLen)) return FALSE;

	BOOL bSend = SendDataToDest();

	return bSend;
}

//�ӷ�������ȡ����
BOOL CHttpClient::RecvDataFromServer(void **pData, long *lDataLen, long lTvPerSec, long lTvPerUsec, int iTimeOutCount, BOOL bSocketBlockMode )
{
	//����У��
	if ((!pData) || (!lDataLen)) return FALSE;

	if (!RecvDataFromSrc(lTvPerSec, lTvPerUsec, iTimeOutCount, bSocketBlockMode)) return FALSE;

	return GetOutRecvData(pData, lDataLen);
}

//�������ã�����Ͽ����ӣ�
BOOL CHttpClient::RefreshConfig(char *host/*����*/, int hostlen/*����*/, unsigned short port/*�˿�*/, bool bIsDomainName/*�Ƿ�����*/)
{
	if (!host || hostlen <= 0) return FALSE;

	if (!m_strServerAddr.empty()) m_strServerAddr.clear();

	m_strServerAddr = host;
	m_ServerPort = port;
	m_bIsDomainName = bIsDomainName;

	return TRUE;
}

//�����ļ�
BOOL CHttpClient::DownLoadFile(char *szFileLink, long lLinkLen, char *szSaveAsFileName, long lFileNameLen, unsigned short port, bool bDownLoadToMemory, long lSec, long lUSec, int iTimeOutCount, BOOL bSocketBlockMode)
{
	//У�����
	if (!szFileLink || lLinkLen <= 0 || !szSaveAsFileName || lFileNameLen <= 0) return FALSE;

	string strLink = szFileLink;
	string strHost;

	//��ʼ��
	if (!m_strHttpReturnHeader.empty()) m_strHttpReturnHeader.clear();
	m_HttpBackStatusCode = -1;

	//ȡ������
	long lLinkHeadPos = strLink.find("http://", 0);			//��httpͷ
	if (lLinkHeadPos == string::npos) return FALSE;

	long	lHostEndPos = strLink.find("/", lLinkHeadPos + strlen("http://"));		//��������βб��/
	if (lHostEndPos == string::npos) return FALSE;

	//��ȡ����
	strHost.assign(strLink, lLinkHeadPos + strlen("http://"), lHostEndPos - (lLinkHeadPos + strlen("http://")));

	char szGetRequestStr[2048] = { 0 };
	sprintf(szGetRequestStr, CLIENT_HTTP_GET_API_STR, szFileLink, strHost.c_str());

	//����
	char *pRecvData = NULL;
	long lRecvDataLen = 0;

	//���õ�ַ�˿ڵ�
	RefreshConfig((char *)strHost.c_str(), strlen((char *)strHost.c_str()), port, true);

	//����
	if (!ConnectServer())
	{
		return FALSE;
	}

	//��������
	if (SendDataToServer(szGetRequestStr, strlen(szGetRequestStr)))
	{
		BOOL bRet = RecvDataFromServer((void **)&pRecvData, &lRecvDataLen, lSec, lUSec, iTimeOutCount, bSocketBlockMode);

		if (!bRet) return FALSE;

		if (pRecvData&&m_HttpBackStatusCode == HTTP_REQUEST_SUCCESS_RET_CODE)
		{
			if (bDownLoadToMemory)
			{
				//���ص��ڴ�
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
				//�����ļ�
				CWriteDailyLog::SaveDataAsFile(pRecvData, lRecvDataLen, szSaveAsFileName);
			}


			//�ͷŻ���
			ReleaseRecvDataOut();
		}
		else
		{
			//�ͷŻ���
			ReleaseRecvDataOut();

			//����ʧ��
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

//������ݻ���
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

//ʹ���ڴ滺���
bool CHttpClient::UseDataCache(bool bUse)
{
	m_bUseDataCache = bUse;
	return true;
}