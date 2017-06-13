#include"stdafx.h"
#include"AdminWnd.h"







CAdminDlg::CAdminDlg(CWnd* pParent /*=NULL*/)
: CDialog(CAdminDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CAdminDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListenActiveDlg)
	DDX_Control(pDX, IDC_BUTTON1, m_BtnGenerate);
	DDX_Control(pDX, IDC_EDIT1, m_EditIP);

	DDX_Control(pDX, IDC_EDIT2, m_EditPort);
//	DDX_Control(pDX, IDC_EDIT3, m_EditFile);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAdminDlg, CDialog)
	//{{AFX_MSG_MAP(CListenActiveDlg)
	ON_WM_PAINT()

	ON_BN_CLICKED(IDC_BUTTON1, &CAdminDlg::OnBnClickedGenerate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListenActiveDlg message handlers

BOOL CAdminDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	SetWindowText(TEXT("客户端配置生成器"));
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CAdminDlg::OnPaint()
{

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

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


void CAdminDlg::OnBnClickedGenerate()
{
	CString strIP, strPort, strFile;
	// TODO:  在此添加控件通知处理程序代码

	//ip
	GetDlgItem(IDC_EDIT1)->GetWindowText(strIP);

	//port
	GetDlgItem(IDC_EDIT2)->GetWindowText(strPort);

	//file
	//GetDlgItem(IDC_EDIT3)->GetWindowText(strFile);

	CIniFileIO WriteIni;
	TCHAR szPath[512] = { 0 };
	_sntprintf(szPath, 512, TEXT(".\\ClientConfig.ini"));

	TCHAR szCache[8192] = { 0 };
	char szCacheTmp[8192] = { 0 };
	char szCipherTmp[8192] = { 0 };
	char *pCipher = szCipherTmp;
	unsigned long ulCipherSize = 8192;
	CRsaEngine RsaSystem;
	RsaSystem.SetPublicKey((unsigned char *)szClientPublicKey);

	//ip
	CWriteDailyLog::UnicodeToAnsi(strIP.GetBuffer(), szCacheTmp, 8192);
	RsaSystem.Encrypte((unsigned char *)szCacheTmp, strlen(szCacheTmp), (unsigned char **)&pCipher, &ulCipherSize);
	CWriteDailyLog::AnsiToUnicode(pCipher, szCache, 8192);
	WriteIni.WriteString(TEXT("Server"), TEXT("ip"), szCache, szPath);

	memset(szCache, 0, sizeof(TCHAR)* 8192);
	memset(szCacheTmp, 0,  8192);
	memset(szCipherTmp, 0, 8192);
	pCipher = szCipherTmp;
	ulCipherSize = 8192;

	CWriteDailyLog::UnicodeToAnsi(strPort.GetBuffer(), szCacheTmp, 8192);
	RsaSystem.Encrypte((unsigned char *)szCacheTmp, strlen(szCacheTmp), (unsigned char **)&pCipher, &ulCipherSize);
	CWriteDailyLog::AnsiToUnicode(pCipher, szCache, 8192);
	WriteIni.WriteString(TEXT("Server"), TEXT("port"), szCache, szPath);

	AfxMessageBox(TEXT("生成成功！"));
}
