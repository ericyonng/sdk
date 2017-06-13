#ifndef TCP_SOCKET_H_
#define TCP_SOCKET_H_
#pragma once
#include"NetworkServiceHead.h"
#include"SocketBase.h"
#include"SocketProtocol.h"
#include<list>
#include<map>
using namespace std;

//��ַ��Ϣ��
class NETWORK_SERVICE_CLASS CAddr
{
protected:
	struct sockaddr_in						m_stAddr_Info;						//��ַ����Ϣ�ṹ���°�
	char									m_DomainName[HOST_ADDR_LEN];		//����
	socklen_t								m_scktst_len;						//��ַ����Ϣ�ṹ��ռ��С
	BOOL									m_bSetAddr;							//�Ƿ�����

	//��ʼ����ַ
public:
	//Ĭ�Ϲ��첻��ʼ��
	CAddr();					
	//��ͨ�����ʼ������
	CAddr(char *szip, unsigned short usPort, unsigned short usFamily = AF_INET);						
	//����
	virtual ~CAddr();
	//��λ
	void Clear();
	//�ͷŶ���
	virtual void Release(){ try{ delete this; } catch (...){} }

	//���ú���
public:	
	//���ظ�ֵ�����
	CAddr & operator =(CAddr &addr);
	//����ip
	void Setip(char *szip);		
	//��������Э��������IPv4����IPv6
	void SetFamily(unsigned short shFamily = AF_INET);				
	//���ö˿ں�
	void SetPort(unsigned short shPort);				
	//���õ�ַ��ṹ��
	void SetAddrInfo(struct sockaddr_in &stAddr_Info);		
	//��ַ
	void SetAddrInfo(char *szip, unsigned short port, unsigned short family);	
	//��ַ
	BOOL SetAddrInfoByDomainName(char *domain, int domainlen, unsigned short port, unsigned short family);	

	//��ȡ��Ϣ
public:
	//����
	char *GetDomainName();
	//��ȡip
	char *Getip();
	//��ȡ�˿ں�
	unsigned short GetPort();
	//��ȡ����Э������
	unsigned short GetFamily();
	//��ȡ��ַ�ṹ�壨ǿתΪ�ɰ�ṹ��ָ�룩
	struct sockaddr * GetpAddrInfo();	
	//��ȡ��ַ�ṹ���°�
	struct sockaddr_in * GetpAddrInInfo();
	//��ȡ���͵�ַ�ṹ��ռ��С
	int GetSocketInfoLen();		
	//��ȡ socklen_t ���͵�ַ�ṹ��ռ��Сָ��
	socklen_t* GetSocketInfoLenPoint();	
	//��ȡ����״̬
	BOOL GetSetAddrStatus(){ return m_bSetAddr; }								
private:
};
/////////////////////////////////////////////////////////////////////////////

//WSAStartup��Ŀ���ǳ�ʼ��winsock DLL��ÿ��Ӧ�ó������һ�ξ����˰ɣ�����ÿ���̵߳���һ�ΰ�
//�����ɹ�����ֵΪ0; ���򷵻�ֵΪSOCKET_ERROR������ͨ������WSAGetLastError��ȡ������롣

//��һ�����̵߳Ļ����£�WSACleanup()��ֹ��Windows Sockets�������߳��ϵĲ���.
//�����ַ��Ϣ�ṹ��
class NETWORK_SERVICE_CLASS CTcpSocket :public ISocketEngine
{
public:
	CTcpSocket();
	virtual ~CTcpSocket();

	//�ӿڹ���
public:
	//���õ�ַ
	virtual void SetDestAddr(void *pAddr);
	//����
	virtual bool Start(){ return false; }
	//����
	virtual bool SendDataTo(void *pData, long lDataLen){ return false; }
	//����
	virtual bool RecvDataFrom(void **pData, long *plDataLen){ return false; }
	//�ͷŶ���
	virtual void Release(){ try{ delete this; } catch (...){} }

	//��������
public:
	//���õ�ַ
	BOOL SetAddrInfo(CAddr &addr);
	//���õ�ַ
	BOOL SetAddrInfo(char *domain, int domainlen, unsigned short port, unsigned short family);
	//���õ�ַ
	BOOL SetAddrInfo(char *ip, int iplen, unsigned short port, unsigned short family, BOOL bNotDomainName);
	//��������ͳ��
	void ResetDataFlowCount();
	//���socket������
	BOOL ClearSocketCache();
	//�����׽���
	VOID SetSocketFD(SOCKET hSock);
	//��ȡ�׽���
	SOCKET GetSocketFD(){ return m_nSocket; }
	//�����Զ��ر��׽���ģʽ
	void SetAutoCloseSocket(bool bAutoClose){ m_bAutoCloseSocket = bAutoClose; }
	//����ͳ��
	DATA_FLOW_COUNT_T *GetFlowCount(){ return &m_stDataFlowCount; }

