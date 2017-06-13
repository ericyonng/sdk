
#include "Stdafx.h"
#include "FilterWnd.h"

BEGIN_MESSAGE_MAP(CFilterWnd, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//构造函数
CFilterWnd::CFilterWnd()
{
	//透明度
	m_nTransparency = FILTER_DEFAULT_TRANSPARENCY;
	m_crFilterColor = FILTER_BACKGROUND_DEFAULT_COLOR;
	m_rcFilter.SetRect(0, 0, 0, 0);
}

//析构函数
CFilterWnd::~CFilterWnd()
{
	//关闭窗口
	//DestroyWindow();
}

//重绘
void CFilterWnd::OnPaint()
{
	//设备环境
	CPaintDC dc(this);
	dc.SetBkMode(TRANSPARENT);

	//绘画背景
	CRect rect(0, 0, 0, 0);
	GetClientRect(&rect);
	dc.FillSolidRect(&rect, m_crFilterColor);
}

//消息处理函数
BOOL CFilterWnd::PreTranslateMessage(MSG* pMsg)
{
	return __super::PreTranslateMessage(pMsg);
} 

//创建预处理
BOOL CFilterWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	//去掉边框
	cs.style &= ~WS_BORDER;  
	cs.style &= ~WS_CAPTION;
	cs.style &= ~WS_THICKFRAME;
	return __super::PreCreateWindow(cs);
}

//创建消息
int CFilterWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	 //透明窗体
	 //SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetWindowLong(m_hWnd, GWL_EXSTYLE,  WS_EX_LAYERED);

	 //透明度
	 SetTransparency(m_nTransparency);

	 //设置窗体Z序
	 if (m_rcFilter.Width() == 0 || m_rcFilter.Height() == 0)
	 {
		 ::SetWindowPos(m_hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	 }
	 else
	 {
		 ::SetWindowPos(m_hWnd, HWND_BOTTOM, m_rcFilter.left - 2, m_rcFilter .top- 2, m_rcFilter.Width() + 4, m_rcFilter.Height() + 4, SWP_HIDEWINDOW);
	 }
	 
	 //窗体区域限制
	 CRgn rgnFilter;
	 rgnFilter.CreateRectRgn(2, 2, m_rcFilter.Width()+2, m_rcFilter.Height()+2);

	 SetWindowRgn(rgnFilter, TRUE);

	 return 0;

}

//设置透明度
VOID CFilterWnd::SetTransparency(INT nTransparency)
{
	//参数校验
	if (nTransparency < 0) return;

	//设置透明度
	m_nTransparency = nTransparency;
	::SetLayeredWindowAttributes(m_hWnd, RGB(255, 255, 255), m_nTransparency, LWA_ALPHA);
}

//设置滤镜颜色
VOID CFilterWnd::SetFilterColor(COLORREF crFilterColor)
{
	m_crFilterColor = crFilterColor;

	//更新界面
	Invalidate();
}