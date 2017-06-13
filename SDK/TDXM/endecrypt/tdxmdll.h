// tdxmdll.h : main header file for the TDXMDLL DLL
//

#if !defined(AFX_TDXMDLL_H__82098895_365C_46E7_A01B_A0F5F562AEA7__INCLUDED_)
#define AFX_TDXMDLL_H__82098895_365C_46E7_A01B_A0F5F562AEA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTdxmdllApp
// See tdxmdll.cpp for the implementation of this class
//

class CTdxmdllApp : public CWinApp
{
public:
	CTdxmdllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTdxmdllApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTdxmdllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDXMDLL_H__82098895_365C_46E7_A01B_A0F5F562AEA7__INCLUDED_)
