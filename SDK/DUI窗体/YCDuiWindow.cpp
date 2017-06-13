#include "StdAfx.h"
#include "YCDuiWindow.h"

///////////////////////////////////////////////////////////////////////////////////////////
//消息定义
#define WM_QUIT_APP							(WM_USER+8976)

///////////////////////////////////////////////////////////////////////////////////////////
//消息映射
BEGIN_MESSAGE_MAP(CYCDuiWnd, CWnd)
 ON_WM_CREATE()
 ON_WM_NCDESTROY()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CYCDuiDialog, CDialog)
 ON_WM_CREATE()
 ON_WM_NCDESTROY()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CYCDuiFrameWnd, CFrameWnd)
 ON_WM_CREATE()
 ON_WM_NCDESTROY()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////////////

#define GET_L_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_H_LPARAM(lp)                        ((int)(short)HIWORD(lp))

///////////////////////////////////////////////////////////////////////////////////////////
//构造函数
CYCDuiWnd::CYCDuiWnd(void)
{
}

//析构函数
CYCDuiWnd::~CYCDuiWnd(void)
{
}

//消息处理
LRESULT CYCDuiWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if( uMsg == WM_ERASEBKGND ) 
    {
        return true;
    }

    return false;
}

//处理消息
LRESULT CYCDuiWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//变量定义
	LRESULT lResult=TRUE;
	BOOL bHandled = FALSE;

	//按键测试
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:	//左键按下
		{
			lResult=HandleLButtonDown(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_LBUTTONDBLCLK:	//左键双击
		{
			lResult=HandleLButtonDbClick(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_NCHITTEST:		//点击测试	
		{
			lResult = HandleNcHitTest(uMsg, wParam, lParam, bHandled); 
			break;
		}
	}
	if (bHandled) return lResult;
	lResult = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
	if (bHandled) return lResult;
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam,lResult)) return lResult;

	return FALSE;
}

//创建消息
int CYCDuiWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

	//设置变量
    m_PaintManager.Init(m_hWnd);
    m_PaintManager.AddNotifier(this);
	m_PaintManager.AddPreMessageFilter(this);
	m_PaintManager.SetPaintNotify(this);

	//变量定义
	CDialogBuilder Builder;
	STRINGorID xml(GetSkinFile());

	//创建控件
	CControlUI* pRoot = Builder.Create(xml,0,this,&m_PaintManager);
	if(pRoot==NULL)
	{
		//创建根节点
		pRoot = static_cast<CControlUI *>(new CContainerUI());
		pRoot->SetManager(&m_PaintManager,NULL);
	}	
	m_PaintManager.AttachDialog(pRoot);

	//初始控件
	InitControlUI();

    return 0;
}

//销毁消息
void CYCDuiWnd::OnNcDestroy()
{
	m_PaintManager.RemovePreMessageFilter(this);
	m_PaintManager.RemoveNotifier(this);
	m_PaintManager.RemoveAllTimers();
	m_PaintManager.ReapObjects(m_PaintManager.GetRoot());

	__super::OnNcDestroy();	
}

//获取对象
CControlUI * CYCDuiWnd::GetControlByName(LPCTSTR pszControlName)
{
	return m_PaintManager.FindControl(pszControlName);
}

//点击测试
bool CYCDuiWnd::DoCaptionHitTest(POINT Point)
{
	//获取区域
	RECT rcClient;
	GetClientRect(&rcClient);

	//控件过滤
	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if( Point.x >= rcClient.left + rcCaption.left && Point.x < rcClient.right - rcCaption.right \
		&& Point.y >= rcCaption.top && Point.y < rcCaption.bottom ) 
	{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(Point));
			if( pControl && _tcsicmp(pControl->GetClass(), TEXT("ButtonUI")) != 0 && 
				lstrcmp(pControl->GetClass(), TEXT("TabButtonUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("CheckButtonUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("RadioButtonUI")) != 0 &&		
				lstrcmp(pControl->GetClass(), TEXT("OptionUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("TextUI")) != 0 )
				return true;
	}

	return false;
}

//自定义消息
LRESULT CYCDuiWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 1L;
}

//按钮测试
LRESULT CYCDuiWnd::HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//变量定义
	POINT pt = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)};
	ScreenToClient(&pt);

	//获取区域
	RECT rcClient;
	GetClientRect(&rcClient);

	if( !IsZoomed() ) 
	{
		RECT rcSizeBox = m_PaintManager.GetSizeBox();
		if( pt.y < rcClient.top + rcSizeBox.top ) 
		{
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
			return HTTOP;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) 
		{
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
		if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	}

	//标题点击
	if(DoCaptionHitTest(pt)==true) return HTCAPTION;

	return HTCLIENT;
}

