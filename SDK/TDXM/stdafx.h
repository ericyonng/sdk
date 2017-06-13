#pragma once

//////////////////////////////////////////////////////////////////////////////////

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0501 //winXP+
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501 //winXP+
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0500 //windowsME
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0600  //IE6.0+
#endif

#define _ATL_ATTRIBUTES
#define _AFX_ALL_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

//////////////////////////////////////////////////////////////////////////////////

//MFC �ļ�
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>

//#include"../DUI��/YCDuiLibHead.h"
#include"ClientDefine.h"

#pragma comment(lib, "../���ļ�/Unicode/Thread.lib")
#pragma comment(lib, "../���ļ�/Unicode/NetworkService.lib")

//��ý���
#pragma comment (lib,"Winmm.lib")

//�ӽ���
#pragma comment (lib, "./endecrypt/tdxmdll.lib")

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../���ļ�/Ansi/YCImage.lib")
#pragma comment (lib,"../���ļ�/Ansi/FGuilib.lib")
#pragma comment (lib,"../���ļ�/Ansi/GuiFoundation.lib")	
#else
#pragma comment (lib,"../���ļ�/Unicode/YCImage.lib")
#pragma comment (lib,"../���ļ�/Unicode/FGuilib.lib")
#pragma comment (lib,"../���ļ�/Unicode/GuiFoundation.lib")	
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../���ļ�/Ansi/YCImageD.lib")
#pragma comment (lib,"../���ļ�/Ansi/FGuilibD.lib")
#pragma comment (lib,"../���ļ�/Ansi/GuiFoundationD.lib")	
#else
#pragma comment (lib,"../���ļ�/Unicode/YCImageD.lib")
#pragma comment (lib,"../���ļ�/Unicode/FGuilibD.lib")
#pragma comment (lib,"../���ļ�/Unicode/GuiFoundationD.lib")		
#endif
#endif






