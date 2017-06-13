#ifndef GUI_FOUNDATION_HEAD_FILE
#define GUI_FOUNDATION_HEAD_FILE
#pragma once

//�����ļ�
#include "GuiFoundationHead.h"
#include"FilterWnd.h"

///////////////////////////////////////////////////////////////////////////////////////////
//DUI����
class GUI_FOUNDATION_CLASS CYCDuiWnd : public CWnd,public INotifyUI,public IPaintNotifyUI,public IMessageFilterUI,public IDialogBuilderCallback
{
	//��������
protected:
	CPaintManagerUI						m_PaintManager;						//�滭����

	//��������
public:
	//���캯��
	CYCDuiWnd(void);
	//��������
	virtual ~CYCDuiWnd(void);

    //�ӿں���
public:
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg) { return;}
	//�����ؼ�
	virtual CControlUI* CreateControl(LPCTSTR pstrClass) { return NULL; }
	//��������
	virtual void SetCustomAttribute(LPCTSTR pszName,LPCTSTR pszValue) { return; }
	//��Ϣ����
    virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	//������Ϣ
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//�滭�ӿ�
public:
	//��ʼ�滭
	virtual void OnBeginPaintWindow(HDC hDC) { return; }
	//�����滭
	virtual void OnEndPaintWindow(HDC hDC) { return; }

	//�̳к���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT(""); }
	//��ʼ�ؼ�
	virtual void InitControlUI() { return; }

	//��Ӧ����
public:
	//��ť����
	virtual LRESULT HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//�������
	virtual LRESULT HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//���˫��
	virtual LRESULT HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//�Զ�����Ϣ
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//��������
protected:
	//�������
	bool DoCaptionHitTest(POINT Point);

	//��������
protected:
	//��ȡ����
	CControlUI * GetControlByName(LPCTSTR pszControlName);

	//���ܺ���
public:
	//��������
	CRect GetCaptionRect() { return CRect(m_PaintManager.GetCaptionRect()); }
	//�ߴ�߿�
	CRect GetSizeBox() { return CRect(m_PaintManager.GetSizeBox()); }
	//Բ�ǳߴ�
	CSize GetRoundCornor() { return CSize(m_PaintManager.GetRoundCorner()); }	
	
	//�̳к���
protected:
	//��Ϣת��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//���ڹ���
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	 

	//��Ϣ����
public:
	//������Ϣ
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//������Ϣ
	void OnNcDestroy();

	DECLARE_MESSAGE_MAP()
};

///////////////////////////////////////////////////////////////////////////////////////////
//DUI����
class GUI_FOUNDATION_CLASS CYCDuiFrameWnd : public CFrameWnd, public INotifyUI, public IPaintNotifyUI, public IMessageFilterUI, public IDialogBuilderCallback
{
	//��������
protected:
	CPaintManagerUI						m_PaintManager;						//�滭����

	//��������
public:
	//���캯��
	CYCDuiFrameWnd(void);
	//��������
	virtual ~CYCDuiFrameWnd(void);

    //�ӿں���
public:
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg) { return;}
	//�����ؼ�
	virtual CControlUI* CreateControl(LPCTSTR pstrClass) { return NULL; }
	//��������
	virtual void SetCustomAttribute(LPCTSTR pszName,LPCTSTR pszValue) { return; }
	//��Ϣ����
    virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	//������Ϣ
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//�滭�ӿ�
public:
	//��ʼ�滭
	virtual void OnBeginPaintWindow(HDC hDC) { return; }
	//�����滭
	virtual void OnEndPaintWindow(HDC hDC) { return; }

	//�̳к���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT(""); }
	//��ʼ�ؼ�
	virtual void InitControlUI() { return; }

	//��Ӧ����
public:
	//��ť����
	virtual LRESULT HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//�������
	virtual LRESULT HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//���˫��
	virtual LRESULT HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//�Զ�����Ϣ
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//��������
protected:
	//�������
	bool DoCaptionHitTest(POINT Point);

	//��������
protected:
	//��ȡ����
	CControlUI * GetControlByName(LPCTSTR pszControlName);

	//���ܺ���
public:
	//��������
	CRect GetCaptionRect() { return CRect(m_PaintManager.GetCaptionRect()); }
	//�ߴ�߿�
	CRect GetSizeBox() { return CRect(m_PaintManager.GetSizeBox()); }
	//Բ�ǳߴ�
	CSize GetRoundCornor() { return CSize(m_PaintManager.GetRoundCorner()); }
	
	//�̳к���
protected:
	//��Ϣת��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//���ڹ���
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	 

	//��Ϣ����
public:
	//������Ϣ
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//������Ϣ
	void OnNcDestroy();

	DECLARE_MESSAGE_MAP()
};

///////////////////////////////////////////////////////////////////////////////////////////
//DUI�Ի���
class GUI_FOUNDATION_CLASS CYCDuiDialog : public CDialog,public INotifyUI,public IPaintNotifyUI, public IMessageFilterUI,public IDialogBuilderCallback
{
	//��������
protected:
	CPaintManagerUI						m_PaintManager;						//�滭����
	BOOL								m_bUseFilterBack;					//ʹ���˾�����
	CRect								m_rcFilter;							//�˾�λ��

	//����
protected:
	CFilterWnd							m_FilterWnd;						//�˾�����
	CWnd	*							m_pWndFilterParent;					//�˾�������

	//��������
public:
	//���캯��
	CYCDuiDialog(void);
	//���캯��
	explicit CYCDuiDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	//���캯��
	explicit CYCDuiDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	//��������
	virtual ~CYCDuiDialog(void);

    //�ӿں���
public:
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg) { return;}
	//�����ؼ�
	virtual CControlUI* CreateControl(LPCTSTR pstrClass) { return NULL; }
	//��������
	virtual void SetCustomAttribute(LPCTSTR pszName,LPCTSTR pszValue) { return; }
	//��Ϣ����
    virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	//������Ϣ
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//�滭�ӿ�
public:
	//��ʼ�滭
	virtual void OnBeginPaintWindow(HDC hDC) { return; }
	//�����滭
	virtual void OnEndPaintWindow(HDC hDC) { return; }

	//��Ӧ����
public:
	//��ť����
	virtual LRESULT HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//�������
	virtual LRESULT HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//���˫��
	virtual LRESULT HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//�Զ�����Ϣ
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//��������
protected:
	//�������
	bool DoCaptionHitTest(POINT Point);

	//��������
protected:
	//��ȡ����
	CControlUI * GetControlByName(LPCTSTR pszControlName);

	//���ܺ���
public:
	//��������
	CRect GetCaptionRect() { return CRect(m_PaintManager.GetCaptionRect()); }
	//�ߴ�߿�
	CRect GetSizeBox() { return CRect(m_PaintManager.GetSizeBox()); }
	//Բ�ǳߴ�
	CSize GetRoundCornor() { return CSize(m_PaintManager.GetRoundCorner()); }
	//��ȡ�˾���
	CFilterWnd *GetFilterWnd(){ return &m_FilterWnd; }

	//�̳к���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT(""); }
	//��ʼ�ؼ�
	virtual void InitControlUI() { return; }

	//�̳к���
protected:
	//��Ϣת��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//���ڹ���
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	 

	//��Ϣ����
public:
	//������Ϣ
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//������Ϣ
	void OnNcDestroy();

	DECLARE_MESSAGE_MAP()	
};
///////////////////////////////////////////////////////////////////////////////////////////
#endif