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

// TDXM.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Resource.h"
#include "TDXMApp.h"
#include "TDXMWnd.h"

// CTDXMApp

BEGIN_MESSAGE_MAP(CTDXMApp, CWinApp)
END_MESSAGE_MAP()


// CTDXMApp ����

CTDXMApp::CTDXMApp()
{
}




// CTDXMApp ��ʼ��

BOOL CTDXMApp::InitInstance()
{
	CWinApp::InitInstance();

	//��������
	AfxOleInit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//���ñ���
	SetRegistryKey(TEXT("TDXM"));

	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH] = TEXT("");
	GetWorkDirectory(szDirectory, CountArray(szDirectory));

	//����Ŀ¼
	SetCurrentDirectory(szDirectory);

	//��������
	WNDCLASS WndClasss;
	ZeroMemory(&WndClasss, sizeof(WndClasss));

	//ע�ᴰ��
	WndClasss.style = CS_DBLCLKS;
	WndClasss.hIcon = LoadIcon(IDR_MAINFRAME);
	WndClasss.lpfnWndProc = DefWindowProc;
	WndClasss.lpszClassName = TEXT("�������");
	WndClasss.hInstance = AfxGetInstanceHandle();
	WndClasss.hCursor = LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW));
	if (AfxRegisterClass(&WndClasss) == FALSE) AfxThrowResourceException();

	//������ԴĿ¼
	HINSTANCE hInstance = AfxGetInstanceHandle();
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin\\"));

	//��ʼ�����绷��
	 CTcpClient::InitSocketEnvironment();

	//��������
	CTDXMFrame * pTDXMFrame = new CTDXMFrame;
	pTDXMFrame->Create(TEXT("�������"), TEXT("�������"), WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_SYSMENU, CRect(0, 0, 0, 0));



	//���ñ���
	m_pMainWnd = pTDXMFrame;

	return TRUE;
	
}

int CTDXMApp::ExitInstance()
{
	//�������绷��
	 CTcpClient::CleanSocketEnvironment();

	return __super::ExitInstance();
}

//����Ŀ¼
bool CTDXMApp::GetWorkDirectory(TCHAR szWorkDirectory[], WORD wBufferCount)
{
	//ģ��·��
	TCHAR szModulePath[MAX_PATH] = TEXT("");
	GetModuleFileName(AfxGetInstanceHandle(), szModulePath, CountArray(szModulePath));

	//�����ļ�
	for (INT i = lstrlen(szModulePath); i >= 0; i--)
	{
		if (szModulePath[i] == TEXT('\\'))
		{
			szModulePath[i] = 0;
			break;
		}
	}

	//���ý��
	ASSERT(szModulePath[0] != 0);
	lstrcpyn(szWorkDirectory, szModulePath, wBufferCount);

	return true;
}

// CTDXMApp ��Ϣ�������

// Ψһ��һ�� CTDXMApp ����
CTDXMApp theApp;



