// listenActiveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "listenActive.h"
#include "listenActiveDlg.h"
//#include"../NetworkService/NetworkServiceHead.h"
//#include"ThreadPool.h"
#include"Task.h"
#

#include<string>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
volatile BOOL runFlag;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CRegTimeSet, CDialog)
	
	ON_EN_CHANGE(IDC_START_HOUR, &CRegTimeSet::OnEnChangeStartHour)
END_MESSAGE_MAP()
CRegTimeSet::CRegTimeSet(CWnd* pParent)
: CDialog(CRegTimeSet::IDD, pParent)
{
	m_nStartHour = -1;
	m_nStartMin = -1;
	m_nStartSec = -1;
	m_nEndHour = -1;
	m_nEndMin = -1;
	m_nEndSec = -1;
	m_pListenDlg = NULL;
}

BOOL CRegTimeSet::OnInitDialog()
{
	BOOL bRet = CDialog::OnInitDialog();
	SYSTEMTIME sysTime;
	memset(&sysTime, 0, sizeof(sysTime));
	GetLocalTime(&sysTime);
	//CString strTime;
	if (m_nStartHour == -1)
	{
		m_nStartHour = sysTime.wHour;
	}
	
	//strTime.Format(TEXT("%d"), m_nStartHour);
	//m_EditStartHour.SetWindowText(strTime);
	//m_EditStartHour.SetLimitText(4);
	//strTime.Empty();

	if (m_nStartMin == -1)
	{
		m_nStartMin = sysTime.wMinute;
	}

	//strTime.Format(TEXT("%d"), m_nStartMin);
	//m_EditStartMin.SetWindowText(strTime);
	//m_EditStartMin.SetLimitText(4);
	//strTime.Empty();

	if (m_nStartSec == -1)
	{
		m_nStartSec = sysTime.wSecond;
	}

	//strTime.Format(TEXT("%d"), m_nStartSec);
	//m_EditStartSec.SetWindowText(strTime);
	//m_EditStartSec.SetLimitText(4);

	//����ʱ��
	if (m_nEndHour == -1)
	{
		m_nEndHour = sysTime.wHour;
	}

	//strTime.Format(TEXT("%d"), m_nEndHour);
//	m_EditEndHour.SetWindowText(strTime);
	//m_EditEndHour.SetLimitText(4);
	//strTime.Empty();

	if (m_nEndMin == -1)
	{
		m_nEndMin = sysTime.wMinute;
	}

	//strTime.Format(TEXT("%d"), m_nEndMin);
	//m_EditEndMin.SetWindowText(strTime);
	//m_EditEndMin.SetLimitText(4);
//	strTime.Empty();

	if (m_nEndSec == -1)
	{
		m_nEndSec = sysTime.wSecond;
	}

	//strTime.Format(TEXT("%d"), m_nEndSec);
	//m_EditEndSec.SetWindowText(strTime);
	//m_EditEndSec.SetLimitText(4);

	return bRet;
}

void CRegTimeSet::OnOK()
{
	//��ʽ���
	BOOL bFormatWrong = FALSE;
	TCHAR szTime[512] = { 0 };
	char szTimeAnsi[1024] = { 0 };
	INT nTime = -1, nStartHour = -1, nStartMin=-1, nStartSec=-1, nEndHour=-1, nEndMin=-1, nEndSec=-1;

	//��ʼʱ��
	{
		//ʱ
		memset(szTime, 0, sizeof(szTime));
		memset(szTimeAnsi, 0, sizeof(szTimeAnsi));
		nTime = -1;
		m_EditStartHour.GetWindowText(szTime, sizeof(szTime));
		CWriteDailyLog::UnicodeToAnsi(szTime, szTimeAnsi, sizeof(szTimeAnsi));
		nTime = atoi(szTimeAnsi);
		if (nTime > 24 || nTime < 0)
		{
			bFormatWrong = TRUE;
		}
		else
		{
			nStartHour = nTime;
		}

		//��
		memset(szTime, 0, sizeof(szTime));
		memset(szTimeAnsi, 0, sizeof(szTimeAnsi));
		nTime = -1;
		m_EditStartMin.GetWindowText(szTime, sizeof(szTime));
		CWriteDailyLog::UnicodeToAnsi(szTime, szTimeAnsi, sizeof(szTimeAnsi));
		nTime = atoi(szTimeAnsi);
		if (nTime > 59 || nTime < 0)
		{
			bFormatWrong = TRUE;
		}
		else
		{
			nStartMin = nTime;
		}

		//��
		memset(szTime, 0, sizeof(szTime));
		memset(szTimeAnsi, 0, sizeof(szTimeAnsi));
		nTime = -1;
		m_EditStartSec.GetWindowText(szTime, sizeof(szTime));
		CWriteDailyLog::UnicodeToAnsi(szTime, szTimeAnsi, sizeof(szTimeAnsi));
		nTime = atoi(szTimeAnsi);
		if (nTime > 59 || nTime < 0)
		{
			bFormatWrong = TRUE;
		}
		else
		{
			nStartSec = nTime;
		}
	}

	//����ʱ��
	{
		//ʱ
		memset(szTime, 0, sizeof(szTime));
		memset(szTimeAnsi, 0, sizeof(szTimeAnsi));
		nTime = -1;
		m_EditEndHour.GetWindowText(szTime, sizeof(szTime));
		CWriteDailyLog::UnicodeToAnsi(szTime, szTimeAnsi, sizeof(szTimeAnsi));
		nTime = atoi(szTimeAnsi);
		if (nTime > 48 || nTime < 0)
		{
			bFormatWrong = TRUE;
		}
		else
		{
			nEndHour = nTime;
		}

		//��
		memset(szTime, 0, sizeof(szTime));
		memset(szTimeAnsi, 0, sizeof(szTimeAnsi));
		nTime = -1;
		m_EditEndMin.GetWindowText(szTime, sizeof(szTime));
		CWriteDailyLog::UnicodeToAnsi(szTime, szTimeAnsi, sizeof(szTimeAnsi));
		nTime = atoi(szTimeAnsi);
		if (nTime > 59 || nTime < 0)
		{
			bFormatWrong = TRUE;
		}
		else
		{
			nEndMin = nTime;
		}

		//��
		memset(szTime, 0, sizeof(szTime));
		memset(szTimeAnsi, 0, sizeof(szTimeAnsi));
		nTime = -1;
		m_EditEndSec.GetWindowText(szTime, sizeof(szTime));
		CWriteDailyLog::UnicodeToAnsi(szTime, szTimeAnsi, sizeof(szTimeAnsi));
		nTime = atoi(szTimeAnsi);
		if (nTime > 59 || nTime < 0)
		{
			bFormatWrong = TRUE;
		}
		else
		{
			nEndSec = nTime;
		}

		//������ʾ 
		if (bFormatWrong || ((nStartHour * 3600 + nStartMin * 30 + nStartSec) >(nEndHour * 3600 + nEndMin * 60 + nEndSec)) || ((nStartHour * 3600 + nStartMin * 30 + nStartSec+24*3600) -(nEndHour * 3600 + nEndMin * 60 + nEndSec)==0))
		{
			m_StaticErrText.GetDC()->SetTextColor(RGB(255, 0, 0));
			m_StaticErrText.SetWindowText(TEXT("��Ǹ����ʽ�������顣����"));
			return;
		}
		else
		{
			m_StaticErrText.SetWindowText(TEXT(""));
		}

	}

	if (!bFormatWrong)
	{
		m_nStartHour = nStartHour;
		m_nStartMin = nStartMin;
		m_nStartSec = nStartSec;
		m_nEndHour = nEndHour;
		m_nEndMin = nEndMin;
		m_nEndSec = nEndSec;

		//д���ñ���
		WriteConfig();
	}


	CDialog::OnOK();
}

