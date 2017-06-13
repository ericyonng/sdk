#include "StdAfx.h"
#include "YCDuiWindow.h"

///////////////////////////////////////////////////////////////////////////////////////////
//��Ϣ����
#define WM_QUIT_APP							(WM_USER+8976)

///////////////////////////////////////////////////////////////////////////////////////////
//��Ϣӳ��
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
//���캯��
CYCDuiWnd::CYCDuiWnd(void)
{
}

//��������
CYCDuiWnd::~CYCDuiWnd(void)
{
}

//��Ϣ����
LRESULT CYCDuiWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if( uMsg == WM_ERASEBKGND ) 
    {
        return true;
    }

    return false;
}

//������Ϣ
LRESULT CYCDuiWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//��������
	LRESULT lResult=TRUE;
	BOOL bHandled = FALSE;

	//��������
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:	//�������
		{
			lResult=HandleLButtonDown(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_LBUTTONDBLCLK:	//���˫��
		{
			lResult=HandleLButtonDbClick(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_NCHITTEST:		//�������	
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

//������Ϣ
int CYCDuiWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

	//���ñ���
    m_PaintManager.Init(m_hWnd);
    m_PaintManager.AddNotifier(this);
	m_PaintManager.AddPreMessageFilter(this);
	m_PaintManager.SetPaintNotify(this);

	//��������
	CDialogBuilder Builder;
	STRINGorID xml(GetSkinFile());

	//�����ؼ�
	CControlUI* pRoot = Builder.Create(xml,0,this,&m_PaintManager);
	if(pRoot==NULL)
	{
		//�������ڵ�
		pRoot = static_cast<CControlUI *>(new CContainerUI());
		pRoot->SetManager(&m_PaintManager,NULL);
	}	
	m_PaintManager.AttachDialog(pRoot);

	//��ʼ�ؼ�
	InitControlUI();

    return 0;
}

//������Ϣ
void CYCDuiWnd::OnNcDestroy()
{
	m_PaintManager.RemovePreMessageFilter(this);
	m_PaintManager.RemoveNotifier(this);
	m_PaintManager.RemoveAllTimers();
	m_PaintManager.ReapObjects(m_PaintManager.GetRoot());

	__super::OnNcDestroy();	
}

//��ȡ����
CControlUI * CYCDuiWnd::GetControlByName(LPCTSTR pszControlName)
{
	return m_PaintManager.FindControl(pszControlName);
}

//�������
bool CYCDuiWnd::DoCaptionHitTest(POINT Point)
{
	//��ȡ����
	RECT rcClient;
	GetClientRect(&rcClient);

	//�ؼ�����
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

//�Զ�����Ϣ
LRESULT CYCDuiWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 1L;
}

//��ť����
LRESULT CYCDuiWnd::HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��������
	POINT pt = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)};
	ScreenToClient(&pt);

	//��ȡ����
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

	//������
	if(DoCaptionHitTest(pt)==true) return HTCAPTION;

	return HTCLIENT;
}

//�������
LRESULT CYCDuiWnd::HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��ȡλ��
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//�������
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//���ô���
	if ((IsZoomed()==FALSE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	//���ô���
	bHandled=TRUE;

	return TRUE;
}

//���˫��
LRESULT CYCDuiWnd::HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��ȡλ��
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//�������
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//���ô���
	bHandled=TRUE;

	return TRUE;
}

//��Ϣת��
BOOL CYCDuiWnd::PreTranslateMessage(MSG* pMsg)
{
	if(CPaintManagerUI::TranslateMessage(pMsg)==true)
	{
		return TRUE;
	}
	
	return __super::PreTranslateMessage(pMsg);
}

//���ڹ���
LRESULT CYCDuiWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//��������
	LRESULT lResult=HandleMessage(message, wParam, lParam);
	if(lResult==FALSE)
	{
		return __super::WindowProc(message,wParam,lParam);
	}

	return lResult;
}


///////////////////////////////////////////////////////////////////////////////////////////
//���캯��
CYCDuiFrameWnd::CYCDuiFrameWnd(void)
{
}

//��������
CYCDuiFrameWnd::~CYCDuiFrameWnd(void)
{
}

//��Ϣ����
LRESULT CYCDuiFrameWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if( uMsg == WM_ERASEBKGND ) 
    {
        return true;
    }

    return false;
}

