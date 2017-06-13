#ifndef ADMIN_WND_H_
#define ADMIN_WND_H_

#pragma once
#include"resource.h"

class CAdminDlg : public CDialog
{
	// Construction
public:
	CAdminDlg(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_DIALOG1 };

	//ืสิด
	CEdit					m_EditIP;
	CEdit					m_EditPort;
	CEdit					m_EditFile;
	CButton					m_BtnGenerate;
	HICON					m_hIcon;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// Generated message map functions
	//{{AFX_MSG(CListenActiveDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();




	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedGenerate();
};

#endif