//д�����ñ�
void CRegTimeSet::WriteConfig()
{
	CIniFileIO ServerConfig;
	ServerConfig.SetFilePath(TEXT(".\\ServerConfig.ini"));

	TCHAR szTime[512] = { 0 };

	//��ʼʱ��
	{
		//ʱ
		memset(szTime, 0, sizeof(szTime));
		m_EditStartHour.GetWindowText(szTime, sizeof(szTime));
		ServerConfig.WriteString(TEXT("RegTimeSet"), TEXT("StartHour"), szTime,TEXT( ".\\ServerConfig.ini"));
		

		//��
		memset(szTime, 0, sizeof(szTime));
		m_EditStartMin.GetWindowText(szTime, sizeof(szTime));
		ServerConfig.WriteString(TEXT("RegTimeSet"), TEXT("StartMin"), szTime, TEXT(".\\ServerConfig.ini"));

		//��
		memset(szTime, 0, sizeof(szTime));
		m_EditStartSec.GetWindowText(szTime, sizeof(szTime));
		ServerConfig.WriteString(TEXT("RegTimeSet"), TEXT("StartSec"), szTime, TEXT(".\\ServerConfig.ini"));
	}

	//����ʱ��
	{
		//ʱ
		memset(szTime, 0, sizeof(szTime));
		m_EditEndHour.GetWindowText(szTime, sizeof(szTime));
		ServerConfig.WriteString(TEXT("RegTimeSet"), TEXT("EndHour"), szTime, TEXT(".\\ServerConfig.ini"));

		//��
		memset(szTime, 0, sizeof(szTime));
		m_EditEndMin.GetWindowText(szTime, sizeof(szTime));
		ServerConfig.WriteString(TEXT("RegTimeSet"), TEXT("EndMin"), szTime, TEXT(".\\ServerConfig.ini"));

		//��
		memset(szTime, 0, sizeof(szTime));
		m_EditEndSec.GetWindowText(szTime, sizeof(szTime));
		ServerConfig.WriteString(TEXT("RegTimeSet"), TEXT("EndSec"), szTime, TEXT(".\\ServerConfig.ini"));
	}

	char szRegTime[1024] = { 0 };
	sprintf(szRegTime, "\r\nע��ʱ�����ã�\r\n��ʼʱ�䣺%d:%d:%d\r\n����ʱ�䣺%d:%d:%d;\r\n"
		, m_nStartHour, m_nStartMin, m_nStartSec, m_nEndHour, m_nEndMin, m_nEndSec);

	if (m_pListenDlg)
	{
		((CListenActiveDlg *)m_pListenDlg)->DispMsg(szRegTime);
	}

}

void CRegTimeSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListenActiveDlg)
	DDX_Control(pDX, IDC_START_HOUR, m_EditStartHour);
	DDX_Control(pDX, IDC_START_MIN, m_EditStartMin);
	DDX_Control(pDX, IDC_START_SEC, m_EditStartSec);

	DDX_Control(pDX, IDC_END_HOUR, m_EditEndHour);
	DDX_Control(pDX, IDC_END_MIN, m_EditEndMin);
	DDX_Control(pDX, IDC_END_SEC, m_EditEndSec);

	DDX_Control(pDX, IDC_ERROR_TEXT, m_StaticErrText);


	//}}AFX_DATA_MAP
}

