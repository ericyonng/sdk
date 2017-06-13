#include "StdAfx.h"
#include "PngImage.h"
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPngImage::CPngImage()
{
	//设置变量
	m_pImage=NULL;
	ZeroMemory(m_strImageName, sizeof(m_strImageName) );

	//是否缩放
	m_bIsScaleImage = false;
	m_rScaleX = 1.0;
	m_rScaleY = 1.0;

	return;
}

//析构函数
CPngImage::~CPngImage()
{
	//销毁图片
	DestroyImage(); 

	return;
}

//是否加载
bool CPngImage::IsNull()
{
	//加载判断
	if (m_pImage==NULL) return true;
	if (m_pImage->GetLastStatus()!=Ok) return true;

	return false;
}

//获取宽度
INT CPngImage::GetWidth()
{
	//加载判断
	ASSERT(IsNull()==false);
	if (IsNull()==true) return 0;

	//获取宽度
	return m_pImage->GetWidth();
}

//获取高度
INT CPngImage::GetHeight()
{
	//加载判断
	ASSERT(IsNull()==false);
	if (IsNull()==true) return 0;

	//获取高度
	return m_pImage->GetHeight();
}

//销毁图片
bool CPngImage::DestroyImage()
{
	//删除对象
	if (m_pImage!=NULL) SafeDelete(m_pImage);

	return true;
}

//加载图片
bool CPngImage::LoadImage(LPCTSTR pszFileName)
{
	//加载判断
	ASSERT(m_pImage==NULL);
	if (m_pImage!=NULL) return false;

	//加载文件
	CT2CW strFileName(pszFileName);
	m_pImage=Image::FromFile((LPCWSTR)strFileName);

	//错误判断
	if ((m_pImage==NULL)||(m_pImage->GetLastStatus()!=Ok)) 
	{
		DestroyImage();
		return false;
	}

	return true;
}

//加载图片
bool CPngImage::LoadImage(HINSTANCE hInstance, LPCTSTR pszResourceName)
{
	//加载判断
	ASSERT(m_pImage==NULL);
	if (m_pImage!=NULL) return false;

	//查找资源
	HRSRC hResource=FindResource(hInstance,pszResourceName,TEXT("PNG"));
	if (hResource==NULL) return false;

	//读取资源
	DWORD dwImageSize=SizeofResource(hInstance,hResource);
	LPVOID pImageBuffer=LoadResource(hInstance,hResource);

	//创建数据
	IStream * pIStream=NULL;
	if (CreateStreamOnHGlobal(NULL,TRUE,&pIStream)!=S_OK)
	{
		ASSERT(FALSE);
		return false;
	}

	//写入数据
	pIStream->Write(pImageBuffer,dwImageSize,NULL);

	//创建位图
	m_pImage=Image::FromStream(pIStream);

	//释放资源
	SafeRelease(pIStream);

	//错误判断
	if ((m_pImage==NULL)||(m_pImage->GetLastStatus()!=Ok))
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//绘画图像
bool CPngImage::DrawImage(CDC * pDC, INT nXPos, INT nYPos)
{
	//加载判断
	ASSERT(m_pImage!=NULL);
	if (m_pImage==NULL) return false;

	//创建屏幕
	ASSERT(pDC!=NULL);
	Graphics graphics(pDC->GetSafeHdc());

	//获取属性
	INT nImageWidth=m_pImage->GetWidth();
	INT nImageHeight=m_pImage->GetHeight();

	//构造位置
	RectF rcDrawRect;
	rcDrawRect.X=(REAL)nXPos;
	rcDrawRect.Y=(REAL)nYPos;
	rcDrawRect.Width=(REAL)nImageWidth;
	rcDrawRect.Height=(REAL)nImageHeight;

	//设置缩放因子
	if (m_bIsScaleImage)
	{
		rcDrawRect.X /= m_rScaleX;
		rcDrawRect.Y /= m_rScaleY;
		SetScaleTransform(graphics); 
	}

	//绘画图像
	graphics.DrawImage(m_pImage,rcDrawRect,0,0,(REAL)nImageWidth,(REAL)nImageHeight,UnitPixel);

	//是否缩放
	m_bIsScaleImage = false;
	m_rScaleX = 1.0;
	m_rScaleY = 1.0;

	return true;
}

//绘画图像
bool CPngImage::DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT nYSrc)
{
	//加载判断
	ASSERT(m_pImage!=NULL);
	if (m_pImage==NULL) return false;

	//创建屏幕
	ASSERT(pDC!=NULL);
	Graphics graphics(pDC->GetSafeHdc());

	//构造位置
	RectF rcDrawRect;
	rcDrawRect.X=(REAL)nXDest;
	rcDrawRect.Y=(REAL)nYDest;
	rcDrawRect.Width=(REAL)nDestWidth;
	rcDrawRect.Height=(REAL)nDestHeight;

	//设置缩放因子
	REAL rXScr = nXScr, rYScr = nYSrc, rDestWidth = nDestWidth, rDestHeight = nDestHeight;
	if (m_bIsScaleImage)
	{
		rcDrawRect.X /= m_rScaleX;
		rcDrawRect.Y /= m_rScaleY;
		rcDrawRect.Width /= m_rScaleX;
		rcDrawRect.Height /= m_rScaleY;
		rDestWidth /= m_rScaleX;
		rDestHeight /= m_rScaleY;
		SetScaleTransform(graphics);
	}

	//绘画图像
	graphics.DrawImage(m_pImage, rcDrawRect, (REAL)rXScr, (REAL)rYScr, (REAL)rDestWidth, (REAL)rDestHeight, UnitPixel);

	//恢复
	m_bIsScaleImage = false;
	m_rScaleX = 1.0;
	m_rScaleY = 1.0;

	return true;
}

