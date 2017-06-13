#include"stdafx.h"
#include"SdkToolsApp.h"

CSdkToolsApp theApp;
BEGIN_MESSAGE_MAP(CSdkToolsApp, CWinApp)

END_MESSAGE_MAP()
BOOL CSdkToolsApp::InitInstance()
{
	CWinApp::InitInstance();

	//��������
	AfxOleInit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH] = TEXT("");
	GetWorkDirectory(szDirectory, CountArray(szDirectory));

	//���ñ���
	SetRegistryKey(TEXT("EricYonngSDKTools"));

	//����Ŀ¼
	SetCurrentDirectory(szDirectory);

	//��������
	WNDCLASS WndClasss;
	ZeroMemory(&WndClasss, sizeof(WndClasss));

	//ע�ᴰ��
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

//����Ŀ¼
bool CSdkToolsApp::GetWorkDirectory(TCHAR szWorkDirectory[], WORD wBufferCount)
{
	memset(szWorkDirectory, 0, wBufferCount*sizeof(TCHAR));

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
