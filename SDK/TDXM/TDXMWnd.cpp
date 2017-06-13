

#include"stdafx.h"
#include"Resource.h"

#include "TDXMWnd.h"
#include<io.h>



//////////////////////////////////////////////////////////////////////////////////
//��̬����

CTDXMFrame * CTDXMFrame::m_pTdxmFrame = NULL;					//���ָ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTDXMFrame, CYCDuiFrameWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////


//���캯��
CTDXMFrame::CTDXMFrame()
{

	//ƽ̨����
	ASSERT(m_pTdxmFrame == NULL);
	if (m_pTdxmFrame == NULL) m_pTdxmFrame = this;

	m_pDlgLogIn = NULL;
	m_pDlgReg = NULL;
	m_pModifyPwd = NULL;

	//��ȡ�����ļ�
	ReadConfig();

	return;
}

//��������
CTDXMFrame::~CTDXMFrame()
{
	//ƽ̨����
	ASSERT(m_pTdxmFrame == this);
	if (m_pTdxmFrame == this) m_pTdxmFrame = NULL;

	if (m_pDlgLogIn)
	{ 
		if (m_pDlgLogIn->m_hWnd)
		{
			m_pDlgLogIn->DestroyWindow();
		}

		if (m_pDlgLogIn)
		{
			delete m_pDlgLogIn;
			m_pDlgLogIn = NULL;
		}
	}

	return;
}

//��Ϣ����
BOOL CTDXMFrame::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN) return TRUE;
	}


	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CTDXMFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID = LOWORD(wParam);

	//�˵�����
	switch (nCommandID)
	{
		case IDM_LOG_IN:		//�û���¼
		{
									if (m_pDlgReg)
									{
										if (m_pDlgReg->m_hWnd)
										{
											m_pDlgReg->ShowWindow(SW_HIDE);
										}
									}

									if (m_pModifyPwd)
									{
										if (m_pModifyPwd->m_hWnd)
										{
											m_pModifyPwd->ShowWindow(SW_HIDE);
										}
									}

									if (m_pDlgLogIn == NULL)
									{
										m_pDlgLogIn = new CDlgLogIn;
									}

									if (m_pDlgLogIn->m_hWnd == NULL)
									{
										m_pDlgLogIn->Create(IDD_DLG_LOGIN, GetDesktopWindow());
									}
									
									m_pDlgLogIn->ShowWindow(SW_SHOW);
									m_pDlgLogIn->SetForegroundWindow();
									m_pDlgLogIn->BringWindowToTop();
									m_pDlgLogIn->UpdateWindow();
			return TRUE;
		}
		case IDM_REGISTER:
		{
							 if (m_pDlgLogIn)
							 {
								 if (m_pDlgLogIn->m_hWnd)
								 {
									 m_pDlgLogIn->ShowWindow(SW_HIDE);
								 }
							 }

							 if (m_pModifyPwd)
							 {
								 if (m_pModifyPwd->m_hWnd)
								 {
									 m_pModifyPwd->ShowWindow(SW_HIDE);
								 }
							 }

							 
							 if (m_pDlgReg == NULL)
							 {
								 m_pDlgReg = new CDlgRegAccount;
							 }

							 if (m_pDlgReg->m_hWnd == NULL)
							 {
								 m_pDlgReg->Create(IDD_DLG_LOGIN, GetDesktopWindow());
							 }

							 m_pDlgReg->ShowWindow(SW_SHOW);
							 m_pDlgReg->SetForegroundWindow();
							 m_pDlgReg->BringWindowToTop();
							 m_pDlgReg->UpdateWindow();
							 return TRUE;
		}
		case IDM_MODIFY_PWD:
		{
							   if (m_pDlgLogIn)
							   {
								   if (m_pDlgLogIn->m_hWnd)
								   {
									   m_pDlgLogIn->ShowWindow(SW_HIDE);
								   }
							   }

							   if (m_pDlgReg)
							   {
								   if (m_pDlgReg->m_hWnd)
								   {
									   m_pDlgReg->ShowWindow(SW_HIDE);
								   }
							   }


							   if (m_pModifyPwd == NULL)
							   {
								   m_pModifyPwd = new CDlgModifyAccount;
							   }

							   if (m_pModifyPwd->m_hWnd == NULL)
							   {
								   m_pModifyPwd->Create(IDD_DLG_LOGIN, GetDesktopWindow());
							   }

							   m_pModifyPwd->ShowWindow(SW_SHOW);
							   m_pModifyPwd->SetForegroundWindow();
							   m_pModifyPwd->BringWindowToTop();
							   m_pModifyPwd->UpdateWindow();
							   return TRUE;
		}
		case IDM_CLOSE:		//�ر�
		{
								//Ͷ����Ϣ
								PostMessage(WM_CLOSE, 0, 0);

								return TRUE;
		}
		case IDM_MINIMIZE:  //��С��
		{
								if (IsWindowVisible())
								{
									ShowWindow(SW_MINIMIZE);
								}
								return TRUE;
		}
	}

	return __super::OnCommand(wParam, lParam);
}

