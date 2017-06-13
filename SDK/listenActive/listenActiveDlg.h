// listenActiveDlg.h : header file
//
#include "ExRichEdit.h"
//#include "MySqlDataBase.h"
#include"YCMySqlDataBaseEngine.h"
#include"resource.h"
#if !defined(AFX_LISTENACTIVEDLG_H__8D8F764C_D0C3_4EDD_ACAA_C80710B7EEC3__INCLUDED_)
#define AFX_LISTENACTIVEDLG_H__8D8F764C_D0C3_4EDD_ACAA_C80710B7EEC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
typedef char *(_stdcall *tagencrypt1)(char *,char *);//��һ�μ���
typedef char *(_stdcall *tagencrypt2)(char *key,char *mail);//�ڶ��μ���
/////////////////////////////////////////////////////////////////////////////
// CListenActiveDlg dialog
typedef struct
{
	int  id;
	CString  userid;
	CString	 password;
	CString	 email;
	CString	 game_server_id;
}activeUser;

class CRegTimeSet : public CDialog
{
public:
	CRegTimeSet(CWnd* pParent = NULL);
	virtual ~CRegTimeSet(){}
	enum { IDD = IDD_DIALOG1 };
	CString m_strTimeStart;
	CString m_strTimeEnd;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	// modal processing
	virtual INT_PTR DoModal();

	//д�����ñ�
	void WriteConfig();

	CStatic m_StaticErrText;
	CEdit	m_EditStartHour;
	CEdit	m_EditStartMin;
	CEdit	m_EditStartSec;
	CEdit	m_EditEndHour;
	CEdit	m_EditEndMin;
	CEdit	m_EditEndSec;
	INT		m_nStartHour;
	INT		m_nStartMin;
	INT		m_nStartSec;
	INT		m_nEndHour;
	INT		m_nEndMin;
	INT		m_nEndSec;

	//������
	void *m_pListenDlg;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnEnChangeStartHour();
};

class CListenActiveDlg : public CDialog
{
// Construction
public:
	CListenActiveDlg(CWnd* pParent = NULL);	// standard constructor
	HINSTANCE m_hmDll;
	tagencrypt1 encrypt1;
	tagencrypt2 encrypt2;
	//CMySqlDataBase mysqldb;
	activeUser activeuser;
// Dialog Data
	//{{AFX_DATA(CListenActiveDlg)
	enum { IDD = IDD_LISTENACTIVE_DIALOG };
	CButton	m_btn_stop;
	CButton	m_btn_start;
	CButton m_btnSetRegTime;	//����ע�Ṧ�ܿ����ر�ʱ��


	CExRichEdit	m_msg;

	//��ɾ�Ĳ�
	CButton	m_btn_query;		
	CButton	m_btn_UpdateConfig;			//��������
	CButton	m_btn_Modify;
	CButton	m_btn_Delete;

	//}}AFX_DATA
	void    DispMsg(CString Msg); //׷����ʾ����
	int operateActive();//������Ҫ���������
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListenActiveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	HICON m_hIcon;
	CYcDBEngine *		m_pDBEngine;		//���ݿ�����

	//��ַ����
	string						m_strServerAddr;			//��������ַ
	volatile unsigned short		m_ServerPort;				//�������˿�
	volatile bool				m_bRegisterEnable;			//ע��ʹ��
	volatile bool				m_bModifyPwdEnable;			//�޸�����ʹ��
	string						m_strMysqlConnectTimeOut;		//���ݿ����ӳ�ʱʱ��
	SOCKET						m_hMeSock;					//�����������׽���
	string						m_strMysqlAdminAccount;		//���ݿ����Ա�ʺ�
	string						m_strMysqlAdminPwd;			//���ݿ����Ա����
	string						m_strMysqlDBName;			//���ݿ���
	WSAEVENT					m_eventMe;					//�Լ����첽�¼�
	volatile bool				m_bMonitor;					//����ʹ��

	//ע��ʱ������
public:
	INT							m_nStartHour;				//ʱ
	INT							m_nStartMin;				//��
	INT							m_nStartSec;				//��
	INT							m_nEndHour;					//ʱ
	INT							m_nEndMin;					//��
	INT							m_nEndSec;					//��

	// Generated message map functions
	//{{AFX_MSG(CListenActiveDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton1();
	afx_msg void OnButton2();

	//��ɾ�Ĳ�
public:
	//��������
	afx_msg void OnQuery();
	afx_msg void OnRegister();
	afx_msg void OnModify();
	afx_msg void OnDelete();

	//��������
	afx_msg void OnRefreshConfig();

	//��ȡip
	string GetStrIP(){ return m_strServerAddr; }
	//��ȡ�˿�
	unsigned short GetPort(){ return m_ServerPort; }
	//��ȡע��ʹ��
	bool	GetRegisterEnable(){ return m_bRegisterEnable; }
	//��ȡ�޸�����ʹ��
	bool   GetModifyPwdEnalbe(){ return m_bModifyPwdEnable; }
	//��ȡ��������ʱʱ��
	string GetMysqlConnectTimeOut(){ return m_strMysqlConnectTimeOut; }
	//���ü����׽���
	void SetMinitorSocke(SOCKET hSock){ m_hMeSock = hSock; }
	//�Լ��������¼�
	void SetMonitorEvent(WSAEVENT eventMe){ m_eventMe = eventMe; }
	//���÷���ʹ��
	void SetServerRunFlag(BOOL bRun);
	bool GetMonitor(){ return m_bMonitor; }

	


	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedRegTimeSet();
	//ע�����
	void RegEnabelAnalys();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTENACTIVEDLG_H__8D8F764C_D0C3_4EDD_ACAA_C80710B7EEC3__INCLUDED_)
