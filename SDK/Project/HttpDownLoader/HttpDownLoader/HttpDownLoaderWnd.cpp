#include"stdafx.h"
#include"HttpDownLoaderWnd.h"
//#include"ThreadHead.h"
#include"Task.h"


//监听定时器
#define IDI_TIMER_DOWNLOADER		1000

//定时间隔
#define TIMER_ESCAPE				50

#define IDI_SCAN_CODE				1001
#define TIME_ESCAPE_SCAN_CODE		1000
///////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgPopUp, CDialog)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_NCDESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//弹窗
CDlgPopUp::CDlgPopUp(CWnd *pParrent) :CDialog(IDD_DIALOG2, pParrent)
{
	m_TextFont.CreateFont(12, 0, 0, 0, 400, 0, 0, 0, GB2312_CHARSET, OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("宋体"));

	//HBITMAP hBit;
	//CBitmap BitImage;
	//BitImage.CreateCompatibleBitmap(GetDC(), 470, 470);
	//BitImage.SetBitmapBits
	m_tagCodeBitImage.bmBits = NULL;
	m_tagCodeBitImage.bmBitsPixel = 8;
	m_tagCodeBitImage.bmHeight = 470;
	m_tagCodeBitImage.bmPlanes = 256;//颜色数量
	m_tagCodeBitImage.bmType = 0;
	m_tagCodeBitImage.bmWidth = 470;
	m_tagCodeBitImage.bmWidthBytes = (m_tagCodeBitImage.bmBitsPixel*m_tagCodeBitImage.bmWidth + 15) / 16 * 2;

	m_pXimageStream = NULL;
	m_pImageBufferCache = NULL;
	m_ulBufferCacheSize = 0;
	m_pScanStatusBufferCache = NULL;
	m_ulScanStatusCacheSize = 0;
	m_pXimageScanStatus = NULL;
	m_bScanSuccessResult = false;
	m_pbIsFinishMonitor = NULL;
	m_iCount = 5;


}

CDlgPopUp::~CDlgPopUp() 
{
	ReleaseImageCacheBuffer();
	ReleaseScanStatusCacheBuffer();
}

