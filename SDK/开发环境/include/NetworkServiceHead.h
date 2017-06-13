#ifndef NETWORK_SERVICE_HEAD_H_
#define NETWORK_SERVICE_HEAD_H_

#pragma once
//////////////////////////////////////////////////////////////////////////////////

//��������
#ifndef NETWORK_SERVICE_CLASS
#ifdef  NETWORK_SERVICE_DLL
#define NETWORK_SERVICE_CLASS _declspec(dllexport)
#else
#define NETWORK_SERVICE_CLASS _declspec(dllimport)
#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
#define NETWORK_SERVICE_DLL_NAME	TEXT("NetworkService.dll")			//�������
#else
#define NETWORK_SERVICE_DLL_NAME	TEXT("NetworkServiceD.dll")			//�������
#endif

//////////////////////////////////////////////////////////////////////////////////

//�����ļ�
#ifndef NETWORK_SERVICE_DLL
#include"SocketBase.h"
#include"SocketProtocol.h"
#include"TcpSocket.h"
#endif
//////////////////////////////////////////////////////////////////////////////////

#endif
