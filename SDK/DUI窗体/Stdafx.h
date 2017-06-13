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


//////////////////////////////////////////////////////////////////////////////////
//链接代码

//多媒体库
#pragma comment (lib,"Winmm.lib")

#ifndef _DEBUG
#ifndef _UNICODE
	#pragma comment (lib,"../库文件/Ansi/FGuilib.lib")
#else
	#pragma comment (lib,"../库文件/Unicode/FGuilib.lib")
#endif
#else
#ifndef _UNICODE
	#pragma comment (lib,"../库文件/Ansi/FGuilibD.lib")
#else
	#pragma comment (lib,"../库文件/Unicode/FGuilibD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////
