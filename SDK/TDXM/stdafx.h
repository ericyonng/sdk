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

//MFC 文件
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>

//#include"../DUI库/YCDuiLibHead.h"
#include"ClientDefine.h"

#pragma comment(lib, "../库文件/Unicode/Thread.lib")
#pragma comment(lib, "../库文件/Unicode/NetworkService.lib")

//多媒体库
#pragma comment (lib,"Winmm.lib")

//加解密
#pragma comment (lib, "./endecrypt/tdxmdll.lib")

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../库文件/Ansi/YCImage.lib")
#pragma comment (lib,"../库文件/Ansi/FGuilib.lib")
#pragma comment (lib,"../库文件/Ansi/GuiFoundation.lib")	
#else
#pragma comment (lib,"../库文件/Unicode/YCImage.lib")
#pragma comment (lib,"../库文件/Unicode/FGuilib.lib")
#pragma comment (lib,"../库文件/Unicode/GuiFoundation.lib")	
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../库文件/Ansi/YCImageD.lib")
#pragma comment (lib,"../库文件/Ansi/FGuilibD.lib")
#pragma comment (lib,"../库文件/Ansi/GuiFoundationD.lib")	
#else
#pragma comment (lib,"../库文件/Unicode/YCImageD.lib")
#pragma comment (lib,"../库文件/Unicode/FGuilibD.lib")
#pragma comment (lib,"../库文件/Unicode/GuiFoundationD.lib")		
#endif
#endif






