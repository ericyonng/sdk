#ifndef HTTP_DOWNLOADER_WND_H_
#define HTTP_DOWNLOADER_WND_H_



#pragma once
#include"resource.h"

class CDlgPopUp :public CDialog
{
	//��Դ
public:
	CPngImage			m_PngBack;
	CPngImage			m_PngScanStatus;
	CString				m_strViewText;
	CFont				m_TextFont;
	BITMAP				m_tagCodeBitImage;
	string				m_strCodeBitImageStream;
	CImage				m_ImageBuffer;		//��ά��ͼ�񻺴�
	HGLOBAL				m_hImageGlobal;		//���
	IStream *			m_pStream;			//��

	CxImage *			m_pXimageStream;	//�ڴ�ͼ
	char *				m_pImageBufferCache;	//���ݻ���
	unsigned long		m_ulBufferCacheSize;	//�����С
	CxImage *			m_pXimageScanStatus;	//ɨ����״̬ͼ
	char *				m_pScanStatusBufferCache;	//ɨ��״̬���ݻ���
	unsigned long		m_ulScanStatusCacheSize;	//ɨ��״̬�����С

	bool				m_bScanSuccessResult;		//ɨ��ɹ�
	bool *				m_pbIsFinishMonitor;			//��������
	int					m_iCount;					//����ʱ

public:
	CDlgPopUp(CWnd *pParrent = NULL);
	virtual ~CDlgPopUp();

public:
	//��ʼ�滭
	void OnPaint();
	//������Ϣ
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�رմ���
	afx_msg void OnNcDestroy();

	//���ܺ���
public:
	//���ر���
	bool LoadBackImg(LPTSTR lpStrBack);

	//����λͼ
	bool RefreshCodeBitmap(void *bmBits = NULL, long bmType = 0, long bmWidth = 470, long bmHeight = 470, unsigned short bmPlanes=256, unsigned short bmBitsPixel=8);
	//�ڴ�λͼ����
	bool DrawMemImageStream(CDC *pDC);
	//�����ڴ�ͼ
	bool  LoadMemImage(void *  pMemData, long  len, unsigned long ulImageType);
	//�����ڴ�ͼ
	bool  LoadMemImage(void *  pMemData);
	//����ɨ����״̬ͼ
	bool  LoadScanStatusMemImage(void *  pMemData, long  len, unsigned long ulImageType);

	//����ڴ滺��
	void ReleaseImageCacheBuffer();
	//����ڴ滺��
	void ReleaseScanStatusCacheBuffer();

	//ɨ��ɹ�
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

	//��Դ
	CEdit					m_EditFileLink;
	CEdit					m_EditFileName;
	CEdit					m_EditSavePath;
	CEdit					m_EditPort;
	CButton					m_BtnDownLoad;
	CButton					m_CheckHttpHeader;
	HICON					m_hIcon;

	//����ͳ��
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
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	//���ܺ���
public:
	//ˢ������
	void RefreshFlowCount(long lCurrentFlow, long lTotalSize);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedDownLoad();


};


#endif