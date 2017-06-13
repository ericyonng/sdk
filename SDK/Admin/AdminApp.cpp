#include"stdafx.h"
#include"AdminApp.h"

#include"AdminWnd.h"

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

// TDXM.cpp : 定义应用程序的类行为。
//

// CTDXMApp

BEGIN_MESSAGE_MAP(CAdmin, CWinApp)
END_MESSAGE_MAP()


// CTDXMApp 构造

CAdmin::CAdmin()
{
}




// CTDXMApp 初始化

BOOL CAdmin::InitInstance()
{
	CWinApp::InitInstance();

	//环境配置
	AfxOleInit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//设置表名
	SetRegistryKey(TEXT("TDXM"));

	//工作目录
	TCHAR szDirectory[MAX_PATH] = TEXT("");
	GetWorkDirectory(szDirectory, CountArray(szDirectory));

	//设置目录
	SetCurrentDirectory(szDirectory);

	//变量定义
	WNDCLASS WndClasss;
	ZeroMemory(&WndClasss, sizeof(WndClasss));

	//注册窗口
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

//进程目录
bool CAdmin::GetWorkDirectory(TCHAR szWorkDirectory[], WORD wBufferCount)
{
	//模块路径
	TCHAR szModulePath[MAX_PATH] = TEXT("");
	GetModuleFileName(AfxGetInstanceHandle(), szModulePath, CountArray(szModulePath));

	//分析文件
	for (INT i = lstrlen(szModulePath); i >= 0; i--)
	{
		if (szModulePath[i] == TEXT('\\'))
		{
			szModulePath[i] = 0;
			break;
		}
	}

	//设置结果
	ASSERT(szModulePath[0] != 0);
	lstrcpyn(szWorkDirectory, szModulePath, wBufferCount);

	return true;
}

// CTDXMApp 消息处理程序

// 唯一的一个 CTDXMApp 对象
CAdmin theApp;



