#ifndef GUI_FOUNDATION_HEAD_FILE
#define GUI_FOUNDATION_HEAD_FILE
#pragma once

//引入文件
#include "GuiFoundationHead.h"
#include"FilterWnd.h"

///////////////////////////////////////////////////////////////////////////////////////////
//DUI窗口
class GUI_FOUNDATION_CLASS CYCDuiWnd : public CWnd,public INotifyUI,public IPaintNotifyUI,public IMessageFilterUI,public IDialogBuilderCallback
{
	//变量定义
protected:
	CPaintManagerUI						m_PaintManager;						//绘画管理

	//函数定义
public:
	//构造函数
	CYCDuiWnd(void);
	//析构函数
	virtual ~CYCDuiWnd(void);

    //接口函数
public:
	//消息提醒
	virtual void Notify(TNotifyUI &  msg) { return;}
	//创建控件
	virtual CControlUI* CreateControl(LPCTSTR pstrClass) { return NULL; }
	//设置属性
	virtual void SetCustomAttribute(LPCTSTR pszName,LPCTSTR pszValue) { return; }
	//消息处理
    virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	//处理消息
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//绘画接口
public:
	//开始绘画
	virtual void OnBeginPaintWindow(HDC hDC) { return; }
	//结束绘画
	virtual void OnEndPaintWindow(HDC hDC) { return; }

	//继承函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT(""); }
	//初始控件
	virtual void InitControlUI() { return; }

	//响应函数
public:
	//按钮测试
	virtual LRESULT HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//左键按下
	virtual LRESULT HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//左键双击
	virtual LRESULT HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//自定义消息
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//辅助函数
protected:
	//点击测试
	bool DoCaptionHitTest(POINT Point);

	//辅助函数
protected:
	//获取对象
	CControlUI * GetControlByName(LPCTSTR pszControlName);

	//功能函数
public:
	//标题区域
	CRect GetCaptionRect() { return CRect(m_PaintManager.GetCaptionRect()); }
	//尺寸边框
	CRect GetSizeBox() { return CRect(m_PaintManager.GetSizeBox()); }
	//圆角尺寸
	CSize GetRoundCornor() { return CSize(m_PaintManager.GetRoundCorner()); }	
	
	//继承函数
protected:
	//消息转换
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//窗口过程
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	 

	//消息函数
public:
	//创建消息
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//销毁消息
	void OnNcDestroy();

	DECLARE_MESSAGE_MAP()
};

///////////////////////////////////////////////////////////////////////////////////////////
//DUI窗口
class GUI_FOUNDATION_CLASS CYCDuiFrameWnd : public CFrameWnd, public INotifyUI, public IPaintNotifyUI, public IMessageFilterUI, public IDialogBuilderCallback
{
	//变量定义
protected:
	CPaintManagerUI						m_PaintManager;						//绘画管理

	//函数定义
public:
	//构造函数
	CYCDuiFrameWnd(void);
	//析构函数
	virtual ~CYCDuiFrameWnd(void);

    //接口函数
public:
	//消息提醒
	virtual void Notify(TNotifyUI &  msg) { return;}
	//创建控件
	virtual CControlUI* CreateControl(LPCTSTR pstrClass) { return NULL; }
	//设置属性
	virtual void SetCustomAttribute(LPCTSTR pszName,LPCTSTR pszValue) { return; }
	//消息处理
    virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	//处理消息
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//绘画接口
public:
	//开始绘画
	virtual void OnBeginPaintWindow(HDC hDC) { return; }
	//结束绘画
	virtual void OnEndPaintWindow(HDC hDC) { return; }

	//继承函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT(""); }
	//初始控件
	virtual void InitControlUI() { return; }

	//响应函数
public:
	//按钮测试
	virtual LRESULT HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//左键按下
	virtual LRESULT HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//左键双击
	virtual LRESULT HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//自定义消息
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//辅助函数
protected:
	//点击测试
	bool DoCaptionHitTest(POINT Point);

	//辅助函数
protected:
	//获取对象
	CControlUI * GetControlByName(LPCTSTR pszControlName);

	//功能函数
public:
	//标题区域
	CRect GetCaptionRect() { return CRect(m_PaintManager.GetCaptionRect()); }
	//尺寸边框
	CRect GetSizeBox() { return CRect(m_PaintManager.GetSizeBox()); }
	//圆角尺寸
	CSize GetRoundCornor() { return CSize(m_PaintManager.GetRoundCorner()); }
	
	//继承函数
protected:
	//消息转换
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//窗口过程
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	 

	//消息函数
public:
	//创建消息
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//销毁消息
	void OnNcDestroy();

	DECLARE_MESSAGE_MAP()
};

///////////////////////////////////////////////////////////////////////////////////////////
//DUI对话框
class GUI_FOUNDATION_CLASS CYCDuiDialog : public CDialog,public INotifyUI,public IPaintNotifyUI, public IMessageFilterUI,public IDialogBuilderCallback
{
	//变量定义
protected:
	CPaintManagerUI						m_PaintManager;						//绘画管理
	BOOL								m_bUseFilterBack;					//使用滤镜背景
	CRect								m_rcFilter;							//滤镜位置

	//窗体
protected:
	CFilterWnd							m_FilterWnd;						//滤镜窗体
	CWnd	*							m_pWndFilterParent;					//滤镜父窗口

	//函数定义
public:
	//构造函数
	CYCDuiDialog(void);
	//构造函数
	explicit CYCDuiDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	//构造函数
	explicit CYCDuiDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	//析构函数
	virtual ~CYCDuiDialog(void);

    //接口函数
public:
	//消息提醒
	virtual void Notify(TNotifyUI &  msg) { return;}
	//创建控件
	virtual CControlUI* CreateControl(LPCTSTR pstrClass) { return NULL; }
	//设置属性
	virtual void SetCustomAttribute(LPCTSTR pszName,LPCTSTR pszValue) { return; }
	//消息处理
    virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	//处理消息
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//绘画接口
public:
	//开始绘画
	virtual void OnBeginPaintWindow(HDC hDC) { return; }
	//结束绘画
	virtual void OnEndPaintWindow(HDC hDC) { return; }

	//响应函数
public:
	//按钮测试
	virtual LRESULT HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//左键按下
	virtual LRESULT HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//左键双击
	virtual LRESULT HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//自定义消息
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//辅助函数
protected:
	//点击测试
	bool DoCaptionHitTest(POINT Point);

	//辅助函数
protected:
	//获取对象
	CControlUI * GetControlByName(LPCTSTR pszControlName);

	//功能函数
public:
	//标题区域
	CRect GetCaptionRect() { return CRect(m_PaintManager.GetCaptionRect()); }
	//尺寸边框
	CRect GetSizeBox() { return CRect(m_PaintManager.GetSizeBox()); }
	//圆角尺寸
	CSize GetRoundCornor() { return CSize(m_PaintManager.GetRoundCorner()); }
	//获取滤镜窗
	CFilterWnd *GetFilterWnd(){ return &m_FilterWnd; }

	//继承函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT(""); }
	//初始控件
	virtual void InitControlUI() { return; }

	//继承函数
protected:
	//消息转换
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//窗口过程
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	 

	//消息函数
public:
	//创建消息
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//销毁消息
	void OnNcDestroy();

	DECLARE_MESSAGE_MAP()	
};
///////////////////////////////////////////////////////////////////////////////////////////
#endif