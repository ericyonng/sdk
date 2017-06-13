#ifndef TCP_SOCKET_H_
#define TCP_SOCKET_H_
#pragma once
#include"NetworkServiceHead.h"
#include"SocketBase.h"
#include"SocketProtocol.h"
#include<list>
#include<map>
using namespace std;

//地址信息类
class NETWORK_SERVICE_CLASS CAddr
{
protected:
	struct sockaddr_in						m_stAddr_Info;						//地址类信息结构体新版
	char									m_DomainName[HOST_ADDR_LEN];		//域名
	socklen_t								m_scktst_len;						//地址类信息结构体空间大小
	BOOL									m_bSetAddr;							//是否设置

	//初始化地址
public:
	//默认构造不初始化
	CAddr();					
	//普通构造初始化数据
	CAddr(char *szip, unsigned short usPort, unsigned short usFamily = AF_INET);						
	//析构
	virtual ~CAddr();
	//复位
	void Clear();
	//释放对象
	virtual void Release(){ try{ delete this; } catch (...){} }

	//配置函数
public:	
	//重载赋值运算符
	CAddr & operator =(CAddr &addr);
	//设置ip
	void Setip(char *szip);		
	//设置网络协议类型是IPv4还是IPv6
	void SetFamily(unsigned short shFamily = AF_INET);				
	//设置端口号
	void SetPort(unsigned short shPort);				
	//设置地址类结构体
	void SetAddrInfo(struct sockaddr_in &stAddr_Info);		
	//地址
	void SetAddrInfo(char *szip, unsigned short port, unsigned short family);	
	//地址
	BOOL SetAddrInfoByDomainName(char *domain, int domainlen, unsigned short port, unsigned short family);	

	//读取信息
public:
	//域名
	char *GetDomainName();
	//获取ip
	char *Getip();
	//获取端口号
	unsigned short GetPort();
	//获取网络协议类型
	unsigned short GetFamily();
	//获取地址结构体（强转为旧版结构体指针）
	struct sockaddr * GetpAddrInfo();	
	//获取地址结构体新版
	struct sockaddr_in * GetpAddrInInfo();
	//获取整型地址结构体空间大小
	int GetSocketInfoLen();		
	//获取 socklen_t 类型地址结构体空间大小指针
	socklen_t* GetSocketInfoLenPoint();	
	//获取设置状态
	BOOL GetSetAddrStatus(){ return m_bSetAddr; }								
private:
};
/////////////////////////////////////////////////////////////////////////////

//WSAStartup的目的是初始化winsock DLL，每个应用程序调用一次就行了吧，不用每个线程调用一次吧
//操作成功返回值为0; 否则返回值为SOCKET_ERROR，可以通过调用WSAGetLastError获取错误代码。

//在一个多线程的环境下，WSACleanup()中止了Windows Sockets在所有线程上的操作.
//定义地址信息结构体
class NETWORK_SERVICE_CLASS CTcpSocket :public ISocketEngine
{
public:
	CTcpSocket();
	virtual ~CTcpSocket();

	//接口功能
public:
	//设置地址
	virtual void SetDestAddr(void *pAddr);
	//启动
	virtual bool Start(){ return false; }
	//发送
	virtual bool SendDataTo(void *pData, long lDataLen){ return false; }
	//接收
	virtual bool RecvDataFrom(void **pData, long *plDataLen){ return false; }
	//释放对象
	virtual void Release(){ try{ delete this; } catch (...){} }

	//辅助功能
public:
	//设置地址
	BOOL SetAddrInfo(CAddr &addr);
	//设置地址
	BOOL SetAddrInfo(char *domain, int domainlen, unsigned short port, unsigned short family);
	//设置地址
	BOOL SetAddrInfo(char *ip, int iplen, unsigned short port, unsigned short family, BOOL bNotDomainName);
	//重置流量统计
	void ResetDataFlowCount();
	//清空socket缓冲区
	BOOL ClearSocketCache();
	//设置套接字
	VOID SetSocketFD(SOCKET hSock);
	//获取套接字
	SOCKET GetSocketFD(){ return m_nSocket; }
	//设置自动关闭套接字模式
	void SetAutoCloseSocket(bool bAutoClose){ m_bAutoCloseSocket = bAutoClose; }
	//流量统计
	DATA_FLOW_COUNT_T *GetFlowCount(){ return &m_stDataFlowCount; }

