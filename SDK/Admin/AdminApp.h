
#ifndef ADMIN_APP_H_
#define ADMIN_APP_H_


#pragma once

#include "../�ṹ����/Public.h"

#include "resource.h"       // ������


// CTDXMApp:
// �йش����ʵ�֣������ TDXM.cpp
//

class CAdmin : public CWinApp
{
public:
	CAdmin();


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

extern CAdmin theApp;

#endif