//������Ϣ
LRESULT CYCDuiFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//��������
	LRESULT lResult=TRUE;
	BOOL bHandled = FALSE;

	//��������
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:	//�������
		{
			lResult=HandleLButtonDown(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_LBUTTONDBLCLK:	//���˫��
		{
			lResult=HandleLButtonDbClick(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_NCHITTEST:		//�������	
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

//������Ϣ
int CYCDuiFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

	//���ñ���
    m_PaintManager.Init(m_hWnd);
    m_PaintManager.AddNotifier(this);
	m_PaintManager.AddPreMessageFilter(this);
	m_PaintManager.SetPaintNotify(this);

	//��������
	CDialogBuilder Builder;
	STRINGorID xml(GetSkinFile());

	//�����ؼ�
	CControlUI* pRoot = Builder.Create(xml,0,this,&m_PaintManager);
	if(pRoot==NULL)
	{
		//�������ڵ�
		pRoot = static_cast<CControlUI *>(new CContainerUI());
		pRoot->SetManager(&m_PaintManager,NULL);
	}	
	m_PaintManager.AttachDialog(pRoot);

	//��ʼ�ؼ�
	InitControlUI();

    return 0;
}

//������Ϣ
void CYCDuiFrameWnd::OnNcDestroy()
{
	m_PaintManager.RemovePreMessageFilter(this);
	m_PaintManager.RemoveNotifier(this);
	m_PaintManager.RemoveAllTimers();
	m_PaintManager.ReapObjects(m_PaintManager.GetRoot());

	__super::OnNcDestroy();	
}


//�������
bool CYCDuiFrameWnd::DoCaptionHitTest(POINT Point)
{
	//��ȡ����
	RECT rcClient;
	GetClientRect(&rcClient);

	//�ؼ�����
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

//��ȡ����
CControlUI * CYCDuiFrameWnd::GetControlByName(LPCTSTR pszControlName)
{
	return m_PaintManager.FindControl(pszControlName);
}

//�Զ�����Ϣ
LRESULT CYCDuiFrameWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 1L;
}

//��ť����
LRESULT CYCDuiFrameWnd::HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��������
	POINT pt = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)};
	ScreenToClient(&pt);

	//��ȡ����
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

	//������
	if(DoCaptionHitTest(pt)==true) return HTCAPTION;

	return HTCLIENT;
}

//�������
LRESULT CYCDuiFrameWnd::HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��ȡλ��
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//�������
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//���ô���
	if ((IsZoomed()==FALSE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	//���ô���
	bHandled=TRUE;

	return TRUE;
}

//���˫��
LRESULT CYCDuiFrameWnd::HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��ȡλ��
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//�������
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//���ô���
	bHandled=TRUE;

	return TRUE;
}

//��Ϣת��
BOOL CYCDuiFrameWnd::PreTranslateMessage(MSG* pMsg)
{
	if(CPaintManagerUI::TranslateMessage(pMsg)==true)
	{
		return TRUE;
	}
	
	return __super::PreTranslateMessage(pMsg);
}

//���ڹ���
LRESULT CYCDuiFrameWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//�˳�����
	if(message==WM_QUIT_APP)
	{
		PostQuitMessage(0);

		return TRUE;
	}

	//��������
	LRESULT lResult=HandleMessage(message, wParam, lParam);
	if(lResult==FALSE)
	{
		return __super::WindowProc(message,wParam,lParam);
	}

	return lResult;
}

///////////////////////////////////////////////////////////////////////////////////////////
//���캯��
CYCDuiDialog::CYCDuiDialog(void)
{
	//ʹ���˾�
	m_bUseFilterBack = FALSE;
	m_pWndFilterParent = NULL;
	m_rcFilter.SetRect(0, 0, 0, 0);
}

//���캯��
CYCDuiDialog::CYCDuiDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd) :CDialog(lpszTemplateName, pParentWnd)
{
	//ʹ���˾�
	m_bUseFilterBack = FALSE;
	m_pWndFilterParent = NULL;
	m_rcFilter.SetRect(0, 0, 0, 0);
}