//左键按下
LRESULT CYCDuiWnd::HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//获取位置
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//点击测试
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//设置处理
	if ((IsZoomed()==FALSE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	//设置处理
	bHandled=TRUE;

	return TRUE;
}

//左键双击
LRESULT CYCDuiWnd::HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//获取位置
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//点击测试
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//设置处理
	bHandled=TRUE;

	return TRUE;
}

//消息转换
BOOL CYCDuiWnd::PreTranslateMessage(MSG* pMsg)
{
	if(CPaintManagerUI::TranslateMessage(pMsg)==true)
	{
		return TRUE;
	}
	
	return __super::PreTranslateMessage(pMsg);
}

//窗口过程
LRESULT CYCDuiWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//变量定义
	LRESULT lResult=HandleMessage(message, wParam, lParam);
	if(lResult==FALSE)
	{
		return __super::WindowProc(message,wParam,lParam);
	}

	return lResult;
}


///////////////////////////////////////////////////////////////////////////////////////////
//构造函数
CYCDuiFrameWnd::CYCDuiFrameWnd(void)
{
}

//析构函数
CYCDuiFrameWnd::~CYCDuiFrameWnd(void)
{
}

//消息处理
LRESULT CYCDuiFrameWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if( uMsg == WM_ERASEBKGND ) 
    {
        return true;
    }

    return false;
}

//处理消息
LRESULT CYCDuiFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//变量定义
	LRESULT lResult=TRUE;
	BOOL bHandled = FALSE;

	//按键测试
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:	//左键按下
		{
			lResult=HandleLButtonDown(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_LBUTTONDBLCLK:	//左键双击
		{
			lResult=HandleLButtonDbClick(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_NCHITTEST:		//点击测试	
		{
			lResult = HandleNcHitTest(uMsg, wParam, lParam, bHandled); 
			break;
		}
	}
	if (bHandled) return lResult;
	lResult = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
	if (bHandled) return lResult;
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam,lResult)) return lResult;

	return FALSE;
}

//创建消息
int CYCDuiFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

	//设置变量
    m_PaintManager.Init(m_hWnd);
    m_PaintManager.AddNotifier(this);
	m_PaintManager.AddPreMessageFilter(this);
	m_PaintManager.SetPaintNotify(this);

	//变量定义
	CDialogBuilder Builder;
	STRINGorID xml(GetSkinFile());

	//创建控件
	CControlUI* pRoot = Builder.Create(xml,0,this,&m_PaintManager);
	if(pRoot==NULL)
	{
		//创建根节点
		pRoot = static_cast<CControlUI *>(new CContainerUI());
		pRoot->SetManager(&m_PaintManager,NULL);
	}	
	m_PaintManager.AttachDialog(pRoot);

	//初始控件
	InitControlUI();

    return 0;
}

//销毁消息
void CYCDuiFrameWnd::OnNcDestroy()
{
	m_PaintManager.RemovePreMessageFilter(this);
	m_PaintManager.RemoveNotifier(this);
	m_PaintManager.RemoveAllTimers();
	m_PaintManager.ReapObjects(m_PaintManager.GetRoot());

	__super::OnNcDestroy();	
}


