// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__2524FF3C_7DC6_443C_8541_2AAE94FBC4A5__INCLUDED_)
#define AFX_STDAFX_H__2524FF3C_7DC6_443C_8541_2AAE94FBC4A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include"../Thread/ThreadHead.h"
#include"../NetworkService/NetworkServiceHead.h"
#include"ServerDefine.h"


#include"../CommonModul/CommonModulHead.h"
#pragma comment(lib, "../库文件/Unicode/CommonModul.lib")

#pragma comment(lib, "../库文件/Unicode/Thread.lib")
#pragma comment(lib, "../库文件/Unicode/NetworkService.lib")
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__2524FF3C_7DC6_443C_8541_2AAE94FBC4A5__INCLUDED_)
