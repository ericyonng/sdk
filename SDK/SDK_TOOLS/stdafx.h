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


#include"../CommonModul/CommonModulHead.h"
#pragma comment(lib, "../库文件/Unicode/CommonModul.lib")
#include"../Thread/ThreadHead.h"
#pragma comment(lib, "../库文件/Unicode/Thread.lib")


#include "../结构定义/Public.h"
#include<string>
using namespace std;