// modal processing
INT_PTR CRegTimeSet::DoModal()
{
	SYSTEMTIME sysTime;
	memset(&sysTime, 0, sizeof(sysTime));
	GetLocalTime(&sysTime);
	//CString strTime;
	if (m_nStartHour == -1)
	{
		m_nStartHour = sysTime.wHour;
	}

	//strTime.Format(TEXT("%d"), m_nStartHour);
	//m_EditStartHour.SetWindowText(strTime);
	//m_EditStartHour.SetLimitText(4);
	//strTime.Empty();

	if (m_nStartMin == -1)
	{
		m_nStartMin = sysTime.wMinute;
	}

	//strTime.Format(TEXT("%d"), m_nStartMin);
	//m_EditStartMin.SetWindowText(strTime);
	//m_EditStartMin.SetLimitText(4);
	//strTime.Empty();

	if (m_nStartSec == -1)
	{
		m_nStartSec = sysTime.wSecond;
	}

	//strTime.Format(TEXT("%d"), m_nStartSec);
	//m_EditStartSec.SetWindowText(strTime);
	//m_EditStartSec.SetLimitText(4);

	//����ʱ��
	if (m_nEndHour == -1)
	{
		m_nEndHour = sysTime.wHour;
	}

	//strTime.Format(TEXT("%d"), m_nEndHour);
	//m_EditEndHour.SetWindowText(strTime);
	//m_EditEndHour.SetLimitText(4);
	//strTime.Empty();

	if (m_nEndMin == -1)
	{
		m_nEndMin = sysTime.wMinute;
	}

	//strTime.Format(TEXT("%d"), m_nEndMin);
	//m_EditEndMin.SetWindowText(strTime);
	//m_EditEndMin.SetLimitText(4);
	//strTime.Empty();

	if (m_nEndSec == -1)
	{
		m_nEndSec = sysTime.wSecond;
	}

	//strTime.Format(TEXT("%d"), m_nEndSec);
//	m_EditEndSec.SetWindowText(strTime);
//	m_EditEndSec.SetLimitText(4);

	return CDialog::DoModal();
}

void CRegTimeSet::OnEnChangeStartHour()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


//////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListenActiveDlg dialog

#define IDI_TIMER_REG_START		2001
#define IDI_TIMER_REG_END		2002

CListenActiveDlg::CListenActiveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListenActiveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListenActiveDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nStartHour=-1;				//ʱ
	m_nStartMin=-1;				//��
	m_nStartSec=-1;				//��
	m_nEndHour=-1;					//ʱ
	m_nEndMin=-1;					//��
	m_nEndSec=-1;					//��\

}

void CListenActiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListenActiveDlg)
	DDX_Control(pDX, IDC_BUTTON2, m_btn_stop);
	DDX_Control(pDX, IDC_BUTTON1, m_btn_start);
	DDX_Control(pDX, IDC_RICHEDIT1, m_msg);

	DDX_Control(pDX, IDC_BUTTON3, m_btn_query);
	DDX_Control(pDX, IDC_BUTTON4, m_btn_UpdateConfig);
	DDX_Control(pDX, IDC_BUTTON5, m_btn_Modify);
	DDX_Control(pDX, IDC_BUTTON6, m_btn_Delete);
	DDX_Control(pDX, IDC_BUTTON7, m_btnSetRegTime);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CListenActiveDlg, CDialog)
	//{{AFX_MSG_MAP(CListenActiveDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnQuery)
	ON_BN_CLICKED(IDC_BUTTON4, OnRefreshConfig)
	ON_BN_CLICKED(IDC_BUTTON5, OnModify)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON6, OnDelete)
	ON_BN_CLICKED(IDC_BUTTON7, &CListenActiveDlg::OnBnClickedRegTimeSet)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListenActiveDlg message handlers

BOOL CListenActiveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//���ø���
	OnRefreshConfig();
	m_hMeSock = INVALID_SOCKET_ID;
	m_eventMe = NULL;
	m_bMonitor = false;
	m_btn_query.EnableWindow(FALSE);
	m_btn_UpdateConfig.EnableWindow(TRUE);
	m_btn_Modify.EnableWindow(FALSE);
	m_btn_Delete.EnableWindow(FALSE);


	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	//��ʼ����������
	this->m_msg.SetBackColor(MSG_COLOR_BLUE);
	m_msg.SetMaxLines(100);
	m_msg.SetReadOnly(FALSE);
	/*try{
		CString strFileName="endll.dll";
	   if((m_hmDll=LoadLibrary(_T(strFileName))) == NULL)        
		 return FALSE;
	   encrypt1 =(tagencrypt1)::GetProcAddress(m_hmDll,"str_encrypt_reversible");
	   encrypt2 = (tagencrypt2)::GetProcAddress(m_hmDll,"str_encrypt2_reversible");

	   if (encrypt1 == NULL || encrypt2 == NULL) DispMsg("ȱ�ټ��ܺ���");
	}catch(CException *e){
		e->Delete();
		DispMsg("����Dllʧ��!\n");
		return FALSE;
	}
	DispMsg("����Dll�ɹ�!\n");*/

	//��ʼ�����ݿ�����
	m_pDBEngine = CYcDBEngine::CreateInstance(this);
	//if (m_pDBEngine)
	//{
	//	if (m_pDBEngine->Init(this)) DispMsg("���ݿ������ʼ���ɹ�!\n");
	//}


	
	//��ʼ������
	//SetTimer(101,2000,NULL);
	runFlag=false;
	if(runFlag)
		DispMsg("�����Ѿ��ɹ�����!\n");
	else
		DispMsg("����δ����!\n");

	//���Ӵ��ڳߴ�仯
	CTaskBase *pMonitorSizeTask = new CListenSizeLimitTask(GetSafeHwnd());
	CThreadPool::GetInstance()->AddTask(pMonitorSizeTask);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CListenActiveDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CListenActiveDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CListenActiveDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//׷����ʾ����
