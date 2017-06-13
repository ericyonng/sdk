#include"stdafx.h"
#include"HttpDownLoaderApp.h"
//#include"resource.h"
#include"HttpDownLoaderWnd.h"



BEGIN_MESSAGE_MAP(CHttpDownLoader, CWinApp)
END_MESSAGE_MAP()


// CTDXMApp ����

CHttpDownLoader::CHttpDownLoader()
{
}




// CTDXMApp ��ʼ��

BOOL CHttpDownLoader::InitInstance()
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

//����Ŀ¼
bool CHttpDownLoader::GetWorkDirectory(TCHAR szWorkDirectory[], WORD wBufferCount)
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
CHttpDownLoader theApp;



