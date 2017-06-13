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
typedef char *(_stdcall *tagencrypt1)(char *,char *);//第一次加密
typedef char *(_stdcall *tagencrypt2)(char *key,char *mail);//第二次加密
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

	//写入配置表
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

	//父窗口
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
	CButton m_btnSetRegTime;	//设置注册功能开启关闭时间


	CExRichEdit	m_msg;

	//增删改查
	CButton	m_btn_query;		
	CButton	m_btn_UpdateConfig;			//更新配置
	CButton	m_btn_Modify;
	CButton	m_btn_Delete;

	//}}AFX_DATA
	void    DispMsg(CString Msg); //追加显示内容
	int operateActive();//处理需要激活的数据
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListenActiveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	HICON m_hIcon;
	CYcDBEngine *		m_pDBEngine;		//数据库引擎

	//地址配置
	string						m_strServerAddr;			//服务器地址
	volatile unsigned short		m_ServerPort;				//服务器端口
	volatile bool				m_bRegisterEnable;			//注册使能
	volatile bool				m_bModifyPwdEnable;			//修改密码使能
	string						m_strMysqlConnectTimeOut;		//数据库链接超时时间
	SOCKET						m_hMeSock;					//服务器监视套接字
	string						m_strMysqlAdminAccount;		//数据库管理员帐号
	string						m_strMysqlAdminPwd;			//数据库管理员密码
	string						m_strMysqlDBName;			//数据库名
	WSAEVENT					m_eventMe;					//自己的异步事件
	volatile bool				m_bMonitor;					//监视使能

	//注册时间设置
public:
	INT							m_nStartHour;				//时
	INT							m_nStartMin;				//分
	INT							m_nStartSec;				//秒
	INT							m_nEndHour;					//时
	INT							m_nEndMin;					//分
	INT							m_nEndSec;					//秒

	// Generated message map functions
	//{{AFX_MSG(CListenActiveDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton1();
	afx_msg void OnButton2();

	//增删改查
public:
	//启动服务
	afx_msg void OnQuery();
	afx_msg void OnRegister();
	afx_msg void OnModify();
	afx_msg void OnDelete();

	//更新配置
	afx_msg void OnRefreshConfig();

	//获取ip
	string GetStrIP(){ return m_strServerAddr; }
	//获取端口
	unsigned short GetPort(){ return m_ServerPort; }
	//获取注册使能
	bool	GetRegisterEnable(){ return m_bRegisterEnable; }
	//获取修改密码使能
	bool   GetModifyPwdEnalbe(){ return m_bModifyPwdEnable; }
	//获取服务器超时时间
	string GetMysqlConnectTimeOut(){ return m_strMysqlConnectTimeOut; }
	//设置监视套接字
	void SetMinitorSocke(SOCKET hSock){ m_hMeSock = hSock; }
	//自己的网络事件
	void SetMonitorEvent(WSAEVENT eventMe){ m_eventMe = eventMe; }
	//设置服务使能
	void SetServerRunFlag(BOOL bRun);
	bool GetMonitor(){ return m_bMonitor; }

	


	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedRegTimeSet();
	//注册分析
	void RegEnabelAnalys();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTENACTIVEDLG_H__8D8F764C_D0C3_4EDD_ACAA_C80710B7EEC3__INCLUDED_)