	//peer����ȡ����
public:
	//��ȡ�Է�ip�˿�
	int GetPeerIP(char *peerip,int iPeerIPSize, unsigned short *peerport);
	//��ȡ���ص�ַ�˿�
	int GetLocalIP(char *localipaddr, int ipaddrsize, unsigned short *localport);

	//win32dll ���绷����ʼ��
public:
	//����socket����
	static BOOL InitSocketEnvironment();
	//������
	static BOOL CleanSocketEnvironment();

	//�׽��ֲ���
protected:
	//����ģʽ
	BOOL SetBlockMode();
	//������ģʽ
	BOOL SetNoBlockMode();
	//�׽��ֳ�ʱ���	1s
	BOOL IsMonitorSocketTimeOut(bool bReadableDetect = true, bool bWriteableDetect = false, int *pErrOut = NULL, bool bSetOneAtLeast = false, bool bInfiniteWaiting = false);
	//�׽��ֳ�ʱ���
	BOOL DelayMonitorSocket(long tv_sec, long tv_usec, bool bReadableDetect = true, bool bWriteableDetect = false, int *pErrOut = NULL, bool bSetOneAtLeast = false, bool bInfiniteWaiting=false);
	//����
	VOID Clear();
	//����״̬
	BOOL GetConnectStatus(){ return m_bConnected; }

	//�������
protected:
	//�׽���
	virtual BOOL CreateSocket();

	//�Ͽ�����
	virtual bool Close();
	//���ŵĶϿ�����
	virtual BOOL GraciousClose(int iHowto);
	//��������
	virtual BOOL SendData(VOID *pData, long *plDataLen);
	//��������
	virtual BOOL RecvData(VOID *pData, long *plDataLen);

	//��������״̬
	BOOL GetNetConnectStatus(){ return m_bConnected; }

protected:
	static bool					m_bInitEnvironment;					//�Ƿ��ʼ�����绷��

	//��Ҫ����
protected:
	SOCKET						m_nSocket;							//�׽���
	bool						m_bAutoCloseSocket;					//�Զ��ر��׽���
	CAddr						m_SocketAddr;						//��ַ
	BOOL						m_bConnected;						//����״̬
	WSADATA						m_wsaData;							//socket����
	fd_set						m_fs_read;							//�ɶ����
	fd_set						m_fs_write;							//��д���
	BOOL						m_bSocketReadable;					//�ɶ�
	BOOL						m_bSocketWriteable;					//��д

	//����ͳ��
protected:
	DATA_FLOW_COUNT_T			m_stDataFlowCount;					//����ͳ��

};

////////////////////////////////////////////////////////////////


//�������
class NETWORK_SERVICE_CLASS CNetworkService :public CTcpSocket
{
public:
	//��ʼ��socket����
	CNetworkService();
	//�ر�socket������socket����
	virtual ~CNetworkService();

//�ⲿ�ӿ�
public:
	//�ͷ�ȡ����������
	BOOL ReleaseRecvDataOut();
	//��ȫ����ʹ��
	void SetTransferSecurity(bool bSet){ m_bSetTransferSecurity = bSet; }
	//��ȫ��������
	bool GetTransferSecurityStatus(){ return m_bSetTransferSecurity ; }

//�ڲ��ӿ�

	//����������
protected:
	//�����������
	virtual BOOL StartNetService();

	//�Ͽ�����
	virtual BOOL	CloseNetService();

	//����ģ��
protected:
	//����ģ��
	//ҵ���߼���������� ���֮ǰ����ҵ�������
	virtual bool AddDataToSend(void *pData, long lDataLen);
	//�������ݰ�
	virtual bool SendDataToDest(BOOL bSocketBlockMode = FALSE, BOOL bNeedSocketBlockSet = TRUE, bool bNeedReturnPacket = true, long lTvPerSec = 1, long lTvPerUsec = 0, int iTimeOutCountSet = TIME_OUT_COUNT);
	//�Է��Ƿ���ճɹ�ȷ��
	virtual BOOL IsDestRecvSuccess(/*��ȷ�ϵİ����*/long lCurrentPackSeq, BOOL bSocketBlockMode, BOOL bNeedSocketBlockSet);
	//������ һ�����е�ҵ��������� BUSINESS_DATA_LIMIT����
	virtual BOOL CreateSocketPacketToSend(long &lPackCount);

