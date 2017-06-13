#ifndef DLG_SDK_TOOLS_H_
#define DLG_SDK_TOOLS_H_

#pragma once
#include"resource.h"


#define MSG_COLOR_RED		RGB(0xff,0x00,0x00)
#define MSG_COLOR_BLUE		RGB(0x0c,0x01,0x80)
#define MSG_COLOR_GREEN		RGB(0x00,0xcc,0x00)
#define MSG_COLOR_YELLOW	RGB(0xcc,0xdd,0x00)
#define MSG_COLOR_GRAY		RGB(0xee,0xee,0xee)
#define MSG_COLOR_BLACK		RGB(0x00,0x00,0x00)



class CExRichEdit : public CRichEditCtrl
{
	// Construction
public:
	CExRichEdit();
	void SetMaxLines(long maxlines);

	// Attributes
public:

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExRichEdit)
	//}}AFX_VIRTUAL

	// Implementation
public:
	void DispMsg(CString Msg);
	void SetBackColor(COLORREF bk_color);
	void DispMsg(CString Msg, COLORREF fn_color, DWORD fn_effects);
	long lMaxLines;
	virtual ~CExRichEdit();

	// Generated message map functions
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//{{AFX_MSG(CExRichEdit)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnCopy() { Copy(); }				//����
	afx_msg void OnCut() { Cut(); }					//����
	afx_msg void OnPaste() { Paste(); }				//ճ��
	afx_msg void OnSelectall() { SetSel(0, -1); }	//ȫѡ
	afx_msg void OnUndo() { Undo(); }		//����
	afx_msg void OnClear() { Clear(); }		//���
	afx_msg void OnSelectfont();//�ı�����

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);

	DECLARE_MESSAGE_MAP()
};

//�����ļ�д����SdkToolsConfig.ini
class CDlgSdkTools :public CDialog
{
public:
	CDlgSdkTools(CWnd* pParent = NULL);
	virtual ~CDlgSdkTools(){}
	enum { IDD = IDD_DIALOG1 };
	//ģ̬����
	virtual INT_PTR DoModal();

	//�����ļ�
protected:
	//��ȡ�����ļ�
	bool LoadConfig();	//����Ŀ¼û�е�Ҫ���� û�������ļ���ʾ�ļ���ʧ
	//��ȡ������ͷ�ļ�Ŀ¼
	void ReadHeaderFiles();
	//��ȡ������lib�ļ�Ŀ¼
	void ReadLibraryFiles();

	//�����ļ�
public:
	//����ͷ�ļ�
	bool CopyHeaderFiles();
	//����lib�ļ�
	bool CopyLibFiles();
	//�滭����
	void DrawProgress(string strFileType, string strFileName, long lCurrentCount, long lTotalNum);

	//��Ϣ��Ӧ
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

protected:
	string		m_strHeaderSrcPath;
	string		m_strHeaderDestPath;
	string		m_strLibSrcPath;
	string		m_strLibDestPath;
	HICON		m_hIcon;
	CExRichEdit  m_HeaderFileView;
	CExRichEdit  m_LibFileView;
	CFont				m_TextFont;

public:
	CButton		m_btnEnvBuild;
	CButton		m_btnCancel;
	CButton		m_btnRefreshList;

public:

	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnEnChangeHeaderEdit();
	afx_msg void OnBnClickedRefresh();
	afx_msg void OnBnClickedAbout();
};
#endif