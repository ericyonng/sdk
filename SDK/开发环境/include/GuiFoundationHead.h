#ifndef MFCGUIFOUNDATION_HEAD_FILE
#define MFCGUIFOUNDATION_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//包含文件
#pragma once

//组件文件
#include "..\DUI库\YCDuiLibHead.h"

//////////////////////////////////////////////////////////////////////////////////
//公共定义

//导出定义
#ifndef GUI_FOUNDATION_CLASS
#ifdef  GUI_FOUNDATION_DLL
		#define GUI_FOUNDATION_CLASS _declspec(dllexport)
	#else
		#define GUI_FOUNDATION_CLASS _declspec(dllimport)
	#endif
#endif

//模块定义
#ifndef _DEBUG
	#define	GUI_FOUNDATION_DLL_NAME	TEXT("GuiFoundation.dll")		//组件名字
#else
	#define GUI_FOUNDATION_DLL_NAME	TEXT("GuiFoundationD.dll")		//组件名字
#endif

//////////////////////////////////////////////////////////////////////////////////
//导出文件

#ifndef GUI_FOUNDATION_DLL
	#include "YCDuiWindow.h"

	//滤镜窗体
	#include "FilterWnd.h"
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif