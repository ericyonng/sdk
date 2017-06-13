#include"stdafx.h"
#include"HttpDownLoaderApp.h"
//#include"resource.h"
#include"HttpDownLoaderWnd.h"



BEGIN_MESSAGE_MAP(CHttpDownLoader, CWinApp)
END_MESSAGE_MAP()


// CTDXMApp 构造

CHttpDownLoader::CHttpDownLoader()
{
}




// CTDXMApp 初始化

BOOL CHttpDownLoader::InitInstance()
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
	WndClasss.lpszClassName = TEXT("HttpDownLoader");
	WndClasss.hInstance = AfxGetInstanceHandle();
	WndClasss.hCursor = LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW));
	if (AfxRegisterClass(&WndClasss) == FALSE) AfxThrowResourceException();

	CHttpDownLoaderDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();


	return TRUE;

}

int CHttpDownLoader::ExitInstance()
{
	return __super::ExitInstance();
}

//进程目录
bool CHttpDownLoader::GetWorkDirectory(TCHAR szWorkDirectory[], WORD wBufferCount)
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
CHttpDownLoader theApp;