	//peer方获取属性
public:
	//获取对方ip端口
	int GetPeerIP(char *peerip,int iPeerIPSize, unsigned short *peerport);
	//获取本地地址端口
	int GetLocalIP(char *localipaddr, int ipaddrsize, unsigned short *localport);

	//win32dll 网络环境初始化
public:
	//启动socket环境
	static BOOL InitSocketEnvironment();
	//清理环境
	static BOOL CleanSocketEnvironment();

	//套接字操作
protected:
	//阻塞模式
	BOOL SetBlockMode();
	//非阻塞模式
	BOOL SetNoBlockMode();
	//套接字超时监控	1s
	BOOL IsMonitorSocketTimeOut(bool bReadableDetect = true, bool bWriteableDetect = false, int *pErrOut = NULL, bool bSetOneAtLeast = false, bool bInfiniteWaiting = false);
	//套接字超时监控
	BOOL DelayMonitorSocket(long tv_sec, long tv_usec, bool bReadableDetect = true, bool bWriteableDetect = false, int *pErrOut = NULL, bool bSetOneAtLeast = false, bool bInfiniteWaiting=false);
	//重置
	VOID Clear();
	//链接状态
	BOOL GetConnectStatus(){ return m_bConnected; }

	//网络操作
protected:
	//套接字
	virtual BOOL CreateSocket();

	//断开链接
	virtual bool Close();
	//优雅的断开连接
	virtual BOOL GraciousClose(int iHowto);
	//发送数据
	virtual BOOL SendData(VOID *pData, long *plDataLen);
	//接收数据
	virtual BOOL RecvData(VOID *pData, long *plDataLen);

	//网络链接状态
	BOOL GetNetConnectStatus(){ return m_bConnected; }

protected:
	static bool					m_bInitEnvironment;					//是否初始化网络环境

	//必要数据
protected:
	SOCKET						m_nSocket;							//套接字
	bool						m_bAutoCloseSocket;					//自动关闭套接字
	CAddr						m_SocketAddr;						//地址
	BOOL						m_bConnected;						//链接状态
	WSADATA						m_wsaData;							//socket环境
	fd_set						m_fs_read;							//可读检测
	fd_set						m_fs_write;							//可写检测
	BOOL						m_bSocketReadable;					//可读
	BOOL						m_bSocketWriteable;					//可写

	//流量统计
protected:
	DATA_FLOW_COUNT_T			m_stDataFlowCount;					//流量统计

};

////////////////////////////////////////////////////////////////


//网络服务
class NETWORK_SERVICE_CLASS CNetworkService :public CTcpSocket
{
public:
	//初始化socket环境
	CNetworkService();
	//关闭socket并清理socket环境
	virtual ~CNetworkService();

//外部接口
public:
	//释放取出来的数据
	BOOL ReleaseRecvDataOut();
	//安全传输使能
	void SetTransferSecurity(bool bSet){ m_bSetTransferSecurity = bSet; }
	//安全传输特性
	bool GetTransferSecurityStatus(){ return m_bSetTransferSecurity ; }

//内部接口

	//服务器链接
protected:
	//启动网络服务
	virtual BOOL StartNetService();

	//断开链接
	virtual BOOL	CloseNetService();

	//发送模块
protected:
	//接收模块
	//业务逻辑层数据添加 添加之前加密业务层数据
	virtual bool AddDataToSend(void *pData, long lDataLen);
	//发送数据包
	virtual bool SendDataToDest(BOOL bSocketBlockMode = FALSE, BOOL bNeedSocketBlockSet = TRUE, bool bNeedReturnPacket = true, long lTvPerSec = 1, long lTvPerUsec = 0, int iTimeOutCountSet = TIME_OUT_COUNT);
	//对方是否接收成功确认
	virtual BOOL IsDestRecvSuccess(/*待确认的包序号*/long lCurrentPackSeq, BOOL bSocketBlockMode, BOOL bNeedSocketBlockSet);
	//包创建 一个包中的业务层数据由 BUSINESS_DATA_LIMIT限制
	virtual BOOL CreateSocketPacketToSend(long &lPackCount);

