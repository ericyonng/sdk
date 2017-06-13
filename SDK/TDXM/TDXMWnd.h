#ifndef TDXM_WND_H_
#define TDXM_WND_H_



#pragma once
#include "TdxmHead.h"

#include"DlgLogIn.h"
#include"DlgRegAccount.h"
#include"DlgModifyAccount.h"
#define IDM_LOG_IN                32774
#define IDM_CLOSE                32775
#define IDM_MINIMIZE                32776
#define IDM_REGISTER                32777
#define IDM_MODIFY_PWD                32778

class CTDXMFrame :public CYCDuiFrameWnd
{
public:
	string				m_strServerAddr;			//服务器地址
	unsigned short		m_ServerPort;				//服务器端口

	//静态变量
protected:
	static CTDXMFrame *			m_pTdxmFrame;					//框架指针
	CDlgLogIn	*				m_pDlgLogIn;
	CDlgRegAccount *			m_pDlgReg;
	CDlgModifyAccount	*		m_pModifyPwd;

public:
	CTDXMFrame();
	virtual ~CTDXMFrame();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//重载函数
protected:
	//消息过虑
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//绘画接口
public:
	//开始绘画
	virtual void OnBeginPaintWindow(HDC hDC);
	//结束绘画
	virtual void OnEndPaintWindow(HDC hDC);

	//重载函数
public:
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT("TDXMFrame.xml"); }

	//界面函数
private:
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);

	//辅助函数
public:
	//获取对象
	CControlUI * GetControlByNameForCustomer(LPCTSTR pszControlName){ return GetControlByName(pszControlName); }

	//消息函数
protected:
	//关闭消息
	VOID OnClose();
	//时间消息
	VOID OnTimer(UINT_PTR nIDEvent);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//位置消息
	VOID OnGetMinMaxInfo(MINMAXINFO * lpMMI);
	//创建消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);


	//功能函数
public:
	//获取实例
	static CTDXMFrame * GetInstance() { return m_pTdxmFrame; }

	//读取配置
	BOOL ReadConfig();

	DECLARE_MESSAGE_MAP()
};


#endif