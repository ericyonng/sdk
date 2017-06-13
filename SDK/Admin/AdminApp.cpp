#include"stdafx.h"
#include"AdminApp.h"

#include"AdminWnd.h"

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

// CTDXMApp

BEGIN_MESSAGE_MAP(CAdmin, CWinApp)
END_MESSAGE_MAP()


// CTDXMApp ����

CAdmin::CAdmin()
{
}




// CTDXMApp ��ʼ��

BOOL CAdmin::InitInstance()
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
	WndClasss.hIcon = LoadIcon(IDI_ICON1);
	WndClasss.lpfnWndProc = DefWindowProc;
	WndClasss.lpszClassName = TEXT("Admin");
	WndClasss.hInstance = AfxGetInstanceHandle();
	WndClasss.hCursor = LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW));
	if (AfxRegisterClass(&WndClasss) == FALSE) AfxThrowResourceException();

	CAdminDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();


	return TRUE;

}

int CAdmin::ExitInstance()
{
	return __super::ExitInstance();
}

//����Ŀ¼
bool CAdmin::GetWorkDirectory(TCHAR szWorkDirectory[], WORD wBufferCount)
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
CAdmin theApp;