void CListenActiveDlg::DispMsg(CString Msg){	

	//m_msg.DispMsg(rizhi, MSG_COLOR_YELLOW, 0);
	m_msg.DispMsg(Msg,MSG_COLOR_YELLOW,0);
}

void CListenActiveDlg::OnTimer(UINT nIDEvent) 
{
	//if(!runFlag)
	//	return;
	// TODO: Add your message handler code here and/or call default
	//DispMsg("�������ݿ�...!\r\n");
	//if (operateActive() == 0) KillTimer(nIDEvent);
	//if (m_pDBEngine->Connect("root", "1", "mwusers"))
	//if (m_pDBEngine->Connect((char *)m_strMysqlAdminAccount.c_str(), (char *)m_strMysqlAdminPwd.c_str(), (char *)m_strMysqlDBName.c_str()))
	
	switch (nIDEvent)
	{
	case 101:
	{
				//	DispMsg("���ӳɹ�!\r\n");

				CTaskBase *pServer = new CMonitorSockTask(this);
				CThreadPool::GetInstance()->AddTask(pServer);
				DispMsg("���ڼ���...\r\n");
				//m_pDBEngine->Close();
				KillTimer(nIDEvent);
				break;
	}
	case IDI_TIMER_REG_START:
	{
								m_bRegisterEnable = true;
								KillTimer(IDI_TIMER_REG_START);
								DispMsg("\r\n����ע��...\r\n");
								break;
	}
	case IDI_TIMER_REG_END:
	{
							  KillTimer(IDI_TIMER_REG_START);
							  KillTimer(IDI_TIMER_REG_END);
							  m_bRegisterEnable = false;

							  SYSTEMTIME sysTime;
							  memset(&sysTime, 0, sizeof(sysTime));
							  GetLocalTime(&sysTime);

							  INT nTime = sysTime.wHour * 3600 + sysTime.wMinute * 60 + sysTime.wSecond;
							  INT nStatTime = m_nStartHour * 3600 + m_nStartMin * 60 + m_nStartSec;
							  INT nEndTime = m_nEndHour * 3600 + m_nEndMin * 60 + m_nEndSec;

							//  if (nStatTime > nTime)
							  {
								 // DispMsg("nStatTime > nTime...\r\n");
								  m_bRegisterEnable = false;
								  SetTimer(IDI_TIMER_REG_START, (nStatTime+24*3600 - nTime) * 1000, NULL);
							  }

							  if (nEndTime >= 24*3600)
							  {
								  //DispMsg("nEndTime >= 24*3600...\r\n");
								  SetTimer(IDI_TIMER_REG_END, (nEndTime - nTime) * 1000, NULL);
							  }
							  else if (nEndTime >= nTime)
							  {
								  //DispMsg("nEndTime  > nTime..\r\n");
								  SetTimer(IDI_TIMER_REG_END, (nEndTime +24*3600- nTime) * 1000, NULL);
							  }

							  if (!m_bRegisterEnable)
							  {
								  DispMsg("\r\n�ر�ע��...\r\n");
							  }
							  else
							  {
								  DispMsg("\r\n����ע��...\r\n");
							  }
								break;
	}


	}


	//Sleep(10);
	CDialog::OnTimer(nIDEvent);
}

void CListenActiveDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	//runFlag=TRUE;
	m_btn_stop.EnableWindow(TRUE);
	m_btn_start.EnableWindow(FALSE);
	DispMsg("��������...\r\n");
	m_bMonitor = true;
	SetTimer(101, 2000, NULL);
}

void CListenActiveDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	//runFlag =FALSE;

	KillTimer(101);
	KillTimer(IDI_TIMER_REG_START);
	KillTimer(IDI_TIMER_REG_END);

	m_btn_stop.EnableWindow(FALSE);
	m_btn_start.EnableWindow(TRUE);
	m_bMonitor = false;

	//�����ź�
	if (m_eventMe)
		WSASetEvent(m_eventMe);

	DispMsg("������ֹͣ............\r\n");

	// TODO: Add your control notification handler code here
	
}
int CListenActiveDlg::operateActive(){
	/*try{
		//��ȡ���ݿ�����Ҫ���������
		CString strSql ="SELECT id,userid,password,email,game_server_id from users_activation order by id asc limit 1";
		long lngRowCount=0;
		mysqldb.setDbName("mysql");
		if(!mysqldb.connectDB()){
			DispMsg("���ӷ�����ʧ��!");
			return -1;
		}

		DispMsg("���ݿ����ӳɹ�!\r\n");
		//return 0;
		CStringArray* temp = mysqldb.excuteQuery(strSql,&lngRowCount);
	
		if(lngRowCount<1){
			mysqldb.close();
		//	DispMsg("��ȡ������Ϣʧ��!");
			return -1;
		}
		mysqldb.close();
		//sscanf(temp[0].GetAt(0),"%d",&activeuser.id);
		activeuser.userid = temp[0].GetAt(1);
		activeuser.password = temp[0].GetAt(2);
		activeuser.email = temp[0].GetAt(3);
		activeuser.game_server_id = temp[0].GetAt(4);
		delete[] temp;
		this->DispMsg("userid:" + activeuser.userid+"\r\n");
		this->DispMsg("serviceid:" + activeuser.game_server_id+"\r\n");

		//���ܺ����account
		char *key;
		key = (encrypt1)((char*)(LPCTSTR)activeuser.password,(char*)(LPCTSTR)activeuser.userid); 
		key = (encrypt2)(key,(char*)(LPCTSTR)activeuser.email); 
		CString strKey = key;
		
		strSql = "insert into account(userid,password,fullname,email,account_create,permission_level,pts_balance,pts_method,pts_start,next_pts_method,suspend_end,first_login,last_login, login_count) values('"
			+ activeuser.userid + "','" + strKey + "','" + activeuser.userid +"','"
			 + activeuser.email + "',NOW(), 0, 0, 'LIFE', now(), 'LIFE', default, default, default, default)";
		//����account
		mysqldb.setDbName(activeuser.game_server_id);
	
		/*
		if(!mysqldb.connectDB()){
			DispMsg("���ӷ�����ʧ��!");
			return -1;
		}
*/
	/*	if(mysqldb.excuteSql(strSql)<1){
			DispMsg("����ʧ��!");
			return -2;	
		}
	
//		mysqldb.close();
		//ɾ�������¼
		mysqldb.setDbName("gamedb");
		
		CString strTemp;
		//strTemp.Format("%d",activeuser.id);
		strSql = "delete from users_activation where id='" + strTemp + "'";
		if(mysqldb.excuteSql(strSql)<1){
			DispMsg("����ʧ��!");
			return -3;	
		}
	}catch(CException *e){
		e->Delete();
		mysqldb.close();
		return -1;
	}*/
	return 0;
}

//���÷���ʹ��
void CListenActiveDlg::SetServerRunFlag(BOOL bRun)
{
	runFlag = bRun;
}

//��������
void CListenActiveDlg::OnRefreshConfig()
{
	if (runFlag)
	{
		DispMsg("������δֹͣ!���ܸ�������...\r\n");
		return;
	}

	//��������
	CIniFileIO ServerConfig;
	ServerConfig.SetFilePath(TEXT(".\\ServerConfig.ini"));
	TCHAR szString[1024] = { 0 };
	ServerConfig.ReadString(TEXT("ServerConfig"), TEXT("IP"), TEXT(""), szString, 1024);

	char cStringCache[1024] = { 0 };
	CWriteDailyLog::UnicodeToAnsi(szString, cStringCache, 1024);

	//��ַ
	m_strServerAddr.clear();
	m_strServerAddr = cStringCache;

	//�˿�
	m_ServerPort = ServerConfig.ReadInt(TEXT("ServerConfig"), TEXT("port"), 0);

	int iEnable = 0;
	//ע��ʹ��
	iEnable = ServerConfig.ReadInt(TEXT("ServerConfig"), TEXT("RegisterEnale"), 0);
	m_bRegisterEnable = iEnable > 0 ? true : false;

	iEnable = 0;
	//�޸�����ʹ��
	iEnable = ServerConfig.ReadInt(TEXT("ServerConfig"), TEXT("ModifyPwdEnale"), 0);
	m_bModifyPwdEnable = iEnable > 0 ? true : false;

	//���ݿ����ӳ�ʱʱ��
	memset(szString, 0, sizeof(TCHAR)* 1024);
	ServerConfig.ReadString(TEXT("MySqlConfig"), TEXT("connect_timeout"), TEXT(""), szString, 1024);
	memset(cStringCache, 0, sizeof(cStringCache));
	CWriteDailyLog::UnicodeToAnsi(szString, cStringCache, 1024);
	m_strMysqlConnectTimeOut.clear();
	m_strMysqlConnectTimeOut = cStringCache;

	//���ݿ����Ա�ʺ�
	memset(szString, 0, sizeof(TCHAR)* 1024);
	ServerConfig.ReadString(TEXT("MySqlConfig"), TEXT("admin_account"), TEXT(""), szString, 1024);
	memset(cStringCache, 0, sizeof(cStringCache));
	CWriteDailyLog::UnicodeToAnsi(szString, cStringCache, 1024);
	m_strMysqlAdminAccount.clear();
	m_strMysqlAdminAccount = cStringCache;

	//���ݿ����Ա���� 
	memset(szString, 0, sizeof(TCHAR)* 1024);
	ServerConfig.ReadString(TEXT("MySqlConfig"), TEXT("admin_pwd"), TEXT(""), szString, 1024);
	memset(cStringCache, 0, sizeof(cStringCache));
	CWriteDailyLog::UnicodeToAnsi(szString, cStringCache, 1024);
	m_strMysqlAdminPwd.clear();
	m_strMysqlAdminPwd = cStringCache;

	//���ݿ���
	memset(szString, 0, sizeof(TCHAR)* 1024);
	ServerConfig.ReadString(TEXT("MySqlConfig"), TEXT("database_name"), TEXT(""), szString, 1024);
	memset(cStringCache, 0, sizeof(cStringCache));
	CWriteDailyLog::UnicodeToAnsi(szString, cStringCache, 1024);
	m_strMysqlDBName.clear();
	m_strMysqlDBName = cStringCache;

	//ע��ʱ������
	{
		//��ʼʱ��
		{
			//ʱ
			memset(szString, 0, sizeof(TCHAR)* 1024);
			ServerConfig.ReadString(TEXT("RegTimeSet"), TEXT("StartHour"), TEXT(""), szString, 1024);
			memset(cStringCache, 0, sizeof(cStringCache));
			CWriteDailyLog::UnicodeToAnsi(szString, cStringCache, 1024);
			m_nStartHour = atoi(cStringCache);

			//��
			memset(szString, 0, sizeof(TCHAR)* 1024);
			ServerConfig.ReadString(TEXT("RegTimeSet"), TEXT("StartMin"), TEXT(""), szString, 1024);
			memset(cStringCache, 0, sizeof(cStringCache));
			CWriteDailyLog::UnicodeToAnsi(szString, cStringCache, 1024);
			m_nStartMin = atoi(cStringCache);

			//��
			memset(szString, 0, sizeof(TCHAR)* 1024);
			ServerConfig.ReadString(TEXT("RegTimeSet"), TEXT("StartSec"), TEXT(""), szString, 1024);
			memset(cStringCache, 0, sizeof(cStringCache));
			CWriteDailyLog::UnicodeToAnsi(szString, cStringCache, 1024);
			m_nStartSec = atoi(cStringCache);
		}

		//����ʱ��
		{
		//ʱ
		memset(szString, 0, sizeof(TCHAR)* 1024);
		ServerConfig.ReadString(TEXT("RegTimeSet"), TEXT("EndHour"), TEXT(""), szString, 1024);
		memset(cStringCache, 0, sizeof(cStringCache));
		CWriteDailyLog::UnicodeToAnsi(szString, cStringCache, 1024);
		m_nEndHour = atoi(cStringCache);

		//��
		memset(szString, 0, sizeof(TCHAR)* 1024);
		ServerConfig.ReadString(TEXT("RegTimeSet"), TEXT("EndMin"), TEXT(""), szString, 1024);
		memset(cStringCache, 0, sizeof(cStringCache));
		CWriteDailyLog::UnicodeToAnsi(szString, cStringCache, 1024);
		m_nEndMin = atoi(cStringCache);

		//��
		memset(szString, 0, sizeof(TCHAR)* 1024);
		ServerConfig.ReadString(TEXT("RegTimeSet"), TEXT("EndSec"), TEXT(""), szString, 1024);
		memset(cStringCache, 0, sizeof(cStringCache));
		CWriteDailyLog::UnicodeToAnsi(szString, cStringCache, 1024);
		m_nEndSec = atoi(cStringCache);
	}


		//�����Զ�ע��ʱ������
		KillTimer(IDI_TIMER_REG_START);
		KillTimer(IDI_TIMER_REG_END);

	

	}
	
	char szRegTime[1024] = { 0 };
	sprintf(szRegTime, "\r\n��ʼʱ�䣺%d:%d:%d\r\n����ʱ�䣺%d:%d:%d;\r\n"
		, m_nStartHour, m_nStartMin, m_nStartSec, m_nEndHour, m_nEndMin, m_nEndSec);


	//��������
	char ServerConfigDetails[1024] = { 0 };
	sprintf(ServerConfigDetails, "��ȡ���óɹ���ip:%s;port:%d;\r\nע��ʱ������:%s ע��ʹ�ܣ�%s;\r\n�޸�����ʹ�ܣ�%s;���ݿ����ӳ�ʱʱ��:%s\r\n���ݿ���:%s;\r\n"
		, m_strServerAddr.c_str(), m_ServerPort, szRegTime, m_bRegisterEnable ? "����" : "����"
		, m_bModifyPwdEnable ? "����" : "����", m_strMysqlConnectTimeOut.c_str()
		, m_strMysqlDBName.c_str());

	DispMsg(ServerConfigDetails);
}