//点击测试
bool CYCDuiFrameWnd::DoCaptionHitTest(POINT Point)
{
	//获取区域
	RECT rcClient;
	GetClientRect(&rcClient);

	//控件过滤
	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if( Point.x >= rcClient.left + rcCaption.left && Point.x < rcClient.right - rcCaption.right \
		&& Point.y >= rcCaption.top && Point.y < rcCaption.bottom ) 
	{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(Point));
			if( pControl && lstrcmp(pControl->GetClass(), TEXT("ButtonUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("TabButtonUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("CheckButtonUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("RadioButtonUI")) != 0 &&	
				lstrcmp(pControl->GetClass(), TEXT("OptionUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("TextUI")) != 0 )
				return true;
	}

	return false;
}

//获取对象
CControlUI * CYCDuiFrameWnd::GetControlByName(LPCTSTR pszControlName)
{
	return m_PaintManager.FindControl(pszControlName);
}

//自定义消息
LRESULT CYCDuiFrameWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 1L;
}

//按钮测试
LRESULT CYCDuiFrameWnd::HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//变量定义
	POINT pt = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)};
	ScreenToClient(&pt);

	//获取区域
	RECT rcClient;
	GetClientRect(&rcClient);

	if( !IsZoomed() ) 
	{
		RECT rcSizeBox = m_PaintManager.GetSizeBox();
		if( pt.y < rcClient.top + rcSizeBox.top ) 
		{
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
			return HTTOP;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) 
		{
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
		if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	}

	//标题点击
	if(DoCaptionHitTest(pt)==true) return HTCAPTION;

	return HTCLIENT;
}

//左键按下
LRESULT CYCDuiFrameWnd::HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//获取位置
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//点击测试
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//设置处理
	if ((IsZoomed()==FALSE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	//设置处理
	bHandled=TRUE;

	return TRUE;
}

//左键双击
LRESULT CYCDuiFrameWnd::HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//获取位置
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//点击测试
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//设置处理
	bHandled=TRUE;

	return TRUE;
}

//消息转换
BOOL CYCDuiFrameWnd::PreTranslateMessage(MSG* pMsg)
{
	if(CPaintManagerUI::TranslateMessage(pMsg)==true)
	{
		return TRUE;
	}
	
	return __super::PreTranslateMessage(pMsg);
}

//窗口过程
LRESULT CYCDuiFrameWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//退出程序
	if(message==WM_QUIT_APP)
	{
		PostQuitMessage(0);

		return TRUE;
	}

	//变量定义
	LRESULT lResult=HandleMessage(message, wParam, lParam);
	if(lResult==FALSE)
	{
		return __super::WindowProc(message,wParam,lParam);
	}

	return lResult;
}

///////////////////////////////////////////////////////////////////////////////////////////
//构造函数
CYCDuiDialog::CYCDuiDialog(void)
{
	//使用滤镜
	m_bUseFilterBack = FALSE;
	m_pWndFilterParent = NULL;
	m_rcFilter.SetRect(0, 0, 0, 0);
}

//构造函数
CYCDuiDialog::CYCDuiDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd) :CDialog(lpszTemplateName, pParentWnd)
{
	//使用滤镜
	m_bUseFilterBack = FALSE;
	m_pWndFilterParent = NULL;
	m_rcFilter.SetRect(0, 0, 0, 0);
}

//构造函数
CYCDuiDialog::CYCDuiDialog(UINT nIDTemplate, CWnd* pParentWnd) :CDialog(nIDTemplate, pParentWnd)
{
	//使用滤镜
	m_bUseFilterBack = FALSE;
	m_pWndFilterParent = NULL;
	m_rcFilter.SetRect(0, 0, 0, 0);
}

//析构函数
CYCDuiDialog::~CYCDuiDialog(void)
{
	//销毁窗口
	//if (m_FilterWnd.m_hWnd) m_FilterWnd.DestroyWindow();
}

//消息处理
LRESULT CYCDuiDialog::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	return FALSE;
}

//处理消息
LRESULT CYCDuiDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//变量定义
	LRESULT lResult=TRUE;
	BOOL bHandled = FALSE;

	//按键测试
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:	//左键按下
		{
			lResult=HandleLButtonDown(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_LBUTTONDBLCLK:	//左键双击
		{
			lResult=HandleLButtonDbClick(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_NCHITTEST:		//点击测试	
		{
			lResult = HandleNcHitTest(uMsg, wParam, lParam, bHandled); 
			break;
		}
	}
	if (bHandled) return lResult;
	lResult = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
	if (bHandled) return lResult;
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam,lResult)) return lResult;

	return FALSE;
}

