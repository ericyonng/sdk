#ifndef DLG_REG_ACCOUNT_H_
#define DLG_REG_ACCOUNT_H_
#pragma once
#include "TdxmHead.h"

#include"../�ṹ����/BusinessLayerProtocol.h"

class CDlgRegAccount :public CYCDuiDialog
{
	//res
public:
	CPngImage		m_PngDlgBack;

public:
	CDlgRegAccount();
	virtual ~CDlgRegAccount();

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
	virtual LPCTSTR GetSkinFile() { return TEXT("register.xml"); }

	////////////////////////////////////////////////////

	//��ť��Ϣ
protected:
	VOID OnOkReg();

	//��Ϣ����
protected:
	//ִ��ע��
	BOOL OnExcuteReg();

	//�ؼ�����
protected:

	//ϵͳ��Ϣ
protected:

	DECLARE_MESSAGE_MAP()
};

#endif