//��ɾ�Ĳ�
void CListenActiveDlg::OnQuery()
{
	return;

	if (!runFlag) return DispMsg("����δ����\r\n");

	
}

void CListenActiveDlg::OnRegister()
{
	return;

	if (!runFlag) return DispMsg("����δ����\r\n");

	int iAbortNormal = 0;

	try
	{
		//�쳣1
		iAbortNormal++;

		activeuser.userid = "fengyanchen2";
		activeuser.password = "123456abc";
		activeuser.email = "3344520@qq.com";

		DispMsg("userid:" + activeuser.userid + "\r\n");
		DispMsg("password:" + activeuser.password + "\r\n");
		DispMsg("password:" + activeuser.password + "\r\n");

		//�쳣2
		iAbortNormal++;

		//���ܺ����account
		char key[4096] = { 0 };
		memset(key, 0, 4096);
		strcpy(key, (encrypt1)((char *)(LPCTSTR)activeuser.password, (char *)(LPCTSTR)activeuser.userid));
		
		char rizhi[100] = { 0 };
		sprintf(rizhi, "key len:%d\r\n", strlen(key));
		DispMsg(rizhi);
		key[4095] = 0;

		//�쳣3
		iAbortNormal++;
		char key2[4096] = { 0 };
		strcpy(key2, (encrypt2)((char *)(LPCTSTR)key, (char *)(LPCTSTR)activeuser.email));
		
		memset(rizhi, 0, 100);
		sprintf(rizhi, "key2 len:%d\r\n", strlen(key2));
		DispMsg(rizhi);
		key2[4095] = 0;



		//�쳣4
		iAbortNormal++;
		DispMsg(key2);

		//�쳣5
		iAbortNormal++;

		char keyCache[4096] = { 0 };
		memmove(keyCache, key2, 4096);
		keyCache[4095] = 0;
		//CString strKeyd ;
		//strKeyd += key2;

		//�쳣6
		iAbortNormal++;

		//	strSql = "insert into account(userid,password,fullname,email,account_create,permission_level,pts_balance,pts_method,pts_start,next_pts_method,suspend_end,first_login,last_login, login_count) values('"
		//		+ activeuser.userid + "','" + strKey + "','" + activeuser.userid + "','"
		//		+ activeuser.email + "',NOW(), 0, 0, 'LIFE', now(), 'LIFE', default, default, default, default)";

		char strSql[400] = { 0 };
		sprintf(strSql, "insert into account(userid, password, fullname, email)values ('%s', '%s', '%s','%s');"
			, activeuser.userid, keyCache, activeuser.userid, activeuser.email);  //������취ʵ���ֶ��ڿ���̨�ֶ�����ָ��

		//�쳣7
		iAbortNormal++;

		bool bResult = false;
		int iRowResult = 0, iErr = 0;
		char strResult[100] = { 0 };

		if (m_pDBEngine->ExecuteSql(strSql, NULL, &bResult, &iRowResult,NULL, &iErr, this))
		{
			memset(strResult, 0, 100);
			sprintf(strResult, "bResult %s,iRowResult %d, iErr %d\r\n", bResult ? "��" : "û��", iRowResult, iErr);
			DispMsg(strResult);

			DispMsg("ע��ɹ�!\r\n");
			if (m_pDBEngine->ExecuteSql("select *from account where userid='fengyanchen2'", NULL, &bResult, &iRowResult,NULL, &iErr, this))
			{
				memset(strResult, 0, 100);
				sprintf(strResult, "bResult %s,iRowResult %d, iErr %d\r\n", bResult ? "��" : "û��", iRowResult, iErr);
				DispMsg(strResult);

				DispMsg("��ѯ�ɹ�!\r\n");
			}
			else
			{
				memset(strResult, 0, 100);
				sprintf(strResult, "bResult %s,iRowResult %d, iErr %d\r\n", bResult ? "��" : "û��", iRowResult, iErr);
				DispMsg(strResult);

				DispMsg("��ѯʧ��!\r\n");
			}
		}
		else
			DispMsg("ע��ʧ��!\r\n");

		memset(strResult, 0, 100);
		sprintf(strResult, "bResult %s,iRowResult %d, iErr %d\r\n", bResult ? "��" : "û��", iRowResult, iErr);
		DispMsg(strResult);

		//�쳣8
		iAbortNormal++;
	}
	catch (...)
	{
		char error[50] = { 0 };
		sprintf(error, "OnRegister  Abnormal error:%d\r\n", iAbortNormal);
		DispMsg(error);

		return;
	}
	
	return;
}

