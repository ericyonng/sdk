// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// TDXM.h : TDXM 应用程序的主头文件
//

#ifndef TDXM_APP_H_
#define TDXM_APP_H_


#pragma once

#include "../结构定义/Public.h"

#include "resource.h"       // 主符号


// CTDXMApp:
// 有关此类的实现，请参阅 TDXM.cpp
//

class CTDXMApp : public CWinApp
{
public:
	CTDXMApp();


	// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// 实现

public:
	//进程目录
	bool GetWorkDirectory(TCHAR szWorkDirectory[], WORD wBufferCount);
	DECLARE_MESSAGE_MAP()
};

extern CTDXMApp theApp;

#endif

