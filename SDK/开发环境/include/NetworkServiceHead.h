#ifndef NETWORK_SERVICE_HEAD_H_
#define NETWORK_SERVICE_HEAD_H_

#pragma once
//////////////////////////////////////////////////////////////////////////////////

//导出定义
#ifndef NETWORK_SERVICE_CLASS
#ifdef  NETWORK_SERVICE_DLL
#define NETWORK_SERVICE_CLASS _declspec(dllexport)
#else
#define NETWORK_SERVICE_CLASS _declspec(dllimport)
#endif
#endif

//模块定义
#ifndef _DEBUG
#define NETWORK_SERVICE_DLL_NAME	TEXT("NetworkService.dll")			//组件名字
#else
#define NETWORK_SERVICE_DLL_NAME	TEXT("NetworkServiceD.dll")			//组件名字
#endif

//////////////////////////////////////////////////////////////////////////////////

//导出文件
#ifndef NETWORK_SERVICE_DLL
#include"SocketBase.h"
#include"SocketProtocol.h"
#include"TcpSocket.h"
#endif
//////////////////////////////////////////////////////////////////////////////////

#endif