//创建消息
int CYCDuiDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

	//使用滤镜
	if (m_bUseFilterBack)
	{
		CRect rcRect(0, 0, 0, 0);
		rcRect = m_rcFilter;
		m_FilterWnd.SetFilterRect(m_rcFilter);
		m_FilterWnd.Create(NULL, NULL, WS_POPUP, rcRect, m_pWndFilterParent);
		m_FilterWnd.ShowWindow(SW_SHOW);
		m_FilterWnd.UpdateWindow();
	}

	//设置变量
    m_PaintManager.Init(m_hWnd);
    m_PaintManager.AddNotifier(this);
	m_PaintManager.AddPreMessageFilter(this);
	m_PaintManager.SetPaintNotify(this);

	//变量定义
	CDialogBuilder Builder;
	STRINGorID xml(GetSkinFile());

	//创建控件
	CControlUI* pRoot = Builder.Create(xml,0,this,&m_PaintManager);
	if(pRoot==NULL)
	{
		//创建根节点
		pRoot = static_cast<CControlUI *>(new CContainerUI());
		pRoot->SetManager(&m_PaintManager,NULL);
	}	
	m_PaintManager.AttachDialog(pRoot);

	//初始控件
	InitControlUI();

    return 0;
}

//销毁消息
VOID CYCDuiDialog::OnNcDestroy()
{
	m_PaintManager.RemovePreMessageFilter(this);
	m_PaintManager.RemoveNotifier(this);
	m_PaintManager.RemoveAllTimers();
	m_PaintManager.ReapObjects(m_PaintManager.GetRoot());	

	__super::OnNcDestroy();
}

//点击测试
bool CYCDuiDialog::DoCaptionHitTest(POINT Point)
{
	//获取区域
	RECT rcClient;
	GetClientRect(&rcClient);

	//控件过滤
	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if( Point.x >= rcClient.left + rcCaption.left && Point.x < rcClient.right - rcCaption.right \
		&& Point.y >= rcCaption.top && Point.y < rcCaption.bottom ) 
	{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(Point));
			if( pControl && _tcsicmp(pControl->GetClass(), TEXT("ButtonUI")) != 0 && 
				lstrcmp(pControl->GetClass(), TEXT("TabButtonUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("CheckButtonUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("RadioButtonUI")) != 0 &&		
				lstrcmp(pControl->GetClass(), TEXT("OptionUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("TextUI")) != 0 )
				return true;
	}

	return false;
}

//获取对象
CControlUI * CYCDuiDialog::GetControlByName(LPCTSTR pszControlName)
{
	return m_PaintManager.FindControl(pszControlName);
}

//按钮测试
LRESULT CYCDuiDialog::HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//变量定义
	POINT pt = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)};
	ScreenToClient(&pt);

	//获取区域
	RECT rcClient;
	GetClientRect(&rcClient);

	if( !IsZoomed() ) 
	{
		RECT rcSizeBox = m_PaintManager.GetSizeBox();
		if( pt.y < rcClient.top + rcSizeBox.top ) 
		{
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
			return HTTOP;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) 
		{
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
		if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	}

	//标题点击
	if(DoCaptionHitTest(pt)==true) return HTCAPTION;

	return HTCLIENT;
}

//左键按下
LRESULT CYCDuiDialog::HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//获取位置
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//点击测试
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//设置处理
	if ((IsZoomed()==FALSE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	//设置处理
	bHandled=TRUE;

	return TRUE;
}

//左键双击
LRESULT CYCDuiDialog::HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//获取位置
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//点击测试
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//设置处理
	bHandled=TRUE;

	return TRUE;
}

//自定义消息
LRESULT CYCDuiDialog::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 1L;
}

//消息转换
BOOL CYCDuiDialog::PreTranslateMessage(MSG* pMsg)
{
	if(CPaintManagerUI::TranslateMessage(pMsg)==true)
	{
		return TRUE;
	}
	
	return __super::PreTranslateMessage(pMsg);
}

//窗口过程
LRESULT CYCDuiDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//变量定义
	LRESULT lHandled = HandleMessage(message, wParam, lParam);
	if(lHandled==FALSE)
	{
		return __super::WindowProc(message,wParam,lParam);
	}

	return lHandled;
}

///////////////////////////////////////////////////////////////////////////////////////////