#include"stdafx.h"
#include"SdkToolsApp.h"

CSdkToolsApp theApp;
BEGIN_MESSAGE_MAP(CSdkToolsApp, CWinApp)

END_MESSAGE_MAP()
BOOL CSdkToolsApp::InitInstance()
{
	CWinApp::InitInstance();

	//环境配置
	AfxOleInit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//工作目录
	TCHAR szDirectory[MAX_PATH] = TEXT("");
	GetWorkDirectory(szDirectory, CountArray(szDirectory));

	//设置表名
	SetRegistryKey(TEXT("EricYonngSDKTools"));

	//设置目录
	SetCurrentDirectory(szDirectory);

	//变量定义
	WNDCLASS WndClasss;
	ZeroMemory(&WndClasss, sizeof(WndClasss));

	//注册窗口
	WndClasss.style = CS_DBLCLKS;
	WndClasss.hIcon = LoadIcon(IDI_ICON1);
	WndClasss.lpfnWndProc = DefWindowProc;
	WndClasss.lpszClassName = TEXT("EricYonngSDKTools");
	WndClasss.hInstance = AfxGetInstanceHandle();
	WndClasss.hCursor = LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW));
	if (AfxRegisterClass(&WndClasss) == FALSE) AfxThrowResourceException();
	m_pMainWnd = &m_DlgSdkTools;

	
	int nResponse = m_DlgSdkTools.DoModal();
	

	return TRUE;
}

int CSdkToolsApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}

//进程目录
bool CSdkToolsApp::GetWorkDirectory(TCHAR szWorkDirectory[], WORD wBufferCount)
{
	memset(szWorkDirectory, 0, wBufferCount*sizeof(TCHAR));

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