//绘画图像
bool CPngImage::DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT nYSrc, INT nSrcWidth, INT nSrcHeight)
{
	//加载判断
	ASSERT(m_pImage!=NULL);
	if (m_pImage==NULL) return false;

	//创建屏幕
	ASSERT(pDC!=NULL);
	Graphics graphics(pDC->GetSafeHdc());

	//构造位置
	RectF rcDrawRect;
	rcDrawRect.X=(REAL)nXDest;
	rcDrawRect.Y=(REAL)nYDest;
	rcDrawRect.Width=(REAL)nDestWidth;
	rcDrawRect.Height=(REAL)nDestHeight;

	//设置缩放因子
	REAL rXScr = nXScr, rYScr=nYSrc;
	if (m_bIsScaleImage)
	{
		rcDrawRect.X /= m_rScaleX;
		rcDrawRect.Y /= m_rScaleY;
		rcDrawRect.Width /= m_rScaleX;
		rcDrawRect.Height /= m_rScaleY;
		//rXScr *= m_rScaleX;
		//rYScr *= m_rScaleY;
		SetScaleTransform(graphics);
	}

	//绘画图像
	graphics.DrawImage(m_pImage, rcDrawRect, (REAL)rXScr, (REAL)rYScr, (REAL)nSrcWidth, (REAL)nSrcHeight, UnitPixel);

	//恢复
	m_bIsScaleImage = false;
	m_rScaleX = 1.0;
	m_rScaleY = 1.0;

	return true;
}

//混合绘画
bool CPngImage::AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, BYTE cbAlphaDepth)
{
	//透明绘画
	AlphaDrawImage(pDestDC,xDest,yDest,GetWidth(),GetHeight(),0,0,cbAlphaDepth);

	return true;
}

//混合绘画
bool CPngImage::AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, BYTE cbAlphaDepth)
{
	//加载判断
	ASSERT(m_pImage!=NULL);
	if (m_pImage==NULL) return false;

	//创建屏幕
	ASSERT(pDestDC!=NULL);
	Graphics graphics(pDestDC->GetSafeHdc());

	//构造位置
	RectF rcDrawRect;
	rcDrawRect.X=(REAL)xDest;
	rcDrawRect.Y=(REAL)yDest;
	rcDrawRect.Width=(REAL)cxDest;
	rcDrawRect.Height=(REAL)cyDest;

	//透明矩阵
	ColorMatrix Matrix=
	{
		1.0f,0.0f,0.0f,0.0f,0.0f, 
		0.0f,1.0f,0.0f,0.0f,0.0f, 
		0.0f,0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,cbAlphaDepth/255.0f,0.0f, 
		0.0f,0.0f,0.0f,0.0f,1.0f
	};

	//设置属性
	ImageAttributes Attributes; 
	Attributes.SetColorMatrix(&Matrix,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap); 

	//设置缩放因子
	REAL rxSrc = xSrc, rySrc = ySrc, rcxDest = cxDest, rcyDest = cyDest;
	if (m_bIsScaleImage)
	{
		rcDrawRect.X /= m_rScaleX;
		rcDrawRect.Y /= m_rScaleY;
		rcDrawRect.Width /= m_rScaleX;
		rcDrawRect.Height /= m_rScaleY;
		rcxDest /= m_rScaleX;
		rcyDest /= m_rScaleY;
		//rxSrc *= m_rScaleX;
		//rySrc *= m_rScaleY;
		SetScaleTransform(graphics);
	}

	//绘画图像
	graphics.DrawImage(m_pImage, rcDrawRect, (REAL)rxSrc, (REAL)rySrc, (REAL)rcxDest, (REAL)rcyDest, UnitPixel, &Attributes);

	//恢复
	m_bIsScaleImage = false;
	m_rScaleX = 1.0;
	m_rScaleY = 1.0;

	return true;
}

