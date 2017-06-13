#ifndef YCDUI_LIB_HEAD_FILE
#define YCDUI_LIB_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//宏定义
#ifdef YCDUI_LIB_DLL
#define YCDUI_LIB_CLASS __declspec(dllexport)
#else
#define YCDUI_LIB_CLASS __declspec(dllimport)
#endif

//模块定义
#ifndef _DEBUG
	#define YCDUILIB_DLL_NAME	TEXT("DuiLib.dll")			//组件名字
#else
	#define YCDUILIB_DLL_NAME	TEXT("DuiLibD.dll")		//组件名字
#endif

//////////////////////////////////////////////////////////////////////////////////

//包含文件
#include "UIBase.h"
#include "UIManager.h"
#include "UIRender.h"	
#include "UIDelegate.h"
#include "UIControl.h"	
#include "UIContainer.h"
#include "UICommon.h"
#include "UIRichEdit.h"
#include "UIList.h"
#include "UICombo.h"	
#include "UIMarkup.h"
#include "UIResLoader.h"
#include "UIDlgBuilder.h"

//////////////////////////////////////////////////////////////////////////////////

#endif