void CDlgPopUp::OnPaint()
{
	CPaintDC tDC(this);
	CDC *pDC = CDC::FromHandle(tDC.GetSafeHdc());
	CRect rcClient, rcCodeSize;
	GetClientRect(&rcClient);
	pDC->SelectObject(m_TextFont);

	//背景
	pDC->FillSolidRect(&rcClient, RGB(255, 255, 255));

	if (!m_PngBack.IsNull())
	{
		m_PngBack.DrawImage(pDC, 0, 0, 235, 235, 0, 0, m_PngBack.GetWidth(), m_PngBack.GetHeight());
	}	

	//绘制内存图
	//if (!m_strCodeBitImageStream.empty())
	{
		//if (!m_ImageBuffer.IsNull())
		//{
		//	m_ImageBuffer.Draw(pDC->GetSafeHdc(), 10, 10);
		//}
	//	DrawMemImageStream(pDC);
	}

	//绘制内存图
	if (m_pXimageStream)
	{
		rcCodeSize.SetRect(0, 0, m_pXimageStream->GetWidth(), m_pXimageStream->GetHeight());
		m_pXimageStream->Draw(pDC->GetSafeHdc(), 0, 0, 235, 235, &rcCodeSize);
	}

	//绘制扫描状态
	//if (m_pXimageScanStatus)
	//{
	//	rcCodeSize.SetRect(0, 45, 38, 40);
	//	m_pXimageScanStatus->Draw(pDC->GetSafeHdc(), 0, 240, 38, 40, &rcCodeSize);
	//	CxImage Png(TEXT("Status.png"), CXIMAGE_FORMAT_PNG);
//		Png.Draw(pDC->GetSafeHdc(), 0, 240, 38, 40, &rcCodeSize);
	//}

	if (m_bScanSuccessResult)
	{
		//m_PngScanStatus.DrawImage(pDC, 0, 0);
		m_PngScanStatus.DrawImage(pDC, 0, 240, 38, 40, 0, 45, 38, 40);
		rcCodeSize.SetRect(40, 240, 100, 280);
		//pDC->DrawText(TEXT("扫描成功"),lstrlen(TEXT("扫描成功")), &rcCodeSize, DT_LEFT | DT_WORDBREAK | DT_END_ELLIPSIS);	
		TCHAR TimeOut[32] = { 0 };
		_sntprintf(TimeOut, 32, TEXT("扫描成功，倒计时：%d"), m_iCount);

		pDC->DrawText(TimeOut, lstrlen(TimeOut), &rcCodeSize, DT_LEFT | DT_WORDBREAK | DT_END_ELLIPSIS);
	}
	else
	{
		rcCodeSize.SetRect(40, 240, 200, 280);
		pDC->DrawText(TEXT("请使用微信扫描二维码登陆"), lstrlen(TEXT("请使用微信扫描二维码登陆")), &rcCodeSize, DT_LEFT | DT_WORDBREAK | DT_END_ELLIPSIS);
		rcCodeSize.SetRect(40, 260, 200, 280);
		pDC->DrawText(TEXT("\"7879game\""), lstrlen(TEXT("\"7879game\"")), &rcCodeSize, DT_CENTER | DT_WORDBREAK | DT_END_ELLIPSIS);
	}

	//CPngImage PngStatus;
	//PngStatus.LoadImage(TEXT("Status.png"));
	//if (!PngStatus.IsNull())
	//{
	//	PngStatus.DrawImage(pDC, 0, 240, 38, 40, 0, 45, 38, 40);
	//}

	//文字
	if (m_strViewText.GetLength() != 0)
	{
		
		pDC->DrawText(m_strViewText, &rcClient, DT_CALCRECT | DT_LEFT | DT_WORDBREAK | DT_END_ELLIPSIS);	//计算字符串宽度
		pDC->DrawText(m_strViewText, &rcClient, DT_LEFT | DT_WORDBREAK | DT_END_ELLIPSIS);	//计算标题字符串宽度
		//pDC->DrawText(m_strViewText, &rcClient, 0);
		//pDC->SelectObject(hObj);
	}

	__super::OnPaint();
}

//创建消息
int CDlgPopUp::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)return -1;

	m_PngScanStatus.LoadImage(AfxGetInstanceHandle(), TEXT("SCAN_STATUS"));

	if (!m_PngBack.IsNull())
	{
		SetWindowPos(NULL, 0, 0, m_PngBack.GetWidth() + 50, m_PngBack.GetHeight() + 50, SWP_NOMOVE);
	}
	else
	{
		SetWindowPos(NULL, 0, 0,500, 500, SWP_NOMOVE);
	}

	//内存图像流资源
	m_hImageGlobal=NULL;		//句柄
	m_pStream=NULL;			//流

	return 0;
}

//关闭窗口
void CDlgPopUp::OnNcDestroy()
{
	if (!m_PngBack.IsNull())
	{
		m_PngBack.DestroyImage();
	}

	m_strViewText.Empty();

	if (m_pbIsFinishMonitor)
		*m_pbIsFinishMonitor = true;

	__super::OnNcDestroy();
}

//加载背景
bool CDlgPopUp::LoadBackImg(LPTSTR lpStrBack)
{
	if (!m_PngBack.IsNull()) m_PngBack.DestroyImage();

	m_PngBack.LoadImage(lpStrBack);

	if (!m_PngBack.IsNull())
	{
		SetWindowPos(NULL, 0, 0, m_PngBack.GetWidth() + 50, m_PngBack.GetHeight() + 50, SWP_NOMOVE);
	}

	return true;
}

//重置位图
bool CDlgPopUp::RefreshCodeBitmap(void *bmBits, long bmType, long bmWidth, long bmHeight, unsigned short bmPlanes, unsigned short bmBitsPixel)
{
	m_tagCodeBitImage.bmBits = bmBits;
	m_tagCodeBitImage.bmBitsPixel = bmBitsPixel;
	m_tagCodeBitImage.bmHeight = bmHeight;
	m_tagCodeBitImage.bmPlanes = bmPlanes;
	m_tagCodeBitImage.bmType = bmType;
	m_tagCodeBitImage.bmWidth = bmWidth;
	m_tagCodeBitImage.bmWidthBytes = (m_tagCodeBitImage.bmBitsPixel*m_tagCodeBitImage.bmWidth + 15) / 16 * 2;

	return true;
}

//内存位图绘制
bool CDlgPopUp::DrawMemImageStream(CDC *pDC)
{
	int iWidth, iHeight, iBytesWidth;
	iWidth = 256;
	iHeight = 256;
	double  arry[256][256] = {0};//存放的是数据
	char *pImageStream = (char *)m_strCodeBitImageStream.c_str();	//图像流数据
	//CImage ImageStream;
	//ImageStream.Load((char *)m_strCodeBitImageStream.c_str());

	iBytesWidth = (iWidth * 24 + 31) / 32 * 4;
	HANDLE hMem = GlobalAlloc(GMEM_MOVEABLE, iBytesWidth * iHeight * 3);	//申请内存
	BYTE *Image = (BYTE*)GlobalLock(hMem);	//锁住内存

	for (int i = 0; i<470; i++)
	for (int j = 0; j<470; j++)
	{
		//*(Image + i*iBytesWidth + j * 3) = int(255 *  (*(char *)(pImageStream+j*470+i)) );//
		//*(Image + 1 + i*iBytesWidth + j * 3) = int(255 * (*(char *)(pImageStream + j * 470 + i)));
		//*(Image + 2 + i*iBytesWidth + j * 3) = int(255 * (*(char *)(pImageStream + j * 470 + i)));
		*(Image + i*iBytesWidth + j * 3) = int(255 * arry[i][j]);//
		*(Image + 1 + i*iBytesWidth + j * 3) = int(255 *arry[i][j]);
		*(Image + 2 + i*iBytesWidth + j * 3) = int(255 * arry[i][j]);
	}

	BITMAPINFO bmi;
	ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = iWidth;
	bmi.bmiHeader.biHeight = iHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;

	CRect rect;
	GetClientRect(rect);
	pDC->SetStretchBltMode(COLORONCOLOR);
	::StretchDIBits(pDC->GetSafeHdc(), 0, 0, iWidth, iHeight, 0, 0, 0, iHeight, Image, &bmi, DIB_RGB_COLORS,
		SRCCOPY);

	GlobalUnlock(hMem);
	GlobalFree(hMem);

	return true;
}

//加载内存图
bool  CDlgPopUp::LoadMemImage(void *  pMemData)
{
	if (pMemData)
	{
		m_pXimageStream = (CxImage *)pMemData;
	}

	return true;
}

//清除内存缓存
void CDlgPopUp::ReleaseImageCacheBuffer()
{
	try
	{
		if (m_pXimageStream)
		{
			m_pXimageStream->Destroy();
			delete m_pXimageStream;
			m_pXimageStream = NULL;
		}

		if (m_pImageBufferCache)
		{
			delete[]m_pImageBufferCache;
			m_pImageBufferCache = NULL;
			m_ulBufferCacheSize = 0;
		}
	}
	catch (...)
	{
		m_pImageBufferCache = NULL;
		m_ulBufferCacheSize = 0;
		m_pXimageStream = NULL;
	}
}

//清除内存缓存
void CDlgPopUp::ReleaseScanStatusCacheBuffer()
{
	try
	{
		if (m_pXimageScanStatus)
		{
			m_pXimageScanStatus->Destroy();
			delete m_pXimageScanStatus;
			m_pXimageScanStatus = NULL;
		}

		if (m_pScanStatusBufferCache)
		{
			delete[]m_pScanStatusBufferCache;
			m_pScanStatusBufferCache = NULL;
			m_ulScanStatusCacheSize = 0;
		}
	}
	catch (...)
	{
		m_pScanStatusBufferCache = NULL;
		m_ulScanStatusCacheSize = 0;
		m_pXimageScanStatus = NULL;
	}
}

//扫描成功
void CDlgPopUp::SetScanResult(bool bSuccess , bool *pbFinishMonitor)
{
	m_pbIsFinishMonitor = pbFinishMonitor;
	m_iCount = 10;
	SetTimer(IDI_SCAN_CODE, TIME_ESCAPE_SCAN_CODE, NULL);
	bool bRet = m_bScanSuccessResult;
	m_bScanSuccessResult = bSuccess;
	if (bRet != m_bScanSuccessResult)  RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);
}

void CDlgPopUp::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_SCAN_CODE:
	{
					  
						  //更新窗口
						  if (m_hWnd != NULL)
						  {
							  RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);
						  }

						  m_iCount--;

						  if (m_iCount <= 0)
						  {
							  KillTimer(nIDEvent);
							  if (m_pbIsFinishMonitor)
								  *m_pbIsFinishMonitor = true;
							  SendMessage(WM_CLOSE);
						  }


						  break;
	}
	}
}

//加载内存图
bool  CDlgPopUp::LoadMemImage(void *  pMemData, long  len, unsigned long ulImageType)
{ 
	ReleaseImageCacheBuffer();

	//m_pImageBufferCache = new char[len];
	//memset(m_pImageBufferCache, 0, len);
	//memmove(m_pImageBufferCache, pMemData, len);
	//m_ulBufferCacheSize = len;


	m_pXimageStream = new  CxImage((BYTE *)pMemData, len, ulImageType);
	/*if (!m_ImageBuffer.IsNull()) m_ImageBuffer.Destroy();

	//申请内存并锁住内存
	if (m_hImageGlobal)
	{
		GlobalFree(m_hImageGlobal);
		m_hImageGlobal = NULL;
	}

	m_hImageGlobal = GlobalAlloc(GMEM_MOVEABLE, len);
	void *  pData = GlobalLock(m_hImageGlobal);

	//拷贝数据
	memmove(pData, pMemData, len);

	//解锁
	GlobalUnlock(m_hImageGlobal);

	if (m_pStream)
	{
		m_pStream->Release();
		m_pStream = NULL;
	}

	//内存中创建流
	if (CreateStreamOnHGlobal(m_hImageGlobal, FALSE, &m_pStream) == S_OK)
	{

		if (SUCCEEDED(m_ImageBuffer.Load(m_pStream)))
		{
			//   .. 
			return true;
		}
		else
		{
			return false;
		}

		//释放
		//pStream->Release();
	}
	else
	{
		return false;
	}

	return true;

	CxImage

	//释放内存
	//GlobalFree(hGlobal);*/

	return true;

}

//加载扫描结果状态图
bool  CDlgPopUp::LoadScanStatusMemImage(void *  pMemData, long  len, unsigned long ulImageType)
{
	ReleaseScanStatusCacheBuffer();

	//m_pImageBufferCache = new char[len];
	//memset(m_pImageBufferCache, 0, len);
	//memmove(m_pImageBufferCache, pMemData, len);
	//m_ulBufferCacheSize = len;


	m_pXimageScanStatus = new  CxImage((BYTE *)pMemData, len, ulImageType);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////

CHttpDownLoaderDlg::CHttpDownLoaderDlg(CWnd* pParent /*=NULL*/)
: CDialog(CHttpDownLoaderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

CHttpDownLoaderDlg::~CHttpDownLoaderDlg()
{
	KillTimer(IDI_TIMER_DOWNLOADER);
}

void CHttpDownLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListenActiveDlg)
	DDX_Control(pDX, IDC_BT_DOWNLOAD, m_BtnDownLoad);
	DDX_Control(pDX, IDC_EDIT_FILE_LINK, m_EditFileLink);
	DDX_Control(pDX, IDC_EDIT_FILE_NAME, m_EditFileName);
	DDX_Control(pDX, IDC_EDIT_SAVE_PATH, m_EditSavePath);
	DDX_Control(pDX, IDC_EDIT_PORT, m_EditPort);
	DDX_Control(pDX, IDC_CHECK_SAVE_HEADER, m_CheckHttpHeader);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHttpDownLoaderDlg, CDialog)
	//{{AFX_MSG_MAP(CListenActiveDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()

	ON_BN_CLICKED(IDC_BT_DOWNLOAD, &CHttpDownLoaderDlg::OnBnClickedDownLoad)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListenActiveDlg message handlers

BOOL CHttpDownLoaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon
	SetWindowText(TEXT("http协议下载器"));
	m_EditPort.SetWindowText(TEXT("80"));
	m_EditSavePath.SetWindowText(TEXT(".\\\\"));
	//m_EditFileLink.SetWindowText(TEXT("http://www.7879game.com/IntApi/wx.aspx"));
	//m_EditFileName.SetWindowText(TEXT("dafasjklj.html"));
	m_CheckHttpHeader.SetCheck(TRUE);
	//m_PopupWnd.Create(IDD_DIALOG2, this);
	CRect rcRect(0, 0, 0, 0);
	GetWindowRect(&rcRect);
	//m_PopupWnd.SetWindowPos(NULL, rcRect.left, rcRect.top, 0, 0,  SWP_NOSIZE | SWP_HIDEWINDOW);

	//m_PopupPersonalInfo.Create(IDD_DIALOG2, this);
	//m_PopupPersonalInfo.SetWindowPos(NULL, rcRect.left, rcRect.top, 500, 500,  SWP_HIDEWINDOW);
	
	m_rcFlowCountArea.SetRect(10, rcRect.Height() - 70, rcRect.Width() - 20, rcRect.Height() - 30);
	m_lCurrentDataFlow = 0;
	m_lTotalDataSize = 0;
	m_lfDataSpeed = 0;
	m_TextFont.CreateFont(12, 0, 0, 0, 400, 0, 0, 0, GB2312_CHARSET, OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("宋体"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CHttpDownLoaderDlg::OnPaint()
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
		CPaintDC dc(this);
		dc.SelectObject(m_TextFont);
		TCHAR szText[512] = { 0 };
		_sntprintf(szText, 512, TEXT("文件名：%s\r\n 当前下载: %.3lf MB/%.3lf MB\r\n当前速度：%.3lf KB/s"), m_CurrentFileName.GetBuffer(), ((double)m_lCurrentDataFlow)/1024/1024, (double)m_lTotalDataSize/1024/1024, m_lfDataSpeed);
		dc.DrawText(szText, lstrlen(szText), &m_rcFlowCountArea, DT_BOTTOM | DT_CENTER | DT_END_ELLIPSIS);
		CDialog::OnPaint();
	}
}


void CHttpDownLoaderDlg::OnBnClickedDownLoad()
{
	CString strFileLink, strFileName, strSavePath, strPort;
	// TODO:  在此添加控件通知处理程序代码

	//获取连接地址
	m_EditFileLink.GetWindowText(strFileLink);

	//获取要保存的文件名
	m_EditFileName.GetWindowText(strFileName);
	m_CurrentFileName.Empty();
	m_CurrentFileName = strFileName;
	m_lfDataSpeed = 0;

	//获取保存路径
	m_EditSavePath.GetWindowText(strSavePath);

	//端口
	m_EditPort.GetWindowText(strPort);

	if (strFileLink.GetLength() == 0 || strFileName.GetLength() == 0 || strSavePath.GetLength() == 0 || strPort.GetLength()==0)
	{
		AfxMessageBox(TEXT("您提供的信息不完整，请检查后重试！"));
		return;
	}
	
	//字符转换
	char szLinkCache[8192] = { 0 };
	string strAnsiLink, strAnsiFile, strAnsiPath, strAnsiPort;
	memset(szLinkCache, 0, sizeof(szLinkCache));
	CWriteDailyLog::UnicodeToAnsi(strFileLink.GetBuffer(), szLinkCache, sizeof(szLinkCache));
	strAnsiLink = szLinkCache;

	memset(szLinkCache, 0, sizeof(szLinkCache));
	CWriteDailyLog::UnicodeToAnsi(strFileName.GetBuffer(), szLinkCache, sizeof(szLinkCache));
	strAnsiFile = szLinkCache;

	memset(szLinkCache, 0, sizeof(szLinkCache));
	CWriteDailyLog::UnicodeToAnsi(strSavePath.GetBuffer(), szLinkCache, sizeof(szLinkCache));
	strAnsiPath = szLinkCache;

	strAnsiPath += strAnsiFile;

	memset(szLinkCache, 0, sizeof(szLinkCache));
	CWriteDailyLog::UnicodeToAnsi(strPort.GetBuffer(), szLinkCache, sizeof(szLinkCache));
	strAnsiPort = szLinkCache;

	//添加下载任务
	CTaskBase *pDownLoadTask = new CHttpDownloadTask(strAnsiLink, atoi((char *)strAnsiPort.c_str()), strAnsiPath, m_CheckHttpHeader.GetCheck(),  this);
	CThreadPool::GetInstance()->AddTask(pDownLoadTask);

	//开启下载
	
	//BOOL bRet = FALSE;
//	bRet = m_httpdown.DownLoadFile((char *)strAnsiLink.c_str(), strAnsiLink.size(), (char *)strAnsiPath.c_str(), strAnsiPath.size(), atoi((char *)strAnsiPort.c_str()));

/*	CRect rcRect(0, 0, 0, 0);
	GetWindowRect(&rcRect);
	if (m_PopupWnd.m_hWnd&&IsWindow(m_PopupWnd.m_hWnd))
	{
		m_PopupWnd.Create(IDD_DIALOG2, this);
	}
	m_PopupWnd.SetWindowPos(NULL, rcRect.left, rcRect.top, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);
	if (m_PopupPersonalInfo.m_hWnd&&IsWindow(m_PopupPersonalInfo.m_hWnd))
	{
		m_PopupPersonalInfo.Create(IDD_DIALOG2, this);
	}
	m_PopupPersonalInfo.SetWindowPos(NULL, rcRect.left, rcRect.top, 500, 500, SWP_HIDEWINDOW);*/
	

	//http头信息
/*	if (m_CheckHttpHeader.GetCheck())
	{
		if (!m_httpdown.m_strHttpReturnHeader.empty())
		{
			strAnsiPath += ".header";
			CWriteDailyLog::SaveDataAsFile((char *)m_httpdown.m_strHttpReturnHeader.c_str(), m_httpdown.m_strHttpReturnHeader.size(), (char *)strAnsiPath.c_str());
		}
	}

	if (!bRet)
	{
		AfxMessageBox(TEXT("下载失败！"));
		return;
	}
	AfxMessageBox(TEXT("下载成功！"));*/

	//添加监视任务
	//CTaskBase *pListenDataTask = new CHttpListenTask(this);
	//GlobalThreadPool.AddTask(pListenDataTask);

	//api信息解析
	//CTaskBase *pListenDataTask = new CHttpWxApiAnalysisTask((char *)strAnsiLink.c_str(), this);
	//GlobalThreadPool.AddTask(pListenDataTask);

	//启动监听
	//SetTimer(IDI_TIMER_DOWNLOADER, TIMER_ESCAPE, NULL);


}

//定时器消息
void CHttpDownLoaderDlg::OnTimer(UINT nIDEvent)
{

/*	switch (nIDEvent)
	{
	case IDI_TIMER_DOWNLOADER:
	{
								 CString strFileLink, strFileName, strSavePath, strPort;
								 // TODO:  在此添加控件通知处理程序代码

								 //获取连接地址
								 m_EditFileLink.GetWindowText(strFileLink);

								 //获取要保存的文件名
								 m_EditFileName.GetWindowText(strFileName);

								 //获取保存路径
								 m_EditSavePath.GetWindowText(strSavePath);

								 //端口
								 m_EditPort.GetWindowText(strPort);

								 if (strFileLink.GetLength() == 0 || strFileName.GetLength() == 0 || strSavePath.GetLength() == 0 || strPort.GetLength() == 0)
								 {
									 AfxMessageBox(TEXT("您提供的信息不完整，请检查后重试！"));
									 return;
								 }

								 //字符转换
								 char szLinkCache[8192] = { 0 };
								 string strAnsiLink, strAnsiFile, strAnsiPath, strAnsiPort;
								 memset(szLinkCache, 0, sizeof(szLinkCache));
								 CWriteDailyLog::UnicodeToAnsi(strFileLink.GetBuffer(), szLinkCache, sizeof(szLinkCache));
								 strAnsiLink = szLinkCache;

								 memset(szLinkCache, 0, sizeof(szLinkCache));
								 CWriteDailyLog::UnicodeToAnsi(strFileName.GetBuffer(), szLinkCache, sizeof(szLinkCache));
								 strAnsiFile = szLinkCache;
								 char tmp[100] = { 0 };
								 static int idt = 0;
								 idt++;
								 sprintf(tmp, "%d", idt);
								 strAnsiFile += tmp;

								 memset(szLinkCache, 0, sizeof(szLinkCache));
								 CWriteDailyLog::UnicodeToAnsi(strSavePath.GetBuffer(), szLinkCache, sizeof(szLinkCache));
								 strAnsiPath = szLinkCache;

								 strAnsiPath += strAnsiFile;

								 memset(szLinkCache, 0, sizeof(szLinkCache));
								 CWriteDailyLog::UnicodeToAnsi(strPort.GetBuffer(), szLinkCache, sizeof(szLinkCache));
								 strAnsiPort = szLinkCache;

								 //开启下载
								 //缓存
								 char *pRecvData = NULL;
								 long lRecvDataLen = 0;

								BOOL bRet = m_httpdown.RecvDataFromServer((void **)&pRecvData, &lRecvDataLen);
								 //char rizhi1[100] = { 0 };
								 //sprintf(rizhi1, "get返回的状态码:%ld", m_HttpBackStatusCode);
								 //CWriteDailyLog::WriteLog(rizhi1, strlen(rizhi1), "返回状态码.txt");

								 //if (bRet)
								 //{
								 //	if (!m_strHttpReturnHeader.empty())
								 //	{
								 //		CWriteDailyLog::WriteLog((char *)m_strHttpReturnHeader.c_str(), m_strHttpReturnHeader.length(), "httpHeadInfo.txt");
								 //	}
								 //}


								 if (pRecvData)
								 {
									 //保存文件
									 CWriteDailyLog::SaveDataAsFile(pRecvData, lRecvDataLen, (char *)strAnsiPath.c_str());

									 //释放缓存
									 m_httpdown.ReleaseRecvDataOut();
								 }
								 else
								 {
									 //if (pRecvData) 			//下载失败
									 //	CWriteDailyLog::WriteLog("有数据", strlen("有数据"), "log.txt");
									 //释放缓存
									 m_httpdown.ReleaseRecvDataOut();

								 }

								 //http头信息
								 if (m_CheckHttpHeader.GetCheck())
								 {
									 if (!m_httpdown.m_strHttpReturnHeader.empty())
									 {
										 strAnsiPath += ".header";
										 CWriteDailyLog::SaveDataAsFile((char *)m_httpdown.m_strHttpReturnHeader.c_str(), m_httpdown.m_strHttpReturnHeader.size(), (char *)strAnsiPath.c_str());
										 m_httpdown.m_strHttpReturnHeader.clear();
									 }
								 }

								 break;
	}
	}

__super::OnTimer(nIDEvent);*/
}

//刷新流量
void CHttpDownLoaderDlg::RefreshFlowCount(long lCurrentFlow, long lTotalSize)
{
	m_lTotalDataSize = lTotalSize;
	m_lfDataSpeed = (abs((double)(m_lCurrentDataFlow - lCurrentFlow))/1024);
	m_lCurrentDataFlow = lCurrentFlow;
	InvalidateRect(&m_rcFlowCountArea, TRUE);
}