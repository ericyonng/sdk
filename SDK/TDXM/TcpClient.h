#ifndef TCP_CLIENT_H_
#define TCP_CLIENT_H_

#pragma once

//#include"TcpSocket.h"
#include"TdxmHead.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//客户端
class CTcpClient :public CNetworkService
{
public:
	//初始化运行环境
	CTcpClient();
	virtual ~CTcpClient();

	//网络服务
public:
	//链接服务器
	BOOL Link(char *pServerAddr, unsigned short port, bool bClearDataCache = true);
	//链接服务器
	BOOL ConnectServer();
	//关闭链接
	BOOL CloseLink();
	//向服务器发送数据
	BOOL SendDataToServer(void *pData, long lDataLen);
	//从服务器获取数据 记得释放空间
	BOOL RecvDataFromServer(void **pData, long *lDataLen);
	//更新配置（必须断开链接）
	BOOL RefreshConfig();

	//配置
protected:
	//读取链接地址
	BOOL	ReadServerAddr(){ return TRUE; }
	//读取加解密密钥
	BOOL	ReadRsaKey(){ return TRUE; }
	//链接远程
	virtual BOOL Connect();

	//数据成员
protected:	
	string				m_strServerAddr;			//服务器地址
	unsigned short		m_ServerPort;				//服务器端口
	bool				m_bIsDomainName;			//是否域名
	//rsa	key :本地保存server端可以解密的加密锁，以及一把专用于客户端的解密锁，服务端保存客户端可以解密的加密锁以及专用于服务端的解密锁
};


#endif