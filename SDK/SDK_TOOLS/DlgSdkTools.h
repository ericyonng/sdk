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
	afx_msg void OnCopy() { Copy(); }				//复制
	afx_msg void OnCut() { Cut(); }					//剪切
	afx_msg void OnPaste() { Paste(); }				//粘贴
	afx_msg void OnSelectall() { SetSel(0, -1); }	//全选
	afx_msg void OnUndo() { Undo(); }		//撤销
	afx_msg void OnClear() { Clear(); }		//清除
	afx_msg void OnSelectfont();//改变字体

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);

	DECLARE_MESSAGE_MAP()
};

//配置文件写死：SdkToolsConfig.ini
class CDlgSdkTools :public CDialog
{
public:
	CDlgSdkTools(CWnd* pParent = NULL);
	virtual ~CDlgSdkTools(){}
	enum { IDD = IDD_DIALOG1 };
	//模态窗口
	virtual INT_PTR DoModal();

	//配置文件
protected:
	//读取配置文件
	bool LoadConfig();	//配置目录没有的要创建 没有配置文件提示文件丢失
	//读取开发库头文件目录
	void ReadHeaderFiles();
	//读取开发库lib文件目录
	void ReadLibraryFiles();

	//拷贝文件
public:
	//拷贝头文件
	bool CopyHeaderFiles();
	//拷贝lib文件
	bool CopyLibFiles();
	//绘画进度
	void DrawProgress(string strFileType, string strFileName, long lCurrentCount, long lTotalNum);

	//消息相应
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