void CListenActiveDlg::OnModify()
{
	return;

	if (!runFlag) return DispMsg("����δ����\r\n");

	int iAbortNormal = 0;

	try
	{
		//�쳣1
		iAbortNormal++;

		activeuser.userid = "fengyanchen2";
		activeuser.password = "234eeed";
		activeuser.email = "3344520@qq.com";

		DispMsg("userid:" + activeuser.userid + "\r\n");
		DispMsg("password:" + activeuser.password + "\r\n");
		DispMsg("password:" + activeuser.password + "\r\n");

		//�쳣2
		iAbortNormal++;

		//���ܺ����account
		char key[4096] = { 0 };
		memset(key, 0, 4096);
		strcpy(key, (encrypt1)((char *)(LPCTSTR)activeuser.password, (char *)(LPCTSTR)activeuser.userid));

		char rizhi[100] = { 0 };
		sprintf(rizhi, "key len:%d\r\n", strlen(key));
		DispMsg(rizhi);
		key[4095] = 0;

		//�쳣3
		iAbortNormal++;
		char key2[4096] = { 0 };
		strcpy(key2, (encrypt2)((char *)(LPCTSTR)key, (char *)(LPCTSTR)activeuser.email));

		memset(rizhi, 0, 100);
		sprintf(rizhi, "key2 len:%d\r\n", strlen(key2));
		DispMsg(rizhi);
		key2[4095] = 0;



		//�쳣4
		iAbortNormal++;
		DispMsg(key2);

		//�쳣5
		iAbortNormal++;

		char keyCache[4096] = { 0 };
		memmove(keyCache, key2, 4096);
		keyCache[4095] = 0;
		//CString strKeyd ;
		//strKeyd += key2;

		//�쳣6
		iAbortNormal++;

		//	strSql = "insert into account(userid,password,fullname,email,account_create,permission_level,pts_balance,pts_method,pts_start,next_pts_method,suspend_end,first_login,last_login, login_count) values('"
		//		+ activeuser.userid + "','" + strKey + "','" + activeuser.userid + "','"
		//		+ activeuser.email + "',NOW(), 0, 0, 'LIFE', now(), 'LIFE', default, default, default, default)";

		char strSql[400] = { 0 };
		sprintf(strSql, "update account set password='%s' where userid='fengyanchen2'"
			, keyCache);  //������취ʵ���ֶ��ڿ���̨�ֶ�����ָ��

		//�쳣7
		iAbortNormal++;

		bool bResult = false;
		int iRowResult = 0, iErr = 0;
		char strResult[100] = { 0 };

		if (m_pDBEngine->ExecuteSql(strSql, NULL, &bResult, &iRowResult,NULL, &iErr, this))
		{
			memset(strResult, 0, 100);
			sprintf(strResult, "bResult %s,iRowResult %d, iErr %d\r\n", bResult ? "��" : "û��", iRowResult, iErr);
			DispMsg(strResult);

			DispMsg("�����޸ĳɹ�!\r\n");
			if (m_pDBEngine->ExecuteSql("select *from account where userid='fengyanchen2'",NULL, &bResult, &iRowResult,NULL, &iErr, this))
			{
				DispMsg("��ѯ�ɹ�!\r\n");
				memset(strResult, 0, 100);
				sprintf(strResult, "bResult %s,iRowResult %d, iErr %d\r\n", bResult ? "��" : "û��", iRowResult, iErr);
				DispMsg(strResult);

			}
			else
			{
				DispMsg("��ѯʧ��!\r\n");
				memset(strResult, 0, 100);
				sprintf(strResult, "bResult %s,iRowResult %d, iErr %d\r\n", bResult ? "��" : "û��", iRowResult, iErr);
				DispMsg(strResult);
			}
		}
		else
			DispMsg("�����޸�ʧ��!\r\n");

		memset(strResult, 0, 100);
		sprintf(strResult, "bResult %s,iRowResult %d, iErr %d\r\n", bResult ? "��" : "û��", iRowResult, iErr);
		DispMsg(strResult);

		//�쳣8
		iAbortNormal++;
	}
	catch (...)
	{
		char error[50] = { 0 };
		sprintf(error, "OnModify  Abnormal error:%d\r\n", iAbortNormal);
		DispMsg(error);

		return;
	}
}

