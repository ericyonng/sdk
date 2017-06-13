#ifndef YCDUI_LIB_HEAD_FILE
#define YCDUI_LIB_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�궨��
#ifdef YCDUI_LIB_DLL
#define YCDUI_LIB_CLASS __declspec(dllexport)
#else
#define YCDUI_LIB_CLASS __declspec(dllimport)
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define YCDUILIB_DLL_NAME	TEXT("DuiLib.dll")			//�������
#else
	#define YCDUILIB_DLL_NAME	TEXT("DuiLibD.dll")		//�������
#endif

//////////////////////////////////////////////////////////////////////////////////

//�����ļ�
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
