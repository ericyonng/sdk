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

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��������
	virtual BOOL OnInitDialog();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//�ӿں���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);

	//���غ���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT("modifyaccount.xml"); }

	////////////////////////////////////////////////////

	//��ť��Ϣ
protected:
	VOID OnOkModify();

	//�ؼ�����
protected:

	//ϵͳ��Ϣ
protected:

	DECLARE_MESSAGE_MAP()
};

#endif