#ifndef SDK_TOOLS_APP_H_
#define SDK_TOOLS_APP_H_

#pragma once
//#include "resource.h"       // 主符号
#include"DlgSdkTools.h"
class CSdkToolsApp :public CWinApp
{
protected:
	CDlgSdkTools		m_DlgSdkTools;
public:
	CSdkToolsApp(){}

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

extern CSdkToolsApp theApp;
#endif