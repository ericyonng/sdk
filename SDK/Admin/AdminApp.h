
#ifndef ADMIN_APP_H_
#define ADMIN_APP_H_


#pragma once

#include "../结构定义/Public.h"

#include "resource.h"       // 主符号


// CTDXMApp:
// 有关此类的实现，请参阅 TDXM.cpp
//

class CAdmin : public CWinApp
{
public:
	CAdmin();


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

extern CAdmin theApp;

#endif