//��ʼ�滭
void CTDXMFrame::OnBeginPaintWindow(HDC hDC)
{
	
	return;
}

//�����滭
void CTDXMFrame::OnEndPaintWindow(HDC hDC)
{


	return;
}

//��Ϣ����
void CTDXMFrame::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
	}
}


//�����ؼ�
VOID CTDXMFrame::RectifyControl(INT nWidth, INT nHeight)
{
	//״̬�ж�
	if ((nWidth == 0) || (nHeight == 0)) return;

	//�ƶ�׼��
	HDWP hDwp = BeginDeferWindowPos(32);
	UINT uFlags = SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOZORDER;

	//DeferWindowPos(hDwp, , NULL, 0, , , , uFlags);

	//��������
	LockWindowUpdate();
	EndDeferWindowPos(hDwp);
	UnlockWindowUpdate();

	//���½���	
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_INVALIDATE);

	return;
}



//�ر���Ϣ
VOID CTDXMFrame::OnClose()
{

	__super::OnClose();
}

//ʱ����Ϣ
VOID CTDXMFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	}

	__super::OnTimer(nIDEvent);
}

//λ����Ϣ
VOID CTDXMFrame::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx, cy);

	return;
}

//λ����Ϣ
VOID CTDXMFrame::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
	__super::OnGetMinMaxInfo(lpMMI);

	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcArce, SPIF_SENDCHANGE);

	//����λ��
	lpMMI->ptMinTrackSize.x = __min(357, rcArce.Width());
	lpMMI->ptMinTrackSize.y = __min(220, rcArce.Height());

	return;
}

//������Ϣ
INT CTDXMFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1) return -1;

	//���ô���
	ModifyStyle(WS_CAPTION | WS_BORDER, WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE, 0, 0);

	//��������
	CRect rcCreate(0, 0, 0, 0);


	//����λ��
	CSize SizeRestrict;
	SizeRestrict.SetSize(357, 220);

	//�ƶ�����
	SetWindowPos(NULL, 0, 0, SizeRestrict.cx, SizeRestrict.cy, SWP_NOMOVE|SWP_NOZORDER);
	CenterWindow();

	CRgn rgnWind;
	CRect rcClient;
	GetWindowRect(&rcClient);
	rgnWind.CreateRoundRectRgn(4, 4, SizeRestrict.cx - 4 + 1, SizeRestrict.cy - 4 + 1, 4, 4);

	//��ʾ����
	SetWindowRgn(rgnWind, TRUE);

	//��¼ϵͳ
	PostMessage(WM_COMMAND, IDM_LOG_IN, 0);

	return 0;
}

//��ȡ����
BOOL CTDXMFrame::ReadConfig()
{
	if (_access("ClientConfig.ini", 0) == -1)
	{
		AfxMessageBox(TEXT("ȱ�������ļ�ClientConfig.ini"));
		AfxGetMainWnd()->PostMessage(WM_CLOSE);
		return TRUE;
	}

	CIniFileIO ClientIni;
	ClientIni.SetFilePath(TEXT(".\\ClientConfig.ini"));

	//��ȡ������ip �˿�
	TCHAR szIP[8192] = { 0 };
	TCHAR szPort[8192] = { 0 };
	char strIp[2048] = { 0 };
	char strPort[2048] = { 0 };
	char strPlain[2048] = { 0 };
	char *pPlain = strPlain;
	unsigned long ulPlainSize = 2048;
	ClientIni.ReadString(TEXT("Server"), TEXT("ip"), TEXT(""), szIP, 8192);
	ClientIni.ReadString(TEXT("Server"), TEXT("port"), TEXT(""), szPort, 8192);
	
	if (lstrlen(szIP) == 0 || lstrlen(szPort) == 0)
	{
		AfxMessageBox(TEXT("ClientConfig.ini�����ļ��𻵣�"));
		AfxGetMainWnd()->PostMessage(WM_CLOSE);
		return TRUE;
	}

	CWriteDailyLog::UnicodeToAnsi(szIP, strIp, 2048);
	CWriteDailyLog::UnicodeToAnsi(szPort, strPort, 2048);

	//���ܶ�ȡ������
	CRsaEngine RsaSystem;
	RsaSystem.SetPrivateKey((unsigned char *)szClientPrivateKey);
	RsaSystem.Decrypte((unsigned char*)strIp, strlen(strIp), (unsigned char **)&pPlain, &ulPlainSize);
	m_strServerAddr = (char *)strPlain;

	//port
	memset(strPlain, 0, 2048);
	ulPlainSize = 2048;
	pPlain = strPlain;
	RsaSystem.Decrypte((unsigned char*)strPort, strlen(strPort), (unsigned char **)&pPlain, &ulPlainSize);
	m_ServerPort = (unsigned short)atoi(strPlain);

	//char rizhi[1024] = { 0 };
	//sprintf(rizhi, "ip:%s; port:%hd", m_strServerAddr.c_str(), m_ServerPort);
	//CWriteDailyLog::WriteLog(rizhi, "rizhi.txt");

	return TRUE;
}