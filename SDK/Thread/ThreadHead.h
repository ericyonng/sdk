#ifndef THREAD_HEAD_H_
#define THREAD_HEAD_H_f

#pragma once

//��������
#ifndef THREAD_CLASS
#ifdef  THREAD_DLL
#define THREAD_CLASS _declspec(dllexport)
#else
#define THREAD_CLASS _declspec(dllimport)
#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
#define THREAD_DLL_NAME				TEXT("Thread.dll")			//�������
#else
#define THREAD_DLL_NAME				TEXT("ThreadD.dll")		//�������
#endif

//////////////////////////////////////////////////////////////////////////////////

//�����ļ�
#ifndef THREAD_DLL
//#include"TaskBase.h"
#include"ThreadPool.h"
#endif
//////////////////////////////////////////////////////////////////////////////////
#endif