void CListenActiveDlg::OnDelete()
{
	return;

	if (!runFlag) return DispMsg("����δ����\r\n");

	int iAbnormal = 0;
	try
	{
		//�쳣1
		iAbnormal++;

		bool bResult = false;
		int iRowResult = 0, iErr = 0;
		char strResult[100] = { 0 };

		if (m_pDBEngine->ExecuteSql("delete from account where userid='fengyanchen2' ",NULL, &bResult, &iRowResult,NULL, &iErr, this))
		{
			DispMsg("�˺�ɾ���ɹ�!\r\n");
			memset(strResult, 0, 100);
			sprintf(strResult, "bResult %s,iRowResult %d, iErr %d\r\n", bResult ? "��" : "û��", iRowResult, iErr);
			DispMsg(strResult);


			if (m_pDBEngine->ExecuteSql("select *from account where userid='fengyanchen2'", NULL, &bResult, &iRowResult,NULL, &iErr, this))
			{
				DispMsg("��ѯ�н��!\r\n");
				memset(strResult, 0, 100);
				sprintf(strResult, "bResult %s,iRowResult %d, iErr %d\r\n", bResult ? "��" : "û��", iRowResult, iErr);
				DispMsg(strResult);


			}
			else
			{
				DispMsg("��ѯ�޽��!\r\n");
				memset(strResult, 0, 100);
				sprintf(strResult, "bResult %s,iRowResult %d, iErr %d\r\n", bResult ? "��" : "û��", iRowResult, iErr);
				DispMsg(strResult);

			}
		}
		else
			DispMsg("�˺�ɾ��ʧ��!\r\n");

		memset(strResult, 0, 100);
		sprintf(strResult, "bResult %s,iRowResult %d, iErr %d\r\n", bResult ? "��" : "û��", iRowResult, iErr);
		DispMsg(strResult);


		//�쳣2
		iAbnormal++;


	}
	catch (...)
	{
		char error[50] = { 0 };
		sprintf(error, "OnDelete  Abnormal error:%d\r\n", iAbnormal);
		DispMsg(error);
	}
}

void CListenActiveDlg::OnBnClickedRegTimeSet()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	SYSTEMTIME sysTime;
	memset(&sysTime, 0, sizeof(sysTime));
	

	CRegTimeSet RegTimeSet(this);
	RegTimeSet.m_nStartHour = m_nStartHour;
	RegTimeSet.m_nStartMin = m_nStartMin;
	RegTimeSet.m_nStartSec = m_nStartSec;
	RegTimeSet.m_nEndHour = m_nEndHour;
	RegTimeSet.m_nEndMin = m_nEndMin;
	RegTimeSet.m_nEndSec = m_nEndSec;
	RegTimeSet.m_pListenDlg = this;
	if (RegTimeSet.DoModal() == IDOK)
	{
		m_nStartHour = RegTimeSet.m_nStartHour;
		m_nStartMin = RegTimeSet.m_nStartMin;
		m_nStartSec = RegTimeSet.m_nStartSec;
		m_nEndHour = RegTimeSet.m_nEndHour;
		m_nEndMin = RegTimeSet.m_nEndMin;
		m_nEndSec = RegTimeSet.m_nEndSec;

		RegEnabelAnalys();

	}	
}

//ע�����
void CListenActiveDlg::RegEnabelAnalys()
{
	SYSTEMTIME sysTime;
	memset(&sysTime, 0, sizeof(sysTime));

	KillTimer(IDI_TIMER_REG_START);
	KillTimer(IDI_TIMER_REG_END);

	GetLocalTime(&sysTime);
	INT nTime = sysTime.wHour * 3600 + sysTime.wMinute * 60 + sysTime.wSecond;
	INT nStatTime = m_nStartHour * 3600 + m_nStartMin * 60 + m_nStartSec;
	INT nEndTime = m_nEndHour * 3600 + m_nEndMin * 60 + m_nEndSec;
	if (nStatTime > nTime)
	{
		//	DispMsg("nStatTime > nTime...\r\n");
		m_bRegisterEnable = false;
		SetTimer(IDI_TIMER_REG_START, (nStatTime - nTime) * 1000, NULL);
	}
	else
	{
		//DispMsg("nStatTime <= nTime m_bRegisterEnable=true...\r\n");
		m_bRegisterEnable = true;
	}

	if (nEndTime >= (24 * 3600))
	{
		//	DispMsg("nEndTime >= 24*3600...\r\n");
		SetTimer(IDI_TIMER_REG_END, (nEndTime - nTime) * 1000, NULL);
	}
	else if (nEndTime > nTime)
	{
		//DispMsg("nEndTime  > nTime...\r\n");
		SetTimer(IDI_TIMER_REG_END, (nEndTime - nTime) * 1000, NULL);
	}
	else
	{
		//	DispMsg("nEndTime < nTime...\r\n");
		m_bRegisterEnable = false;
		SetTimer(IDI_TIMER_REG_START, (nStatTime + 24 * 3600 - nTime) * 1000, NULL);
	}

	if (nStatTime < nTime&&nEndTime > nTime)
	{
		if (!m_bRegisterEnable)
		{
			DispMsg("\r\n�ر�ע��...\r\n");
		}
		else
		{
			DispMsg("\r\n����ע��...\r\n");
		}
	}
	if (nEndTime < nTime)
	{
		if (!m_bRegisterEnable)
		{
			DispMsg("\r\n�ر�ע��...\r\n");
		}
		else
		{
			DispMsg("\r\n����ע��...\r\n");
		}
	}
}

