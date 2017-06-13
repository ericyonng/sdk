#ifndef SOCKET_BASE_H_
#define SOCKET_BASE_H_
#pragma once

#include"NetworkServiceHead.h"

#include<Winsock2.h>
#include<ws2tcpip.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

interface NETWORK_SERVICE_CLASS ISocketEngine
{
public:
	virtual void SetDestAddr(void *pAddr) = NULL;
	virtual bool Start() = NULL;
	virtual bool Close() = NULL;
	virtual bool SendDataTo(void *pData, long lDataLen) = NULL;
	virtual bool RecvDataFrom(void **pData, long *plDataLen) = NULL;
	virtual void Release(){ delete this; }

protected:
	//∞Û∂®µÿ÷∑
	//virtual BOOL Bind(char *ip, int iplen, unsigned short port, unsigned short family) = NULL;
	//virtual BOOL Connect() = NULL;
};

#endif