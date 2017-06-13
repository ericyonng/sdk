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

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
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
	virtual LPCTSTR GetSkinFile() { return TEXT("login.xml"); }

	////////////////////////////////////////////////////

	//��ť��Ϣ
protected:
	VOID OnRegister();
	VOID OnModifyAccount();
	VOID OnLogIn();

	//���ܺ���
protected:
	//���ӷ�����
	VOID OnLinkServer();
	//����seed_no
	VOID GetSeedNo();
	//���ܷ���
	static BOOL EncryptData(VOID *pData, unsigned long *lDataLen, VOID *pEncryptData, unsigned long *lEnDataLen);

	//�ؼ�����
protected:

	//ϵͳ��Ϣ
protected:

	//��������
public:
	//���ӽ��
	VOID SetLinkResult(LPTSTR lpStr);

	DECLARE_MESSAGE_MAP()
};

#endif
