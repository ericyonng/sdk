#ifndef DLG_MODIFY_ACCOUNT_H_
#define DLG_MODIFY_ACCOUNT_H_
#pragma once
#include "TdxmHead.h"



class CDlgModifyAccount :public CYCDuiDialog
{
	//res
public:
	CPngImage		m_PngDlgBack;

public:
	CDlgModifyAccount();
	virtual ~CDlgModifyAccount();

	//重载函数
protected:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();
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
	virtual LPCTSTR GetSkinFile() { return TEXT("modifyaccount.xml"); }

	////////////////////////////////////////////////////

	//按钮消息
protected:
	VOID OnOkModify();

	//控件函数
protected:

	//系统消息
protected:

	DECLARE_MESSAGE_MAP()
};

#endif