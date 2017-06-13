#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_

#pragma once

//#include"../NetworkService/NetworkServiceHead.h"
#include"../NetworkService/TcpSocket.h"


//服务端 采用异步IO模型，可以扩展到iocp模型
class CTcpServer :public CNetworkService
{
public:
	CTcpServer();
	virtual ~CTcpServer();

public:
	//启动服务器
	BOOL StartServer();
	//关闭服务器
	BOOL CloseServer();
	//更新配置	(必须先关闭服务器)
	BOOL RefreshConfig();
	//设置主窗体
	void SetMainWind(void *pMainWnd){ m_pMainWind = pMainWnd; }
	//设置ip
	void SetIp(string ip);
	//设置端口
	void SetPort(unsigned short port){ m_ServerPort = port; }

public:
	//监听端口 支持200个链接队列等待 //最多支持250个链接
	BOOL Listen(int iBacklog=200);
	//确定链接并获取客户端信息
	BOOL Accept(SOCKET *pSocketFDForClient, CAddr *pRemoteAddr=NULL);
	//配置异步IO模型
	BOOL SetAsynchronousServer();
	//监视IO事件
	BOOL WaitForEvent();
	//添加监控事件
	BOOL AddEvent(SOCKET hClntSock);
	//绑定地址
	BOOL Bind(char *ip, int iplen, unsigned short port, unsigned short family);			//有待商榷

	//获取数据
	BOOL ReadDataFromClient(void **pData, long *lDataLen);
	//发送数据
	BOOL SendDataToClient(void *pData, long lDataLen);

	//辅助函数
protected:
	//调整socket数组
	void CompressSockets(SOCKET hSockArr[], int idx, int total);
	//调整事件数组
	void CompressEvents(WSAEVENT hEventArr[], int idx, int total);

	//导入配置
protected:
	BOOL	ImportAddrConfig(){ return TRUE; }
	BOOL	ReadRsaKey(){ return TRUE; }

	

protected:
	void	*			m_pMainWind;				//主窗体

protected:
	//地址配置
	string				m_strServerAddr;			//服务器地址
	unsigned short		m_ServerPort;				//服务器端口
	bool				m_bIsDomainName;			//是否域名
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
#endif