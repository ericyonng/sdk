#ifndef YC_IMAGE_HEAD_FILE
#define YC_IMAGE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//包含文件

//MFC 文件
#include <GdiPlus.h>
#include <AtlImage.h>

//命名空间
using namespace Gdiplus;

//////////////////////////////////////////////////////////////////////////////////
//公共定义

//导出定义
#ifndef YC_IMAGE_CLASS
	#ifdef  YC_IMAGE_DLL
		#define YC_IMAGE_CLASS _declspec(dllexport)
	#else
		#define YC_IMAGE_CLASS _declspec(dllimport)
	#endif
#endif

//模块定义
#ifndef _DEBUG
	#define YC_IMAGE_DLL_NAME		TEXT("YCImage.dll")					//组件名字
#else
	#define YC_IMAGE_DLL_NAME		TEXT("YCImageD.dll")				//组件名字
#endif

//////////////////////////////////////////////////////////////////////////////////
//导出文件

#ifndef YC_IMAGE_DLL
	#include "BitImage.h"
	#include "PngImage.h"
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif