#ifndef YC_IMAGE_HEAD_FILE
#define YC_IMAGE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

//MFC �ļ�
#include <GdiPlus.h>
#include <AtlImage.h>

//�����ռ�
using namespace Gdiplus;

//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef YC_IMAGE_CLASS
	#ifdef  YC_IMAGE_DLL
		#define YC_IMAGE_CLASS _declspec(dllexport)
	#else
		#define YC_IMAGE_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define YC_IMAGE_DLL_NAME		TEXT("YCImage.dll")					//�������
#else
	#define YC_IMAGE_DLL_NAME		TEXT("YCImageD.dll")				//�������
#endif

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

#ifndef YC_IMAGE_DLL
	#include "BitImage.h"
	#include "PngImage.h"
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif