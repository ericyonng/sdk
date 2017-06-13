#ifndef HTTP_DOWNLOADER_WND_H_
#define HTTP_DOWNLOADER_WND_H_



#pragma once
#include"resource.h"

class CDlgPopUp :public CDialog
{
	//资源
public:
	CPngImage			m_PngBack;
	CPngImage			m_PngScanStatus;
	CString				m_strViewText;
	CFont				m_TextFont;
	BITMAP				m_tagCodeBitImage;
	string				m_strCodeBitImageStream;
	CImage				m_ImageBuffer;		//二维码图像缓存
	HGLOBAL				m_hImageGlobal;		//句柄
	IStream *			m_pStream;			//流

	CxImage *			m_pXimageStream;	//内存图
	char *				m_pImageBufferCache;	//数据缓存
	unsigned long		m_ulBufferCacheSize;	//缓存大小
	CxImage *			m_pXimageScanStatus;	//扫描结果状态图
	char *				m_pScanStatusBufferCache;	//扫描状态数据缓存
	unsigned long		m_ulScanStatusCacheSize;	//扫描状态缓存大小

	bool				m_bScanSuccessResult;		//扫描成功
	bool *				m_pbIsFinishMonitor;			//结束监听
	int					m_iCount;					//倒计时

public:
	CDlgPopUp(CWnd *pParrent = NULL);
	virtual ~CDlgPopUp();

public:
	//开始绘画
	void OnPaint();
	//创建消息
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//关闭窗口
	afx_msg void OnNcDestroy();

	//功能函数
public:
	//加载背景
	bool LoadBackImg(LPTSTR lpStrBack);

	//重置位图
	bool RefreshCodeBitmap(void *bmBits = NULL, long bmType = 0, long bmWidth = 470, long bmHeight = 470, unsigned short bmPlanes=256, unsigned short bmBitsPixel=8);
	//内存位图绘制
	bool DrawMemImageStream(CDC *pDC);
	//加载内存图
	bool  LoadMemImage(void *  pMemData, long  len, unsigned long ulImageType);
	//加载内存图
	bool  LoadMemImage(void *  pMemData);
	//加载扫描结果状态图
	bool  LoadScanStatusMemImage(void *  pMemData, long  len, unsigned long ulImageType);

	//清除内存缓存
	void ReleaseImageCacheBuffer();
	//清除内存缓存
	void ReleaseScanStatusCacheBuffer();

	//扫描成功
	void SetScanResult(bool bSuccess = true, bool *pbFinishMonitor=NULL);

	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()

};

class CHttpDownLoaderDlg : public CDialog
{
	// Construction
public:
	CHttpDownLoaderDlg(CWnd* pParent = NULL);	// standard constructor
	~CHttpDownLoaderDlg();
	enum { IDD = IDD_DIALOG1 };

	//资源
	CEdit					m_EditFileLink;
	CEdit					m_EditFileName;
	CEdit					m_EditSavePath;
	CEdit					m_EditPort;
	CButton					m_BtnDownLoad;
	CButton					m_CheckHttpHeader;
	HICON					m_hIcon;

	//流量统计
public:
	CRect					m_rcFlowCountArea;
	long					m_lCurrentDataFlow;
	long					m_lTotalDataSize;
	double					m_lfDataSpeed;
	CFont					m_TextFont;
	CString					m_CurrentFileName;
//	CHttpClient				m_httpdown;
//	CDlgPopUp				m_PopupWnd;
//	CDlgPopUp               m_PopupPersonalInfo;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// Generated message map functions
	//{{AFX_MSG(CListenActiveDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();


public:
	//定时器消息
	afx_msg void OnTimer(UINT nIDEvent);

	//功能函数
public:
	//刷新流量
	void RefreshFlowCount(long lCurrentFlow, long lTotalSize);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedDownLoad();


};


#endif