	//����ģ��
protected:
	//ȡ�����յ�������
	virtual BOOL GetOutRecvData(void **pData, long *lDataLen);
	//�������ݰ�
	virtual BOOL RecvDataFromSrc(BOOL bSocketBlockMode = FALSE, BOOL bNeedSocketBlockSet = TRUE, BOOL bNeedSendSuccessRecvReturnPack = TRUE, long lTvPerSec = 1, long lTvPerUsec = 0, int iTimeOutCountSet = TIME_OUT_COUNT);
	//���(������)��socket�����ջ���������ƴ�ӳ�ҵ������� �õ�һ��ҵ���߼�������
	virtual BOOL CreateBussnessLayerToGet();

	//rsa key����
protected:
	//���ù�Կ
	void SetRsaPublicKey(unsigned char *ucKey);
	//����˽Կ
	void SetRsaPrivateKey(unsigned char *ucKey);

	//������
protected:
	//��ջ��������ݶ��� �������������
	virtual BOOL ClearAllDataCache();
	//��մ����������� 3���е�send
	virtual BOOL ClearSocketPackCacheToSend();
	//��մ����հ������� 3���е�recv
	virtual BOOL ClearSocketPackCacheToRecv();
	//socketҵ������ݻ��������������� 1��
	virtual BOOL ClearBusinessLayerToSendCache();
	//socketҵ������ݻ��������������� 2��
	virtual BOOL ClearBusinessLayerToRecvCache();

	//rsa��Կ����
protected:
	string					m_strRsaPublicKey;				//��Կ
	string					m_strRsaPrivateKey;				//˽Կ
	bool					m_bSetTransferSecurity;			//�Ƿ����ô��䰲ȫ

	//ҵ�����������ݻ����� 1��
protected:
	long					m_lCacheDataSize;				//ҵ���߼��������ܳ���
	SOCKET_PACK_HEAD_T		m_stPackeHead;					//��ͷ
	ACCESS_LAYER_T			m_AccessLayer;					//���Ӳ�����
	list<CACHE_T *>			m_CacheDataQueue;				//ҵ������ݻ���
	SOCKET_PACK_TAIL		m_stPackeTail;					//��β

	//ҵ�����������ݻ�����	2��
protected:
	long					m_lRecvCacheDataSize;				//ҵ���߼��������ܳ���
	list<CACHE_T *>			m_RecvCacheDataQueue;				//ҵ������ݻ���
	void *					m_pRecvDataOut;						//ȡ������������Ҫ�ͷ�

	//socket������ 3��
protected:
	map<long, CACHE_T *>		m_SocketPackCacheToSend;		//���ͻ�����	���ڷ���ʧ�ܵ��ط�	keyֵΪ�����
	map<long, CACHE_T *>		m_SocketPackCacheToRecv;		//���ջ�����	���ڽ������ݰ�		keyֵΪ�����

	//Ŀ������
protected:
	string				m_strServerAddr;			//Ŀ�ĵ�ַ
	unsigned short		m_ServerPort;				//Ŀ�Ķ˿�
	bool				m_bIsDomainName;			//Ŀ���Ƿ�����

};

//�ͻ��� ��Ҫ���ù�˽Կ
class NETWORK_SERVICE_CLASS CNetworkTcpClient :public CNetworkService
{
public:
	//��ʼ�����л���
	CNetworkTcpClient();
	virtual ~CNetworkTcpClient();

	//�������
public:
	//����
	virtual bool Start();
	//���ӷ�����
	virtual BOOL Link(char *pServerAddr, unsigned short port, bool bClearDataCache = true);
	//�ر�����
	virtual BOOL CloseLink();
	//����
	virtual bool SendDataTo(void *pData, long lDataLen);
	//���� ���պ���Ҫ�ֶ��ͷſռ�
	virtual bool RecvDataFrom(void **pData, long *lDataLen);
	//�������ã�����Ͽ����ӣ�
	virtual BOOL RefreshConfig(char *host/*����*/, int hostlen/*����*/, unsigned short port/*�˿�*/, bool bIsDomainName/*�Ƿ�����*/);
	//rsa	key :���ر���server�˿��Խ��ܵļ��������Լ�һ��ר���ڿͻ��˵Ľ�����������˱���ͻ��˿��Խ��ܵļ������Լ�ר���ڷ���˵Ľ�����

protected:
	//����Զ��
	virtual BOOL Connect();
};