	//接收模块
protected:
	//取出接收到的数据
	virtual BOOL GetOutRecvData(void **pData, long *lDataLen);
	//接收数据包
	virtual BOOL RecvDataFromSrc(BOOL bSocketBlockMode = FALSE, BOOL bNeedSocketBlockSet = TRUE, BOOL bNeedSendSuccessRecvReturnPack = TRUE, long lTvPerSec = 1, long lTvPerUsec = 0, int iTimeOutCountSet = TIME_OUT_COUNT);
	//解包(并解密)将socket待接收缓冲中数据拼接成业务层数据 得到一个业务逻辑层数据
	virtual BOOL CreateBussnessLayerToGet();

	//rsa key设置
protected:
	//设置公钥
	void SetRsaPublicKey(unsigned char *ucKey);
	//设置私钥
	void SetRsaPrivateKey(unsigned char *ucKey);

	//清理组
protected:
	//清空缓冲区数据队列 清除所有区数据
	virtual BOOL ClearAllDataCache();
	//清空待发包缓冲区 3区中的send
	virtual BOOL ClearSocketPackCacheToSend();
	//清空待接收包缓冲区 3区中的recv
	virtual BOOL ClearSocketPackCacheToRecv();
	//socket业务层数据缓冲区待发送清理 1区
	virtual BOOL ClearBusinessLayerToSendCache();
	//socket业务层数据缓冲区待接收清理 2区
	virtual BOOL ClearBusinessLayerToRecvCache();

	//rsa密钥设置
protected:
	string					m_strRsaPublicKey;				//公钥
	string					m_strRsaPrivateKey;				//私钥
	bool					m_bSetTransferSecurity;			//是否设置传输安全

	//业务层待发送数据缓冲区 1区
protected:
	long					m_lCacheDataSize;				//业务逻辑层数据总长度
	SOCKET_PACK_HEAD_T		m_stPackeHead;					//包头
	ACCESS_LAYER_T			m_AccessLayer;					//链接层数据
	list<CACHE_T *>			m_CacheDataQueue;				//业务层数据缓冲
	SOCKET_PACK_TAIL		m_stPackeTail;					//包尾

	//业务层待接收数据缓冲区	2区
protected:
	long					m_lRecvCacheDataSize;				//业务逻辑层数据总长度
	list<CACHE_T *>			m_RecvCacheDataQueue;				//业务层数据缓冲
	void *					m_pRecvDataOut;						//取出来的数据需要释放

	//socket包缓冲 3区
protected:
	map<long, CACHE_T *>		m_SocketPackCacheToSend;		//发送缓冲区	用于发送失败等重发	key值为包序号
	map<long, CACHE_T *>		m_SocketPackCacheToRecv;		//接收缓冲区	用于接收数据包		key值为包序号

	//目标属性
protected:
	string				m_strServerAddr;			//目的地址
	unsigned short		m_ServerPort;				//目的端口
	bool				m_bIsDomainName;			//目的是否域名

};

//客户端 需要设置公私钥
class NETWORK_SERVICE_CLASS CNetworkTcpClient :public CNetworkService
{
public:
	//初始化运行环境
	CNetworkTcpClient();
	virtual ~CNetworkTcpClient();

	//网络服务
public:
	//启动
	virtual bool Start();
	//链接服务器
	virtual BOOL Link(char *pServerAddr, unsigned short port, bool bClearDataCache = true);
	//关闭链接
	virtual BOOL CloseLink();
	//发送
	virtual bool SendDataTo(void *pData, long lDataLen);
	//接收 接收后需要手动释放空间
	virtual bool RecvDataFrom(void **pData, long *lDataLen);
	//更新配置（必须断开链接）
	virtual BOOL RefreshConfig(char *host/*主机*/, int hostlen/*长度*/, unsigned short port/*端口*/, bool bIsDomainName/*是否域名*/);
	//rsa	key :本地保存server端可以解密的加密锁，以及一把专用于客户端的解密锁，服务端保存客户端可以解密的加密锁以及专用于服务端的解密锁

protected:
	//链接远程
	virtual BOOL Connect();
};

