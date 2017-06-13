#ifndef PNG_IMAGE_HEAD_FILE
#define PNG_IMAGE_HEAD_FILE

#pragma once

#include "YCImageHead.h"

//////////////////////////////////////////////////////////////////////////////////

//图片对象
class YC_IMAGE_CLASS CPngImage
{
	//变量定义
protected:
	Image *							m_pImage;							//图片对象
	TCHAR							m_strImageName[MAX_PATH];			//图片名称

//设置缩放因子
public:
	bool m_bIsScaleImage;
	REAL m_rScaleX;		//缩放比=目标/源
	REAL m_rScaleY;
	
	//辅助函数
protected:
	//设置缩放
	bool SetScaleTransform(Gdiplus::Graphics &gdi);
	//矩形检查
	bool IsRectIntersect(const CRect & rcRect, INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//函数定义
public:
	//构造函数
	CPngImage();
	//析构函数
	virtual ~CPngImage();

	//状态函数
public:
	//是否加载
	bool IsNull();
	//获取宽度
	INT GetWidth();
	//获取高度
	INT GetHeight();

	//管理函数
public:
	//销毁图片
	bool DestroyImage();
	//加载图片
	bool LoadImage(LPCTSTR pszFileName);
	//加载图片
	bool LoadImage(HINSTANCE hInstance, LPCTSTR pszResourceName);

	//绘画函数
public:
	//绘画图像
	bool DrawImage(CDC * pDC, INT nXPos, INT nYPos);
	//绘画图像
	bool DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT nYSrc);
	//绘画图像
	bool DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT nYSrc, INT nSrcWidth, INT nSrcHeight);

	//透明绘画
public:
	//混合绘画
	bool AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, BYTE cbAlphaDepth);
	//混合绘画
	bool AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, BYTE cbAlphaDepth);
	//混合绘画
	bool AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, INT cxSrc, INT cySrc, BYTE cbAlphaDepth);

	//绘画图片
public:
	//平铺绘画
	bool DrawImageTile(CDC * pDC, CRect & rcDestRect);
	//平铺绘画
	bool DrawImageTile(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight);
};

//////////////////////////////////////////////////////////////////////////////////

#endif