//������

//����� �����첽IOģ�ͣ�������չ��iocpģ�� ��Ҫ���ù�˽Կ
class NETWORK_SERVICE_CLASS CNetworkTcpServer :public CNetworkService
{
public:
	CNetworkTcpServer();
	virtual ~CNetworkTcpServer();

	//�����ر�����
public:
	//����
	virtual bool Start();
	//�رշ�����
	virtual BOOL CloseServer();
	//��������	(�����ȹرշ�����)
	virtual BOOL RefreshConfig(char *host/*����*/, int hostlen/*����*/, unsigned short port/*�˿�*/, bool bIsDomainName/*�Ƿ�����*/);

	//��������
protected:
	//�󶨵�ַ
	virtual BOOL Bind(char *ip, int iplen, unsigned short port, unsigned short family);			//�д���ȶ
	//�����˿� ֧��200�����Ӷ��еȴ� //���֧��250������
	virtual BOOL Listen(int iBacklog = LISTEN_MAX_LIMIT);
	//ȷ�����Ӳ���ȡ�ͻ�����Ϣ
	BOOL Accept(SOCKET *pSocketFDForClient, CAddr *pRemoteAddr = NULL);
	//�����첽IOģ��
	virtual BOOL SetAsynchronousServer();
	//����IO�¼� ������ �пɶ��¼����Ƴ�����ͻ��˱��ֶ����� ��Ҫһ���߳�ȥִ�пɶ��¼� �������׽���������Ҫ���ٶ���߳�ִ�м������� ������չ��IOCP
	virtual BOOL WaitForEvent();
	//��Ӽ���¼�
	virtual BOOL AddEvent(SOCKET hClntSock);


	//������������
public:
	//����
	virtual bool RecvDataFrom(void **pData, long *lDataLen);
	//����
	virtual bool SendDataTo(void *pData, long lDataLen);

	//�첽��������
protected:
	//����socket����
	virtual void CompressSockets(SOCKET hSockArr[], int idx, int total);
	//�����¼�����
	virtual void CompressEvents(WSAEVENT hEventArr[], int idx, int total);

protected:
	bool				m_bServerStart;				//�������Ƿ�����
	//rsa	key :���ر���server�˿��Խ��ܵļ��������Լ�һ��ר���ڿͻ��˵Ľ�����������˱���ͻ��˿��Խ��ܵļ������Լ�ר���ڷ���˵Ľ�����

