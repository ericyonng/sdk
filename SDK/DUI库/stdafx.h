#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN	
#endif

#ifndef WINVER				
#define WINVER 0x0500	
#endif

#ifndef _WIN32_WINNT		
#define _WIN32_WINNT 0x0500
#endif						

#ifndef _WIN32_WINDOWS	
#define _WIN32_WINDOWS 0x0500 
#endif

#ifndef _WIN32_IE			
#define _WIN32_IE 0x0500	
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#include <afxwin.h>    
#include <afxext.h>     

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         
#include <afxodlgs.h>       
#include <afxdisp.h>      
#endif

//宏定义
#define lengthof(x) (sizeof(x)/sizeof(*x))
#define MAX max
#define MIN min
#define CLAMP(x,a,b) (MIN(b,MAX(a,x)))

//引入文件
#include <windowsx.h>
#include "YCDuiLibHead.h"
