
#ifndef HTTP_DOWNLOADER_APP_H_
#define HTTP_DOWNLOADER_APP_H_


#pragma once

#include "Public.h"


// CTDXMApp:
// �йش����ʵ�֣������ TDXM.cpp
//

class CHttpDownLoader : public CWinApp
{
public:
	CHttpDownLoader();


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

extern CHttpDownLoader theApp;

#endif