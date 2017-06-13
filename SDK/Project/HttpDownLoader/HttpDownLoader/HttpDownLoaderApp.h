
#ifndef HTTP_DOWNLOADER_APP_H_
#define HTTP_DOWNLOADER_APP_H_


#pragma once

#include "Public.h"


// CTDXMApp:
// 有关此类的实现，请参阅 TDXM.cpp
//

class CHttpDownLoader : public CWinApp
{
public:
	CHttpDownLoader();


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

extern CHttpDownLoader theApp;

#endif