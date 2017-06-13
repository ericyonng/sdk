#include"stdafx.h"
#include"DlgSdkTools.h"
#include"Task.h"

BEGIN_MESSAGE_MAP(CExRichEdit, CRichEditCtrl)
	//{{AFX_MSG_MAP(CExRichEdit)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP

	ON_WM_CONTEXTMENU()	//菜单消息
	ON_WM_CREATE()

	ON_COMMAND(ID_RICH_COPY, OnCopy)
	ON_COMMAND(ID_RICH_CUT, OnCut)
	ON_COMMAND(ID_RICH_PASTE, OnPaste)
	ON_COMMAND(ID_RICH_SELECTALL, OnSelectall)
	ON_COMMAND(ID_RICH_UNDO, OnUndo)
	ON_COMMAND(ID_RICH_CLEAR, OnClear)
	ON_COMMAND(ID_RICH_SETFONT, OnSelectfont)
END_MESSAGE_MAP()

CExRichEdit::CExRichEdit()
{
	lMaxLines = 0;
}

CExRichEdit::~CExRichEdit()
{
}




/////////////////////////////////////////////////////////////////////////////
// CExRichEdit message handlers

void CExRichEdit::SetMaxLines(long maxlines)
{
	lMaxLines = maxlines;
}

void CExRichEdit::DispMsg(CString Msg, COLORREF fn_color, DWORD fn_effects)
{
	int nTextLength;// = m_cMessage.GetWindowTextLength();

	SetSel(-1, -1);

	CHARFORMAT cf;
	memset((char*)&cf, 0, sizeof(cf));
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_COLOR | CFM_BOLD | CFM_CHARSET | CFM_FACE | CFM_SIZE;
	cf.dwEffects = fn_effects;
	cf.crTextColor = fn_color;
	cf.bCharSet = GB2312_CHARSET;
	lstrcpy(cf.szFaceName, TEXT("宋体"));
	cf.yHeight = 180;

	SetSelectionCharFormat(cf);

	ReplaceSel(Msg);

	int TotalLines = GetLineCount();
	if (lMaxLines > 0) {
		if (TotalLines > lMaxLines)
		{
			//SetReadOnly(FALSE);
			nTextLength = LineIndex(TotalLines - lMaxLines);		//清除前几行
			SetSel(0, nTextLength);
			Clear();
			//SetReadOnly(TRUE);
		}
	}

	PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
}

void CExRichEdit::SetBackColor(COLORREF bk_color)
{
//	SetReadOnly(FALSE);
	SetBackgroundColor(FALSE, bk_color);
	//SetReadOnly(TRUE);
}

void CExRichEdit::DispMsg(CString Msg)
{
	int nTextLength;// = m_cMessage.GetWindowTextLength();

	SetSel(-1, -1);

	CHARFORMAT cf;
	memset((char*)&cf, 0, sizeof(cf));
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_COLOR | CFM_BOLD | CFM_CHARSET | CFM_FACE | CFM_SIZE;
	cf.dwEffects = 0;
	cf.crTextColor = MSG_COLOR_BLACK;
	cf.bCharSet = GB2312_CHARSET;
	lstrcpy(cf.szFaceName, TEXT("宋体"));
	cf.yHeight = 180;

	SetSelectionCharFormat(cf);

	ReplaceSel(Msg);

	int TotalLines = GetLineCount();
	if (lMaxLines > 0) {
		if (TotalLines > lMaxLines)
		{
			//SetReadOnly(FALSE);
			nTextLength = LineIndex(TotalLines - lMaxLines);
			SetSel(0, nTextLength);
			Clear();
			//SetReadOnly(TRUE);
		}
	}

	PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
}

void CExRichEdit::OnSelectfont()
{
	// TODO: Add your command handler code here
	CHARFORMAT cf;
	LOGFONT lf;
	memset(&cf, 0, sizeof(CHARFORMAT));
	memset(&lf, 0, sizeof(LOGFONT));
	//判断是否选择了内容
	BOOL m_bSelect = (GetSelectionType() != SEL_EMPTY) ? TRUE : FALSE;
	if (m_bSelect)
	{
		GetSelectionCharFormat(cf);
	}
	else
	{
		GetDefaultCharFormat(cf);
	}
	//得到相关字体属性
	BOOL bIsBold = cf.dwEffects & CFE_BOLD;
	BOOL bIsItalic = cf.dwEffects & CFE_ITALIC;
	BOOL bIsUnderline = cf.dwEffects & CFE_UNDERLINE;
	BOOL bIsStrickout = cf.dwEffects & CFE_STRIKEOUT;
	//设置属性
	lf.lfCharSet = cf.bCharSet;
	lf.lfHeight = cf.yHeight / 15;
	lf.lfPitchAndFamily = cf.bPitchAndFamily;
	lf.lfItalic = bIsItalic;
	lf.lfWeight = (bIsBold ? FW_BOLD : FW_NORMAL);
	lf.lfUnderline = bIsUnderline;
	lf.lfStrikeOut = bIsStrickout;
	memset(lf.lfFaceName, 0, sizeof(lf.lfFaceName));
	lstrcpyn(lf.lfFaceName, cf.szFaceName, CountArray(lf.lfFaceName));

	CFontDialog dlg(&lf);
	dlg.m_cf.rgbColors = cf.crTextColor;
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCharFormat(cf);               //获得所选字体的属性
		if (m_bSelect)
			SetSelectionCharFormat(cf);  //为选定的内容设定所选字体
		else
			SetWordCharFormat(cf);       //为将要输入的内容设定字体
	}

}

int CExRichEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CRichEditCtrl::OnCreate(lpCreateStruct)==-1) return -1;
	SetEventMask(ENM_MOUSEEVENTS);
	return 0;
}

void CExRichEdit::DoDataExchange(CDataExchange* pDX)
{
	CRichEditCtrl::DoDataExchange(pDX);


	//LoadConfig();
	//	DDX_Control(pDX, IDC_EDIT3, m_EditFile);

	//}}AFX_DATA_MAP
}

//响应内容消息
void CExRichEdit::OnContextMenu(CWnd* pWnd, CPoint pos)
{
	CRichEditCtrl::OnContextMenu(pWnd, pos);

	//创建一个弹出式菜单
	CMenu popmenu;
	popmenu.CreatePopupMenu();

	//添加菜单项目
	popmenu.AppendMenu(0, ID_RICH_UNDO, TEXT("&撤销"));
	popmenu.AppendMenu(0, MF_SEPARATOR);
	popmenu.AppendMenu(0, ID_RICH_CUT, TEXT("&剪切"));
	popmenu.AppendMenu(0, ID_RICH_COPY, TEXT("&复制"));
	popmenu.AppendMenu(0, ID_RICH_PASTE, TEXT("&粘贴"));
	popmenu.AppendMenu(0, ID_RICH_CLEAR, TEXT("&清除"));
	popmenu.AppendMenu(0, MF_SEPARATOR);
	popmenu.AppendMenu(0, ID_RICH_SELECTALL, TEXT("&选择全部"));
	popmenu.AppendMenu(0, MF_SEPARATOR);
	popmenu.AppendMenu(0, ID_RICH_SETFONT, TEXT("&选择字体"));

	//初始化菜单项
	UINT nUndo = (CanUndo() ? 0 : MF_GRAYED);
	popmenu.EnableMenuItem(ID_RICH_UNDO, MF_BYCOMMAND | nUndo);
	UINT nSel = ((GetSelectionType() != SEL_EMPTY) ? 0 : MF_GRAYED);
	popmenu.EnableMenuItem(ID_RICH_CUT, MF_BYCOMMAND | nSel);
	popmenu.EnableMenuItem(ID_RICH_COPY, MF_BYCOMMAND | nSel);
	popmenu.EnableMenuItem(ID_RICH_CLEAR, MF_BYCOMMAND | nSel);
	UINT nPaste = (CanPaste() ? 0 : MF_GRAYED);
	popmenu.EnableMenuItem(ID_RICH_PASTE, MF_BYCOMMAND | nPaste);

	//显示菜单
	CPoint pt;
	GetCursorPos(&pt);
	popmenu.TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
	popmenu.DestroyMenu();
}

//////////////////////////////////////////////////////////////////////



class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUT_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_CONTACT_EDIT)->SetWindowText(TEXT("1602938334@qq.com 昵称：焱烈"));
	return TRUE;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}



/////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDlgSdkTools, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CDlgSdkTools::OnBnClickedOk)
	ON_EN_CHANGE(IDC_HEADER_EDIT, &CDlgSdkTools::OnEnChangeHeaderEdit)
	ON_BN_CLICKED(IDB_REFRESH, &CDlgSdkTools::OnBnClickedRefresh)
	ON_BN_CLICKED(IDB_ABOUT, &CDlgSdkTools::OnBnClickedAbout)
END_MESSAGE_MAP()

CDlgSdkTools::CDlgSdkTools(CWnd* pParent)
: CDialog(CDlgSdkTools::IDD, pParent)
{
	//m_hIcon=AfxGetApp()->LoadIcon(IDI_ICON1);

}


void CDlgSdkTools::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_HEADER_EDIT, m_HeaderFileView);
	DDX_Control(pDX, IDC_LIB_EDIT, m_LibFileView);
	DDX_Control(pDX, IDOK, m_btnEnvBuild);
	DDX_Control(pDX, IDCANCEL, m_btnCancel); 
	DDX_Control(pDX, IDB_REFRESH, m_btnRefreshList);
	//LoadConfig();
	//	DDX_Control(pDX, IDC_EDIT3, m_EditFile);

	//}}AFX_DATA_MAP
}
BOOL CDlgSdkTools::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	SetWindowText(TEXT("EricYonngSdkTools"));

	m_HeaderFileView.SetReadOnly(FALSE);
	m_HeaderFileView.SetBackColor(MSG_COLOR_BLUE);
	m_HeaderFileView.SetMaxLines(1000000);	//100万条

	m_LibFileView.SetReadOnly(FALSE);
	m_LibFileView.SetBackColor(MSG_COLOR_BLUE);
	m_LibFileView.SetMaxLines(1000000);	//100万条

	LoadConfig();

	ReadHeaderFiles();

	ReadLibraryFiles(); 

	//添加系统菜单
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUT_DIALOG & 0xFFF0) == IDM_ABOUT_DIALOG);
	ASSERT(IDM_ABOUT_DIALOG < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUT_DIALOG);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUT_DIALOG, strAboutMenu);	//在原基础上追加
		}
	}

	m_TextFont.CreateFont(12, 0, 0, 0, 400, 0, 0, 0, GB2312_CHARSET, OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("宋体"));


	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CDlgSdkTools::OnPaint()
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


void CDlgSdkTools::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUT_DIALOG)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CDlgSdkTools::OnBnClickedOk()
{
	//PostMessage(WM_CLOSE);
	// TODO:  在此添加控件通知处理程序代码
	//CDialog::OnOK();

	//拷贝文件任务
	CTaskBase *pCopyFilesTask = new CCopyFilesTask(this);
	GlobalThreadPool.AddTask(pCopyFilesTask);

	//bool bCopySuccess=true;

	//bCopySuccess&=CopyHeaderFiles();

	//bCopySuccess&=CopyLibFiles();

	//if (bCopySuccess) MessageBox(TEXT("拷贝成功！"));
}

//模态窗口
INT_PTR CDlgSdkTools::DoModal()
{
	//if (!LoadConfig()) return false;
//	SetIcon(m_hIcon, TRUE);			// Set big icon
//	SetIcon(m_hIcon, FALSE);		// Set small icon
	//SetWindowText(TEXT("EricYonngSdkTools"));
	return CDialog::DoModal();
}


//读取配置文件
//配置目录没有的要创建 没有配置文件提示文件丢失
bool CDlgSdkTools::LoadConfig()
{
	bool bFileDestroy = false;
	TCHAR szWorkPlace[MAX_PATH] = { 0 };
	CWriteDailyLog::GetWorkDirectory(szWorkPlace, MAX_PATH);
	TCHAR szString[1024] = { 0 };

	//判断配置文件是否存在

		//文件存在读取配置
		CIniFileIO FileIni;
		FileIni.SetFilePath(TEXT(".\\SdkToolsConfig.ini"));
		

		//读取头文件
		FileIni.ReadString(TEXT("DEST_HEADER_FILES_PATH"), TEXT("path"), TEXT(""), szString, 1024);
		if (lstrlen(szString)==0)
		{
			memset(szString, 0, sizeof(szString));
			lstrcpyn(szString, TEXT("..\\..\\开发环境\\include\\"), CountArray(TEXT("..\\..\\开发环境\\include\\")));
			FileIni.WriteString(TEXT("DEST_HEADER_FILES_PATH"), TEXT("path"), szString, TEXT(".\\SdkToolsConfig.ini"));
		}

		CString strPath = szWorkPlace;
		strPath += "\\";
		strPath += szString;
		GetDlgItem(IDC_HEADER_PATH_EDIT)->SetWindowText(strPath);

		strPath.Empty();
		memset(szString, 0, sizeof(szString));

		//读取lib文件
		FileIni.ReadString(TEXT("DEST_LIB_FILES_PATH"), TEXT("path"), TEXT(""), szString, 1024);
		if (lstrlen(szString) == 0)
		{
			memset(szString, 0, sizeof(szString));
			lstrcpyn(szString, TEXT("..\\..\\开发环境\\lib\\"), CountArray(TEXT("..\\..\\开发环境\\lib\\")));
			FileIni.WriteString(TEXT("DEST_LIB_FILES_PATH"), TEXT("path"), szString, TEXT(".\\SdkToolsConfig.ini"));
		}
		strPath = szWorkPlace;
		strPath += "\\";
		strPath += szString;
		GetDlgItem(IDC_LIB_PATH_EDIT)->SetWindowText(strPath);

	return true;
}

//读取开发库头文件目录
void CDlgSdkTools::ReadHeaderFiles()
{
	TCHAR szString[1024] = { 0 };

	//文件存在读取配置
	CIniFileIO FileIni;
	FileIni.SetFilePath(TEXT(".\\SdkToolsConfig.ini"));

	//读取头文件
	FileIni.ReadString(TEXT("SRC_SDK_LIST_FILE"), TEXT("HeaderListFile"), TEXT(""), szString, 1024);
	if (lstrlen(szString) == 0)
	{
		memset(szString, 0, sizeof(szString));
		lstrcpyn(szString, TEXT(".\\SdkHeaderList.txt"), CountArray(TEXT(".\\SdkHeaderList.txt")));
		FileIni.WriteString(TEXT("SRC_SDK_LIST_FILE"), TEXT("HeaderListFile"), szString, TEXT(".\\SdkToolsConfig.ini"));
	}
	char szFileString[1024] = { 0 };
	CWriteDailyLog::UnicodeToAnsi(szString, szFileString, sizeof(szFileString));

	{
		TCHAR szUnicodeLine[1024] = { 0 };
		char szCacheLine[1024] = { 0 };
		long lLine = 0;
		FILE *fp = NULL;
		while (true)
		{
			memset(szCacheLine, 0, sizeof(szCacheLine));
			memset(szUnicodeLine, 0, sizeof(szUnicodeLine));
			if (CWriteDailyLog::ReadOneLine(szCacheLine, sizeof(szCacheLine), szFileString, &fp) > 0)
			{
				CWriteDailyLog::AnsiToUnicode(szCacheLine, szUnicodeLine, sizeof(szUnicodeLine));
				m_HeaderFileView.DispMsg(szUnicodeLine, MSG_COLOR_YELLOW, 0);
				m_HeaderFileView.DispMsg(TEXT("\r\n"));
				//m_HeaderFileView.DispMsg( szUnicodeLine);
			}
			else
			{
				//判断文件是否结尾
				if (fp)
				{
					if (feof(fp)) break;
				}
				m_HeaderFileView.DispMsg(TEXT("\r\n"));
			}
			lLine++;
		}	
		if (fp) fclose(fp);
	}

	//m_HeaderFileView.GetLineCount();//获取行数
	//m_HeaderFileView.LineIndex(i);	//行首的索引
	//m_HeaderFileView.LineLength();	//一行字符串长度
	//m_HeaderFileView.SetSel(1, 1);
	//m_HeaderFileView.GetSelText();

}

//读取开发库lib文件目录
void CDlgSdkTools::ReadLibraryFiles()
{
	TCHAR szString[1024] = { 0 };

	//文件存在读取配置
	CIniFileIO FileIni;
	FileIni.SetFilePath(TEXT(".\\SdkToolsConfig.ini"));

	//读取头文件
	FileIni.ReadString(TEXT("SRC_SDK_LIST_FILE"), TEXT("LibListFile"), TEXT(""), szString, 1024);
	if (lstrlen(szString) == 0)
	{
		memset(szString, 0, sizeof(szString));
		lstrcpyn(szString, TEXT(".\\SdkLibList.txt"), CountArray(TEXT(".\\SdkLibList.txt")));
		FileIni.WriteString(TEXT("SRC_SDK_LIST_FILE"), TEXT("LibListFile"), szString, TEXT(".\\SdkToolsConfig.ini"));
	}
	char szFileString[1024] = { 0 };
	CWriteDailyLog::UnicodeToAnsi(szString, szFileString, sizeof(szFileString));

	{
		TCHAR szUnicodeLine[1024] = { 0 };
		char szCacheLine[1024] = { 0 };
		long lLine = 0;
		FILE *fp = NULL;
		while (true)
		{
			memset(szCacheLine, 0, sizeof(szCacheLine));
			memset(szUnicodeLine, 0, sizeof(szUnicodeLine));
			if (CWriteDailyLog::ReadOneLine(szCacheLine, sizeof(szCacheLine), szFileString, &fp) > 0)
			{
				CWriteDailyLog::AnsiToUnicode(szCacheLine, szUnicodeLine, sizeof(szUnicodeLine));
				m_LibFileView.DispMsg(szUnicodeLine, MSG_COLOR_YELLOW, 0);
				m_LibFileView.DispMsg(TEXT("\r\n"));
				//m_HeaderFileView.DispMsg( szUnicodeLine);
			}
			else
			{
				//判断文件是否结尾
				if (fp)
				{
					if (feof(fp)) break;
				}

				m_LibFileView.DispMsg(TEXT("\r\n"));
			}
			lLine++;
		}
		if (fp) fclose(fp);
	}
}

//拷贝头文件
bool CDlgSdkTools::CopyHeaderFiles()
{
	//m_HeaderFileView.GetLineCount();//获取行数
	//m_HeaderFileView.LineIndex(i);	//行首的索引
	//m_HeaderFileView.LineLength();	//一行字符串长度
	//m_HeaderFileView.SetSel(1, 1);
	//m_HeaderFileView.GetSelText();

	//逐行拷贝
	INT nLineCount = m_HeaderFileView.GetLineCount();
	nLineCount--;	//最后一行空行

	int iIndex = 0, iLengthLine=0, iMasterDirPos=0;
	CString strLineText = TEXT(""), strDestFile;
	char szDestPath[1024] = {0};
	string strDest, strSrcFileName, strFilter, strMasterDir, strSrcPathCombine;
	GetDlgItem(IDC_HEADER_PATH_EDIT)->GetWindowText(strDestFile);
	CWriteDailyLog::UnicodeToAnsi(strDestFile.GetBuffer(), szDestPath, sizeof(szDestPath));
	strDest = szDestPath;


	
	char szLineText[1024] = { 0 };

	//获取richedit每一行文本
	int i = 0, iCountFile = 0, iFilterCount=0;
	for (i = 0; i < nLineCount; i++)
	{
		strLineText.Empty();
		strDest.clear();
		strDest = szDestPath;
		strSrcFileName.clear();
		memset(szLineText, 0, sizeof(szLineText));
		//取出单行路径进行拷贝
		iIndex=m_HeaderFileView.LineIndex(i);		//获取行索引
		iLengthLine = m_HeaderFileView.LineLength(iIndex);	//获取行长度

		//空行过滤
		if (iLengthLine == 0)
		{
			iFilterCount++;
			continue;
		}

		m_HeaderFileView.GetLine(i, strLineText.GetBufferSetLength(iLengthLine + 1), iLengthLine);	//获取行字符串
		strLineText.SetAt(iLengthLine, TEXT('\0')); // null terminate	//0结尾字符串
		CWriteDailyLog::UnicodeToAnsi(strLineText.GetBuffer(), szLineText, sizeof(szLineText));	//转换成ansi
		strLineText.ReleaseBuffer(iLengthLine + 1);

		//注释过滤
		strFilter.clear();
		strFilter = szLineText;
		if (strFilter.find('#', 0) != string::npos)
		{
			iFilterCount++;
			continue;
		}

		//提取主目录
		if ((iMasterDirPos=strFilter.find(';', 0)) != string::npos)
		{
			iFilterCount++;
			strMasterDir.clear();
			strMasterDir = strFilter.substr(iMasterDirPos + strlen(";"), strFilter.length() - (iMasterDirPos + strlen(";")));
			continue;
		}

	
		
		//组合目的
		strDest += szLineText;

		//组合源
		strSrcPathCombine.clear();
		strSrcPathCombine = strMasterDir;
		strSrcPathCombine += szLineText;

		//创建文件夹
		CWriteDailyLog::CreateRecursiveDir(strDest);

		//进度
		DrawProgress("头文件", szLineText, iCountFile, nLineCount - iFilterCount);

		//拷贝文件
		if (!CWriteDailyLog::CopyFIle(strSrcPathCombine, strDest))
		{
			TCHAR szSrcErr[1024] = { 0 }, szDestErr[1024] = { 0 };
			CWriteDailyLog::AnsiToUnicode((char *)strSrcPathCombine.c_str(), szSrcErr, sizeof(szSrcErr));
			CWriteDailyLog::AnsiToUnicode((char *)strDest.c_str(), szDestErr, sizeof(szDestErr));
			//拷贝出错
			CString strErr;
			strErr.Format(TEXT("拷贝出错!\r\n源：%s\r\n目标：%s"), szSrcErr, szDestErr);

			MessageBox(strErr);
			return false;
		}
		iCountFile++;
	}

	return true;

}

//拷贝lib文件
bool CDlgSdkTools::CopyLibFiles()
{
	//m_HeaderFileView.GetLineCount();//获取行数
	//m_HeaderFileView.LineIndex(i);	//行首的索引
	//m_HeaderFileView.LineLength();	//一行字符串长度
	//m_HeaderFileView.SetSel(1, 1);
	//m_HeaderFileView.GetSelText();

	//逐行拷贝
	INT nLineCount = m_LibFileView.GetLineCount();
	nLineCount--;	//最后一行空行

	int iIndex = 0, iLengthLine = 0, iMasterDirPos = 0;
	CString strLineText = TEXT(""), strDestFile;
	char szDestPath[1024] = { 0 };
	string strDest, strSrcFileName, strFilter, strMasterDir, strSrcPathCombine;
	GetDlgItem(IDC_LIB_PATH_EDIT)->GetWindowText(strDestFile);
	CWriteDailyLog::UnicodeToAnsi(strDestFile.GetBuffer(), szDestPath, sizeof(szDestPath));
	strDest = szDestPath;



	char szLineText[1024] = { 0 };

	//获取richedit每一行文本
	int i = 0, iCountFile=0, iFilterCount=0;
	for (i = 0; i < nLineCount; i++)
	{
		strLineText.Empty();
		strDest.clear();
		strDest = szDestPath;
		strSrcFileName.clear();
		memset(szLineText, 0, sizeof(szLineText));
		//取出单行路径进行拷贝
		iIndex = m_LibFileView.LineIndex(i);		//获取行索引
		iLengthLine = m_LibFileView.LineLength(iIndex);	//获取行长度

		//空行过滤
		if (iLengthLine == 0)
		{
			iFilterCount++;
			continue;
		}

		m_LibFileView.GetLine(i, strLineText.GetBufferSetLength(iLengthLine + 1), iLengthLine);	//获取行字符串
		strLineText.SetAt(iLengthLine, TEXT('\0')); // null terminate	//0结尾字符串
		CWriteDailyLog::UnicodeToAnsi(strLineText.GetBuffer(), szLineText, sizeof(szLineText));	//转换成ansi
		strLineText.ReleaseBuffer(iLengthLine + 1);

		//注释过滤
		strFilter.clear();
		strFilter = szLineText;
		if (strFilter.find('#', 0) != string::npos)
		{
			iFilterCount++;
			continue;
		}

		//提取主目录
		if ((iMasterDirPos = strFilter.find(';', 0)) != string::npos)
		{
			iFilterCount++;
			strMasterDir.clear();
			strMasterDir = strFilter.substr(iMasterDirPos + strlen(";"), strFilter.length() - (iMasterDirPos + strlen(";")));
			continue;
		}



		//组合目的
		strDest += szLineText;

		//组合源
		strSrcPathCombine.clear();
		strSrcPathCombine = strMasterDir;
		strSrcPathCombine += szLineText;

		//创建文件夹
		CWriteDailyLog::CreateRecursiveDir(strDest);



		//拷贝文件
		if (!CWriteDailyLog::CopyFIle(strSrcPathCombine, strDest))
		{
			TCHAR szSrcErr[1024] = { 0 }, szDestErr[1024] = { 0 };
			CWriteDailyLog::AnsiToUnicode((char *)strSrcPathCombine.c_str(), szSrcErr, sizeof(szSrcErr));
			CWriteDailyLog::AnsiToUnicode((char *)strDest.c_str(), szDestErr, sizeof(szDestErr));
			//拷贝出错
			CString strErr;
			strErr.Format(TEXT("拷贝出错!\r\n源：%s\r\n目标：%s"), szSrcErr, szDestErr);

			MessageBox(strErr);
			return false;
		}

		iCountFile++;

		//进度
		DrawProgress("库文件", szLineText, iCountFile, nLineCount - iFilterCount);
	}

	return true;
}

//绘画进度
void CDlgSdkTools::DrawProgress(string strFileType, string strFileName, long lCurrentCount, long lTotalNum)
{
	CClientDC dc(this);
	CDC *pDC = CDC::FromHandle(dc.GetSafeHdc());
	HGDIOBJ pOld=pDC->SelectObject(m_TextFont);
	//绘画名字

	CRect rcClient, rcType, rcFileText, rcProgress, rcRange;

	GetClientRect(&rcClient);
	rcType.SetRect(rcClient.left + 40-4, rcClient.top + 4+206+12, rcClient.right - 40-4, rcClient.top + 24+206+12);
	rcFileText.SetRect(rcType.left, rcType.bottom + 4-5, rcType.right, rcType.bottom + 24-5);
	rcProgress.SetRect(rcFileText.left, rcFileText.bottom + 4-10, rcFileText.right, rcFileText.bottom + 24-10);
	rcRange.SetRect(rcType.left, rcType.top, rcType.right, rcProgress.bottom + 14);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->FillSolidRect(&rcRange, RGB(0xf0, 0xf0, 0xf0));
	
	//类型
	TCHAR szString[1024] = { 0 };
	memset(szString, 0, sizeof(szString));
	CWriteDailyLog::AnsiToUnicode((char *)strFileType.c_str(), szString, sizeof(szString));	
	pDC->DrawText(szString, lstrlen(szString), &rcType, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_END_ELLIPSIS);

	//文件名
	memset(szString, 0, sizeof(szString));
	CWriteDailyLog::AnsiToUnicode((char *)strFileName.c_str(), szString, sizeof(szString));
	pDC->DrawText(szString, lstrlen(szString), &rcFileText, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_END_ELLIPSIS);

	//进度
	memset(szString, 0, sizeof(szString));
	double lfProgress = (double)lCurrentCount / ((double)lTotalNum);
	_sntprintf(szString, CountArray(szString), TEXT("进度:%.3lf%% 总数:%ld"), lfProgress * 100, lTotalNum);
	rcProgress.SetRect(rcFileText.left, rcFileText.bottom , rcFileText.right, rcFileText.bottom + 20);
	pDC->DrawText(szString, lstrlen(szString), &rcProgress, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_END_ELLIPSIS);
	rcProgress.OffsetRect(0, 20);
	rcProgress.bottom = rcProgress.top + 14;
	pDC->FillSolidRect(&rcProgress, RGB(255, 255, 255));
	rcProgress.SetRect(rcFileText.left, rcFileText.bottom + 20, fabs(rcFileText.Width() *lfProgress) + rcFileText.left, rcFileText.bottom+40);
	rcProgress.bottom = rcProgress.top + 14;
	pDC->FillSolidRect(&rcProgress, RGB(0, 255, 0));

	pDC->SelectObject(pOld);
}

void CDlgSdkTools::OnEnChangeHeaderEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CDlgSdkTools::OnBnClickedRefresh()
{
	//清除RichEdit
	HWND hEditWnd = GetDlgItem(IDC_HEADER_EDIT)->GetSafeHwnd();
	::SendMessage(hEditWnd, WM_SETTEXT, (WPARAM)0, (LPARAM)"");
	hEditWnd = GetDlgItem(IDC_LIB_EDIT)->GetSafeHwnd();
	::SendMessage(hEditWnd, WM_SETTEXT, (WPARAM)0, (LPARAM)"");

//	m_LibFileView.SetSel(0, -1);
//	m_LibFileView.Clear();
//	m_HeaderFileView.SetSel(0, -1);
//	m_HeaderFileView.Clear();
	ReadHeaderFiles();

	ReadLibraryFiles();

	// TODO:  在此添加控件通知处理程序代码
}


void CDlgSdkTools::OnBnClickedAbout()
{
	// TODO:  在此添加控件通知处理程序代码
	CAboutDlg dlg;
	dlg.DoModal();
}
