#ifndef SDK_TOOLS_APP_H_
#define SDK_TOOLS_APP_H_

#pragma once
//#include "resource.h"       // ������
#include"DlgSdkTools.h"
class CSdkToolsApp :public CWinApp
{
protected:
	CDlgSdkTools		m_DlgSdkTools;
public:
	CSdkToolsApp(){}

	// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// ʵ��

public:
	//����Ŀ¼
	bool GetWorkDirectory(TCHAR szWorkDirectory[], WORD wBufferCount);
	DECLARE_MESSAGE_MAP()
};

extern CSdkToolsApp theApp;
#endif