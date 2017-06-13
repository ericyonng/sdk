// ExRichEdit.cpp : implementation file
//

#include "stdafx.h"
#include "ExRichEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExRichEdit

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
	lstrcpyn(lf.lfFaceName, cf.szFaceName, sizeof(lf.lfFaceName)/sizeof(TCHAR));

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
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1) return -1;
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