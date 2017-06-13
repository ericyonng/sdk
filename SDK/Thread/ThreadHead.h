#ifndef THREAD_HEAD_H_
#define THREAD_HEAD_H_f

#pragma once

//导出定义
#ifndef THREAD_CLASS
#ifdef  THREAD_DLL
#define THREAD_CLASS _declspec(dllexport)
#else
#define THREAD_CLASS _declspec(dllimport)
#endif
#endif

//模块定义
#ifndef _DEBUG
#define THREAD_DLL_NAME				TEXT("Thread.dll")			//组件名字
#else
#define THREAD_DLL_NAME				TEXT("ThreadD.dll")		//组件名字
#endif

//////////////////////////////////////////////////////////////////////////////////

//导出文件
#ifndef THREAD_DLL
//#include"TaskBase.h"
#include"ThreadPool.h"
#endif
//////////////////////////////////////////////////////////////////////////////////
#endif