	//�첽IO����
protected:
	SOCKET				m_hSockArr[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT			m_hEventArr[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT			m_newEvent;
	WSANETWORKEVENTS	m_netEvents;
	int					m_NumOfClntSock;
};

///////////////////////////////////////////////////////////////////////
#define HTTP_REQUEST_SUCCESS_RET_CODE		200		//����ɹ����ش���

//http����
class NETWORK_SERVICE_CLASS CHttpService :public CTcpSocket
{

public:
	//��ʼ��socket����
	CHttpService(LPTSTR lpszRecvLockerName = NULL, bool bLockAtOnce = false, bool bInitiallyOwn = false, bool bManualReset = true);
	//�ر�socket������socket����
	virtual ~CHttpService();


	//����������
public:
	//�����������
	virtual BOOL StartService();
	//���ӷ�����
	virtual BOOL Link(char *pServerAddr, unsigned short port, bool bClearDataCache = true);
	//�Ͽ�����
	virtual BOOL CloseService();

	//�ڲ��ӿ�
protected:
	//����Զ��
	virtual BOOL Connect();

	//��������
public:
	EFFECT_DATA_FLOW_COUNT_T *GetEffectDataFlowCount(){ return &m_stContentFlowCount; }

	//����ģ��
public:
	//����ģ��
	//ҵ���߼���������� ���֮ǰ����ҵ�������
	BOOL AddDataToSend(void *pData, long lDataLen);
	//�������ݰ�
	BOOL SendDataToDest(BOOL bSocketBlockMode = FALSE);

	//����ģ��
public:
	//ȡ�����յ�������
	BOOL GetOutRecvData(void **pData, long *lDataLen);
	//�������ݰ�
	BOOL RecvDataFromSrc(long lTvPerSec=1, long lTvPerUsec = 0, int iTimeOutCount = TIME_OUT_COUNT, BOOL bSocketBlockMode = FALSE);
	//�ͷ�ȡ����������
	BOOL ReleaseRecvDataOut();

	//�ڲ��ӿ�

	//���ݰ�����
protected:
	//������ һ�����е�ҵ��������� BUSINESS_DATA_LIMIT����
	BOOL CreateSocketPacketToSend(long &lPackCount);
	//���(������)��socket�����ջ���������ƴ�ӳ�ҵ������� �õ�һ��ҵ���߼�������
	BOOL CreateBussnessLayerToGet();

	//������
public:
	//��ջ��������ݶ��� �������������
	BOOL ClearAllDataCache();
	//��մ����������� 3���е�send
	BOOL ClearSocketPackCacheToSend();
	//��մ����հ������� 3���е�recv
	BOOL ClearSocketPackCacheToRecv();
	//socketҵ������ݻ��������������� 1��
	BOOL ClearBusinessLayerToSendCache();
	//socketҵ������ݻ��������������� 2��
	BOOL ClearBusinessLayerToRecvCache();
	//����httpͷ��Ϣ
	BOOL ClearHttpHeader();
	//��������ͳ��
	void ResetEffectFlowCount();

	//http״̬����
public:
	long					m_HttpBackStatusCode;			//״̬��
	string					m_strHttpReturnHeader;			//���ص�httpͷ��Ϣ

	//ҵ�����������ݻ����� 1��
protected:
	long					m_lCacheDataSize;				//ҵ���߼��������ܳ���
	list<CACHE_T *>			m_CacheDataQueue;				//ҵ������ݻ���

	//ҵ�����������ݻ�����	2��
protected:
	long					m_lRecvCacheDataSize;				//ҵ���߼��������ܳ���
	list<CACHE_T *>			m_RecvCacheDataQueue;				//ҵ������ݻ���
	void *					m_pRecvDataOut;						//ȡ������������Ҫ�ͷ�

	//socket������ 3��
protected:
	map<long, CACHE_T *>		m_SocketPackCacheToSend;		//���ͻ�����	���ڷ���ʧ�ܵ��ط�	keyֵΪ�����
	map<long, CACHE_T *>		m_SocketPackCacheToRecv;		//���ջ�����	���ڽ������ݰ�		keyֵΪ�����

	//���ջ������ػ�
protected:
	//CSynLocker	*				m_pRecvCacheLocker;				//���ջ�������

	//���ݳ�Ա
protected:
	string							m_strServerAddr;			//��������ַ
	unsigned short					m_ServerPort;				//�������˿�
	bool							m_bIsDomainName;			//�Ƿ�����
	EFFECT_DATA_FLOW_COUNT_T		m_stContentFlowCount;	//��ʵ��Ч��������ͳ��

};

//http�ͻ���
class NETWORK_SERVICE_CLASS CHttpClient :public CHttpService
{
public:
	string                  m_strHttpData;
	char	*				m_pHttpDataCache;
	unsigned long			m_ulHttpDataSize;
	bool					m_bUseDataCache;

public:
	CHttpClient();
	virtual ~CHttpClient();

	//���ط���
public:
	//�����ļ�
	BOOL DownLoadFile(char *szFileLink, long lLinkLen, char *szSaveAsFileName, long lFileNameLen, unsigned short port = 80, bool bDownLoadToMemory = false, long lSec = 1, long lUSec = 0, int iTimeOutCount = TIME_OUT_COUNT, BOOL bSocketBlockMode = FALSE);

	//�������
public:
	//���ӷ�����
	BOOL ConnectServer();
	//�ر�����
	virtual BOOL CloseLink();
	//���������������
	BOOL SendDataToServer(void *pData, long lDataLen);
	//�ӷ�������ȡ���� �ǵ��ͷſռ�
	BOOL RecvDataFromServer(void **pData, long *lDataLen, long lTvPerSec = 1, long lTvPerUsec = 0, int iTimeOutCount = TIME_OUT_COUNT, BOOL bSocketBlockMode = FALSE);
	//�������ã�����Ͽ����ӣ�
	BOOL RefreshConfig(char *host/*����*/, int hostlen/*����*/, unsigned short port/*�˿�*/, bool bIsDomainName/*�Ƿ�����*/);

	//���ݻ���
public:
	//������ݻ���
	virtual bool ReleaseHttpDataCache();
	//ʹ���ڴ滺���
	bool UseDataCache(bool bUse = true);
};

//IPC
#endif