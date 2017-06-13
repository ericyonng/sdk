#if !defined(AFX_EXRICHEDIT_H__01784CAD_9A89_481A_AC0E_63E1E5205AB3__INCLUDED_)
#define AFX_EXRICHEDIT_H__01784CAD_9A89_481A_AC0E_63E1E5205AB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExRichEdit.h : header file
//

#define MSG_COLOR_RED		RGB(0xff,0x00,0x00)
#define MSG_COLOR_BLUE		RGB(0x0c,0x01,0x80)
#define MSG_COLOR_GREEN		RGB(0x00,0xcc,0x00)
#define MSG_COLOR_YELLOW	RGB(0xcc,0xdd,0x00)
#define MSG_COLOR_GRAY		RGB(0xee,0xee,0xee)
#define MSG_COLOR_BLACK		RGB(0x00,0x00,0x00)

#define ID_RICH_UNDO	201
#define ID_RICH_CUT		202
#define ID_RICH_COPY	203
#define ID_RICH_PASTE	204
#define ID_RICH_CLEAR	205
#define ID_RICH_SELECTALL	206
#define ID_RICH_SETFONT		207

/////////////////////////////////////////////////////////////////////////////
// CExRichEdit window

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
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXRICHEDIT_H__01784CAD_9A89_481A_AC0E_63E1E5205AB3__INCLUDED_)
