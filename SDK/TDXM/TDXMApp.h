// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// TDXM.h : TDXM Ӧ�ó������ͷ�ļ�
//

#ifndef TDXM_APP_H_
#define TDXM_APP_H_


#pragma once

#include "../�ṹ����/Public.h"

#include "resource.h"       // ������


// CTDXMApp:
// �йش����ʵ�֣������ TDXM.cpp
//

class CTDXMApp : public CWinApp
{
public:
	CTDXMApp();


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

extern CTDXMApp theApp;

#endif