//混合绘画
bool CPngImage::AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, INT cxSrc, INT cySrc, BYTE cbAlphaDepth)
{
	//创建缓冲
	if ((cxDest!=cxSrc)||(cyDest!=cySrc))
	{
		//加载判断
		ASSERT(m_pImage!=NULL);
		if (m_pImage==NULL) return false;

		//创建屏幕
		ASSERT(pDestDC!=NULL);
		Graphics graphics(pDestDC->GetSafeHdc());

		//构造位置
		RectF rcDrawRect;
		rcDrawRect.X=(REAL)xDest;
		rcDrawRect.Y=(REAL)yDest;
		rcDrawRect.Width=(REAL)cxDest;
		rcDrawRect.Height=(REAL)cyDest;

		//透明矩阵
		ColorMatrix Matrix=
		{
			1.0f,0.0f,0.0f,0.0f,0.0f, 
			0.0f,1.0f,0.0f,0.0f,0.0f, 
			0.0f,0.0f,1.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,cbAlphaDepth/255.0f,0.0f, 
			0.0f,0.0f,0.0f,0.0f,1.0f
		};

		//设置属性
		ImageAttributes Attributes; 
		Attributes.SetColorMatrix(&Matrix,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap); 

		//设置缩放因子
		REAL rxSrc = xSrc, rySrc = ySrc;
		if (m_bIsScaleImage)
		{
			rcDrawRect.X /= m_rScaleX;
			rcDrawRect.Y /= m_rScaleY;
			//rxSrc *= m_rScaleX;
			//rySrc *= m_rScaleY;
			rcDrawRect.Width /= m_rScaleX;
			rcDrawRect.Height /= m_rScaleY;
			SetScaleTransform(graphics);
		}
		
		//绘画图像
		graphics.DrawImage(m_pImage, rcDrawRect, (REAL)rxSrc, (REAL)rySrc, (REAL)cxSrc, (REAL)cySrc, UnitPixel, &Attributes);
	}
	else
	{
		//普通调用
		AlphaDrawImage(pDestDC,xDest,yDest,cxDest,cyDest,xSrc,ySrc,cbAlphaDepth);
	}

	//恢复
	m_bIsScaleImage = false;
	m_rScaleX = 1.0;
	m_rScaleY = 1.0;

	return true;
}

//设置缩放
bool CPngImage::SetScaleTransform(Gdiplus::Graphics &gdi)
{
	if (gdi.ScaleTransform(m_rScaleX, m_rScaleY) == 0) return true;

	return false;
}

//平铺绘画
bool CPngImage::DrawImageTile(CDC * pDC, CRect & rcDestRect)
{
	//平铺绘画
	return DrawImageTile(pDC, rcDestRect.left, rcDestRect.top, rcDestRect.Width(), rcDestRect.Height());
}
//平铺绘画
bool CPngImage::DrawImageTile(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//效验状态
	ASSERT(IsNull() == false);
	if (IsNull() == true) return false;

	//无效区域
	CRect rcDirty;
	pDC->GetClipBox(&rcDirty);

	//变量定义
	INT nImageWidth = GetWidth();
	INT nImageHeight = GetHeight();

	//平铺绘画
	for (INT nXDrawPos = 0; nXDrawPos<nWidth; nXDrawPos += nImageWidth)
	{
		for (INT nYDrawPos = 0; nYDrawPos<nHeight; nYDrawPos += nImageHeight)
		{
			//计算位置
			INT nDrawWidth = __min(nWidth - nXDrawPos, nImageWidth);
			INT nDrawHeight = __min(nHeight - nYDrawPos, nImageHeight);

			//绘画图形
			bool bIntersect = IsRectIntersect(rcDirty, nXDrawPos + nXPos, nYDrawPos + nYPos, nDrawWidth, nDrawHeight);
			if (bIntersect == true) DrawImage(pDC, nXDrawPos + nXPos, nYDrawPos + nYPos, nDrawWidth, nDrawHeight, 0, 0, nDrawWidth, nDrawHeight);
		}
	}

	return true;
}

//矩形检查
bool CPngImage::IsRectIntersect(const CRect & rcRect, INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//范围检测
	if ((nXPos>rcRect.right) || (nYPos>rcRect.bottom)) return false;
	if (((nXPos + nWidth)<rcRect.left) || ((nYPos + nHeight)<rcRect.top)) return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
