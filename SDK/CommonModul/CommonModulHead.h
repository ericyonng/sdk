#ifndef COMMON_MODUL_HEAD_H_
#define COMMON_MODUL_HEAD_H_

#pragma once


//��������
#ifndef COMMON_MODUL_CLASS
#ifdef  COMMON_MODUL_DLL
#define COMMON_MODUL_CLASS _declspec(dllexport)
#else
#define COMMON_MODUL_CLASS _declspec(dllimport)
#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
#define COMMON_MODUL_DLL_NAME				TEXT("CommonModul.dll")			//�������
#else
#define COMMON_MODUL_DLL_NAME				TEXT("CommonModulD.dll")		//�������
#endif

//////////////////////////////////////////////////////////////////////////////////

//�����ļ�
#ifndef COMMON_MODUL_DLL
//#include"TaskBase.h"
#include"IniFileIO.h"
#include"Rsa.h"
#include"DailyLog.h"
#endif
#endif