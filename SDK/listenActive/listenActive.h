// listenActive.h : main header file for the LISTENACTIVE application
//

#if !defined(AFX_LISTENACTIVE_H__ABF83930_7248_4A38_8198_8210BB1959F4__INCLUDED_)
#define AFX_LISTENACTIVE_H__ABF83930_7248_4A38_8198_8210BB1959F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CListenActiveApp:
// See listenActive.cpp for the implementation of this class
//

class CListenActiveApp : public CWinApp
{
public:
	CListenActiveApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListenActiveApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CListenActiveApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTENACTIVE_H__ABF83930_7248_4A38_8198_8210BB1959F4__INCLUDED_)