//服务器

//服务端 采用异步IO模型，可以扩展到iocp模型 需要设置公私钥
class NETWORK_SERVICE_CLASS CNetworkTcpServer :public CNetworkService
{
public:
	CNetworkTcpServer();
	virtual ~CNetworkTcpServer();

	//启动关闭配置
public:
	//启动
	virtual bool Start();
	//关闭服务器
	virtual BOOL CloseServer();
	//更新配置	(必须先关闭服务器)
	virtual BOOL RefreshConfig(char *host/*主机*/, int hostlen/*长度*/, unsigned short port/*端口*/, bool bIsDomainName/*是否域名*/);

	//监听配置
protected:
	//绑定地址
	virtual BOOL Bind(char *ip, int iplen, unsigned short port, unsigned short family);			//有待商榷
	//监听端口 支持200个链接队列等待 //最多支持250个链接
	virtual BOOL Listen(int iBacklog = LISTEN_MAX_LIMIT);
	//确定链接并获取客户端信息
	BOOL Accept(SOCKET *pSocketFDForClient, CAddr *pRemoteAddr = NULL);
	//配置异步IO模型
	virtual BOOL SetAsynchronousServer();
	//监视IO事件 不完善 有可读事件即移除，与客户端保持短链接 需要一个线程去执行可读事件 监听的套接字有限需要开辟多个线程执行监听任务 建议扩展成IOCP
	virtual BOOL WaitForEvent();
	//添加监控事件
	virtual BOOL AddEvent(SOCKET hClntSock);


	//服务器数据流
public:
	//接收
	virtual bool RecvDataFrom(void **pData, long *lDataLen);
	//发送
	virtual bool SendDataTo(void *pData, long lDataLen);

	//异步辅助函数
protected:
	//调整socket数组
	virtual void CompressSockets(SOCKET hSockArr[], int idx, int total);
	//调整事件数组
	virtual void CompressEvents(WSAEVENT hEventArr[], int idx, int total);

protected:
	bool				m_bServerStart;				//服务器是否启动
	//rsa	key :本地保存server端可以解密的加密锁，以及一把专用于客户端的解密锁，服务端保存客户端可以解密的加密锁以及专用于服务端的解密锁

	//异步IO参数
protected:
	SOCKET				m_hSockArr[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT			m_hEventArr[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT			m_newEvent;
	WSANETWORKEVENTS	m_netEvents;
	int					m_NumOfClntSock;
};

///////////////////////////////////////////////////////////////////////
#define HTTP_REQUEST_SUCCESS_RET_CODE		200		//请求成功返回代码

//http服务
class NETWORK_SERVICE_CLASS CHttpService :public CTcpSocket
{

public:
	//初始化socket环境
	CHttpService(LPTSTR lpszRecvLockerName = NULL, bool bLockAtOnce = false, bool bInitiallyOwn = false, bool bManualReset = true);
	//关闭socket并清理socket环境
	virtual ~CHttpService();


	//服务器链接
public:
	//启动网络服务
	virtual BOOL StartService();
	//链接服务器
	virtual BOOL Link(char *pServerAddr, unsigned short port, bool bClearDataCache = true);
	//断开链接
	virtual BOOL CloseService();

	//内部接口
protected:
	//链接远程
	virtual BOOL Connect();

	//辅助函数
public:
	EFFECT_DATA_FLOW_COUNT_T *GetEffectDataFlowCount(){ return &m_stContentFlowCount; }

	//发送模块
public:
	//接收模块
	//业务逻辑层数据添加 添加之前加密业务层数据
	BOOL AddDataToSend(void *pData, long lDataLen);
	//发送数据包
	BOOL SendDataToDest(BOOL bSocketBlockMode = FALSE);

	//接收模块
public:
	//取出接收到的数据
	BOOL GetOutRecvData(void **pData, long *lDataLen);
	//接收数据包
	BOOL RecvDataFromSrc(long lTvPerSec=1, long lTvPerUsec = 0, int iTimeOutCount = TIME_OUT_COUNT, BOOL bSocketBlockMode = FALSE);
	//释放取出来的数据
	BOOL ReleaseRecvDataOut();

	//内部接口

	//数据包操作
protected:
	//包创建 一个包中的业务层数据由 BUSINESS_DATA_LIMIT限制
	BOOL CreateSocketPacketToSend(long &lPackCount);
	//解包(并解密)将socket待接收缓冲中数据拼接成业务层数据 得到一个业务逻辑层数据
	BOOL CreateBussnessLayerToGet();

	//清理组
public:
	//清空缓冲区数据队列 清除所有区数据
	BOOL ClearAllDataCache();
	//清空待发包缓冲区 3区中的send
	BOOL ClearSocketPackCacheToSend();
	//清空待接收包缓冲区 3区中的recv
	BOOL ClearSocketPackCacheToRecv();
	//socket业务层数据缓冲区待发送清理 1区
	BOOL ClearBusinessLayerToSendCache();
	//socket业务层数据缓冲区待接收清理 2区
	BOOL ClearBusinessLayerToRecvCache();
	//清理http头信息
	BOOL ClearHttpHeader();
	//清理流量统计
	void ResetEffectFlowCount();

	//http状态代码
public:
	long					m_HttpBackStatusCode;			//状态码
	string					m_strHttpReturnHeader;			//返回的http头信息

	//业务层待发送数据缓冲区 1区
protected:
	long					m_lCacheDataSize;				//业务逻辑层数据总长度
	list<CACHE_T *>			m_CacheDataQueue;				//业务层数据缓冲

	//业务层待接收数据缓冲区	2区
protected:
	long					m_lRecvCacheDataSize;				//业务逻辑层数据总长度
	list<CACHE_T *>			m_RecvCacheDataQueue;				//业务层数据缓冲
	void *					m_pRecvDataOut;						//取出来的数据需要释放

	//socket包缓冲 3区
protected:
	map<long, CACHE_T *>		m_SocketPackCacheToSend;		//发送缓冲区	用于发送失败等重发	key值为包序号
	map<long, CACHE_T *>		m_SocketPackCacheToRecv;		//接收缓冲区	用于接收数据包		key值为包序号

	//接收缓冲区守护
protected:
	//CSynLocker	*				m_pRecvCacheLocker;				//接收缓冲区锁

	//数据成员
protected:
	string							m_strServerAddr;			//服务器地址
	unsigned short					m_ServerPort;				//服务器端口
	bool							m_bIsDomainName;			//是否域名
	EFFECT_DATA_FLOW_COUNT_T		m_stContentFlowCount;	//真实有效数据流量统计

};

//http客户端
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

	//下载服务
public:
	//下载文件
	BOOL DownLoadFile(char *szFileLink, long lLinkLen, char *szSaveAsFileName, long lFileNameLen, unsigned short port = 80, bool bDownLoadToMemory = false, long lSec = 1, long lUSec = 0, int iTimeOutCount = TIME_OUT_COUNT, BOOL bSocketBlockMode = FALSE);

	//网络服务
public:
	//链接服务器
	BOOL ConnectServer();
	//关闭链接
	virtual BOOL CloseLink();
	//向服务器发送数据
	BOOL SendDataToServer(void *pData, long lDataLen);
	//从服务器获取数据 记得释放空间
	BOOL RecvDataFromServer(void **pData, long *lDataLen, long lTvPerSec = 1, long lTvPerUsec = 0, int iTimeOutCount = TIME_OUT_COUNT, BOOL bSocketBlockMode = FALSE);
	//更新配置（必须断开链接）
	BOOL RefreshConfig(char *host/*主机*/, int hostlen/*长度*/, unsigned short port/*端口*/, bool bIsDomainName/*是否域名*/);

	//数据缓存
public:
	//清楚数据缓存
	virtual bool ReleaseHttpDataCache();
	//使用内存缓存块
	bool UseDataCache(bool bUse = true);
};

//IPC
#endif