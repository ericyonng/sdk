#ifndef COMMON_MODUL_HEAD_H_
#define COMMON_MODUL_HEAD_H_

#pragma once


//导出定义
#ifndef COMMON_MODUL_CLASS
#ifdef  COMMON_MODUL_DLL
#define COMMON_MODUL_CLASS _declspec(dllexport)
#else
#define COMMON_MODUL_CLASS _declspec(dllimport)
#endif
#endif

//模块定义
#ifndef _DEBUG
#define COMMON_MODUL_DLL_NAME				TEXT("CommonModul.dll")			//组件名字
#else
#define COMMON_MODUL_DLL_NAME				TEXT("CommonModulD.dll")		//组件名字
#endif

//////////////////////////////////////////////////////////////////////////////////

//导出文件
#ifndef COMMON_MODUL_DLL
//#include"TaskBase.h"
#include"IniFileIO.h"
#include"Rsa.h"
#include"DailyLog.h"
#endif
#endif