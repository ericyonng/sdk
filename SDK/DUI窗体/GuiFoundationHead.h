#ifndef MFCGUIFOUNDATION_HEAD_FILE
#define MFCGUIFOUNDATION_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�
#pragma once

//����ļ�
#include "..\DUI��\YCDuiLibHead.h"

//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef GUI_FOUNDATION_CLASS
#ifdef  GUI_FOUNDATION_DLL
		#define GUI_FOUNDATION_CLASS _declspec(dllexport)
	#else
		#define GUI_FOUNDATION_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define	GUI_FOUNDATION_DLL_NAME	TEXT("GuiFoundation.dll")		//�������
#else
	#define GUI_FOUNDATION_DLL_NAME	TEXT("GuiFoundationD.dll")		//�������
#endif

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

#ifndef GUI_FOUNDATION_DLL
	#include "YCDuiWindow.h"

	//�˾�����
	#include "FilterWnd.h"
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif