#ifndef DLG_LOG_IN_H_
#define DLG_LOG_IN_H_
#pragma once
#include "TdxmHead.h"
#include"TcpClient.h"

//#include"SocketProtocol.h"
class CDlgLogIn :public CYCDuiDialog
{
	//res
public:
	CPngImage				m_PngDlgBack;
	CTcpClient				m_TcpSocket;
	DWORD					m_Seed_NO[MONITOR_SEED_LEN];
	HINSTANCE				m_hEncryptDll;

	static CDlgLogIn*		m_pLogIn;

public:
	CDlgLogIn();
	virtual ~CDlgLogIn();

	//重载函数
protected:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

	//接口函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);

	//重载函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT("login.xml"); }

	////////////////////////////////////////////////////

	//按钮消息
protected:
	VOID OnRegister();
	VOID OnModifyAccount();
	VOID OnLogIn();

	//功能函数
protected:
	//链接服务器
	VOID OnLinkServer();
	//产生seed_no
	VOID GetSeedNo();
	//加密方法
	static BOOL EncryptData(VOID *pData, unsigned long *lDataLen, VOID *pEncryptData, unsigned long *lEnDataLen);

	//控件函数
protected:

	//系统消息
protected:

	//辅助函数
public:
	//链接结果
	VOID SetLinkResult(LPTSTR lpStr);

	DECLARE_MESSAGE_MAP()
};

#endif
