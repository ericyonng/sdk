
#include "Stdafx.h"
#include "FilterWnd.h"

BEGIN_MESSAGE_MAP(CFilterWnd, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//���캯��
CFilterWnd::CFilterWnd()
{
	//͸����
	m_nTransparency = FILTER_DEFAULT_TRANSPARENCY;
	m_crFilterColor = FILTER_BACKGROUND_DEFAULT_COLOR;
	m_rcFilter.SetRect(0, 0, 0, 0);
}

//��������
CFilterWnd::~CFilterWnd()
{
	//�رմ���
	//DestroyWindow();
}

//�ػ�
void CFilterWnd::OnPaint()
{
	//�豸����
	CPaintDC dc(this);
	dc.SetBkMode(TRANSPARENT);

	//�滭����
	CRect rect(0, 0, 0, 0);
	GetClientRect(&rect);
	dc.FillSolidRect(&rect, m_crFilterColor);
}

//��Ϣ������
BOOL CFilterWnd::PreTranslateMessage(MSG* pMsg)
{
	return __super::PreTranslateMessage(pMsg);
} 

//����Ԥ����
BOOL CFilterWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	//ȥ���߿�
	cs.style &= ~WS_BORDER;  
	cs.style &= ~WS_CAPTION;
	cs.style &= ~WS_THICKFRAME;
	return __super::PreCreateWindow(cs);
}

//������Ϣ
int CFilterWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	 //͸������
	 //SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetWindowLong(m_hWnd, GWL_EXSTYLE,  WS_EX_LAYERED);

	 //͸����
	 SetTransparency(m_nTransparency);

	 //���ô���Z��
	 if (m_rcFilter.Width() == 0 || m_rcFilter.Height() == 0)
	 {
		 ::SetWindowPos(m_hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	 }
	 else
	 {
		 ::SetWindowPos(m_hWnd, HWND_BOTTOM, m_rcFilter.left - 2, m_rcFilter .top- 2, m_rcFilter.Width() + 4, m_rcFilter.Height() + 4, SWP_HIDEWINDOW);
	 }
	 
	 //������������
	 CRgn rgnFilter;
	 rgnFilter.CreateRectRgn(2, 2, m_rcFilter.Width()+2, m_rcFilter.Height()+2);

	 SetWindowRgn(rgnFilter, TRUE);

	 return 0;

}

//����͸����
VOID CFilterWnd::SetTransparency(INT nTransparency)
{
	//����У��
	if (nTransparency < 0) return;

	//����͸����
	m_nTransparency = nTransparency;
	::SetLayeredWindowAttributes(m_hWnd, RGB(255, 255, 255), m_nTransparency, LWA_ALPHA);
}

//�����˾���ɫ
VOID CFilterWnd::SetFilterColor(COLORREF crFilterColor)
{
	m_crFilterColor = crFilterColor;

	//���½���
	Invalidate();
}