//���캯��
CYCDuiDialog::CYCDuiDialog(UINT nIDTemplate, CWnd* pParentWnd) :CDialog(nIDTemplate, pParentWnd)
{
	//ʹ���˾�
	m_bUseFilterBack = FALSE;
	m_pWndFilterParent = NULL;
	m_rcFilter.SetRect(0, 0, 0, 0);
}

//��������
CYCDuiDialog::~CYCDuiDialog(void)
{
	//���ٴ���
	//if (m_FilterWnd.m_hWnd) m_FilterWnd.DestroyWindow();
}

//��Ϣ����
LRESULT CYCDuiDialog::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	return FALSE;
}

//������Ϣ
LRESULT CYCDuiDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//��������
	LRESULT lResult=TRUE;
	BOOL bHandled = FALSE;

	//��������
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:	//�������
		{
			lResult=HandleLButtonDown(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_LBUTTONDBLCLK:	//���˫��
		{
			lResult=HandleLButtonDbClick(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_NCHITTEST:		//�������	
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

//������Ϣ
int CYCDuiDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

	//ʹ���˾�
	if (m_bUseFilterBack)
	{
		CRect rcRect(0, 0, 0, 0);
		rcRect = m_rcFilter;
		m_FilterWnd.SetFilterRect(m_rcFilter);
		m_FilterWnd.Create(NULL, NULL, WS_POPUP, rcRect, m_pWndFilterParent);
		m_FilterWnd.ShowWindow(SW_SHOW);
		m_FilterWnd.UpdateWindow();
	}

	//���ñ���
    m_PaintManager.Init(m_hWnd);
    m_PaintManager.AddNotifier(this);
	m_PaintManager.AddPreMessageFilter(this);
	m_PaintManager.SetPaintNotify(this);

	//��������
	CDialogBuilder Builder;
	STRINGorID xml(GetSkinFile());

	//�����ؼ�
	CControlUI* pRoot = Builder.Create(xml,0,this,&m_PaintManager);
	if(pRoot==NULL)
	{
		//�������ڵ�
		pRoot = static_cast<CControlUI *>(new CContainerUI());
		pRoot->SetManager(&m_PaintManager,NULL);
	}	
	m_PaintManager.AttachDialog(pRoot);

	//��ʼ�ؼ�
	InitControlUI();

    return 0;
}

//������Ϣ
VOID CYCDuiDialog::OnNcDestroy()
{
	m_PaintManager.RemovePreMessageFilter(this);
	m_PaintManager.RemoveNotifier(this);
	m_PaintManager.RemoveAllTimers();
	m_PaintManager.ReapObjects(m_PaintManager.GetRoot());	

	__super::OnNcDestroy();
}

//�������
bool CYCDuiDialog::DoCaptionHitTest(POINT Point)
{
	//��ȡ����
	RECT rcClient;
	GetClientRect(&rcClient);

	//�ؼ�����
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

//��ȡ����
CControlUI * CYCDuiDialog::GetControlByName(LPCTSTR pszControlName)
{
	return m_PaintManager.FindControl(pszControlName);
}

//��ť����
LRESULT CYCDuiDialog::HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��������
	POINT pt = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)};
	ScreenToClient(&pt);

	//��ȡ����
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

	//������
	if(DoCaptionHitTest(pt)==true) return HTCAPTION;

	return HTCLIENT;
}

//�������
LRESULT CYCDuiDialog::HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��ȡλ��
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//�������
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//���ô���
	if ((IsZoomed()==FALSE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	//���ô���
	bHandled=TRUE;

	return TRUE;
}

//���˫��
LRESULT CYCDuiDialog::HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��ȡλ��
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//�������
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//���ô���
	bHandled=TRUE;

	return TRUE;
}

//�Զ�����Ϣ
LRESULT CYCDuiDialog::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 1L;
}

//��Ϣת��
BOOL CYCDuiDialog::PreTranslateMessage(MSG* pMsg)
{
	if(CPaintManagerUI::TranslateMessage(pMsg)==true)
	{
		return TRUE;
	}
	
	return __super::PreTranslateMessage(pMsg);
}

//���ڹ���
LRESULT CYCDuiDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//��������
	LRESULT lHandled = HandleMessage(message, wParam, lParam);
	if(lHandled==FALSE)
	{
		return __super::WindowProc(message,wParam,lParam);
	}

	return lHandled;
}

///////////////////////////////////////////////////////////////////////////////////////////