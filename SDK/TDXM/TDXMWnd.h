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
	string				m_strServerAddr;			//��������ַ
	unsigned short		m_ServerPort;				//�������˿�

	//��̬����
protected:
	static CTDXMFrame *			m_pTdxmFrame;					//���ָ��
	CDlgLogIn	*				m_pDlgLogIn;
	CDlgRegAccount *			m_pDlgReg;
	CDlgModifyAccount	*		m_pModifyPwd;

public:
	CTDXMFrame();
	virtual ~CTDXMFrame();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//�滭�ӿ�
public:
	//��ʼ�滭
	virtual void OnBeginPaintWindow(HDC hDC);
	//�����滭
	virtual void OnEndPaintWindow(HDC hDC);

	//���غ���
public:
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT("TDXMFrame.xml"); }

	//���溯��
private:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);

	//��������
public:
	//��ȡ����
	CControlUI * GetControlByNameForCustomer(LPCTSTR pszControlName){ return GetControlByName(pszControlName); }

	//��Ϣ����
protected:
	//�ر���Ϣ
	VOID OnClose();
	//ʱ����Ϣ
	VOID OnTimer(UINT_PTR nIDEvent);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//λ����Ϣ
	VOID OnGetMinMaxInfo(MINMAXINFO * lpMMI);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);


	//���ܺ���
public:
	//��ȡʵ��
	static CTDXMFrame * GetInstance() { return m_pTdxmFrame; }

	//��ȡ����
	BOOL ReadConfig();

	DECLARE_MESSAGE_MAP()
};


#endif