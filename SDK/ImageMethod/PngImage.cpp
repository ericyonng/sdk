#include "StdAfx.h"
#include "PngImage.h"
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPngImage::CPngImage()
{
	//���ñ���
	m_pImage=NULL;
	ZeroMemory(m_strImageName, sizeof(m_strImageName) );

	//�Ƿ�����
	m_bIsScaleImage = false;
	m_rScaleX = 1.0;
	m_rScaleY = 1.0;

	return;
}

//��������
CPngImage::~CPngImage()
{
	//����ͼƬ
	DestroyImage(); 

	return;
}

//�Ƿ����
bool CPngImage::IsNull()
{
	//�����ж�
	if (m_pImage==NULL) return true;
	if (m_pImage->GetLastStatus()!=Ok) return true;

	return false;
}

//��ȡ���
INT CPngImage::GetWidth()
{
	//�����ж�
	ASSERT(IsNull()==false);
	if (IsNull()==true) return 0;

	//��ȡ���
	return m_pImage->GetWidth();
}

//��ȡ�߶�
INT CPngImage::GetHeight()
{
	//�����ж�
	ASSERT(IsNull()==false);
	if (IsNull()==true) return 0;

	//��ȡ�߶�
	return m_pImage->GetHeight();
}

//����ͼƬ
bool CPngImage::DestroyImage()
{
	//ɾ������
	if (m_pImage!=NULL) SafeDelete(m_pImage);

	return true;
}

//����ͼƬ
bool CPngImage::LoadImage(LPCTSTR pszFileName)
{
	//�����ж�
	ASSERT(m_pImage==NULL);
	if (m_pImage!=NULL) return false;

	//�����ļ�
	CT2CW strFileName(pszFileName);
	m_pImage=Image::FromFile((LPCWSTR)strFileName);

	//�����ж�
	if ((m_pImage==NULL)||(m_pImage->GetLastStatus()!=Ok)) 
	{
		DestroyImage();
		return false;
	}

	return true;
}

//����ͼƬ
bool CPngImage::LoadImage(HINSTANCE hInstance, LPCTSTR pszResourceName)
{
	//�����ж�
	ASSERT(m_pImage==NULL);
	if (m_pImage!=NULL) return false;

	//������Դ
	HRSRC hResource=FindResource(hInstance,pszResourceName,TEXT("PNG"));
	if (hResource==NULL) return false;

	//��ȡ��Դ
	DWORD dwImageSize=SizeofResource(hInstance,hResource);
	LPVOID pImageBuffer=LoadResource(hInstance,hResource);

	//��������
	IStream * pIStream=NULL;
	if (CreateStreamOnHGlobal(NULL,TRUE,&pIStream)!=S_OK)
	{
		ASSERT(FALSE);
		return false;
	}

	//д������
	pIStream->Write(pImageBuffer,dwImageSize,NULL);

	//����λͼ
	m_pImage=Image::FromStream(pIStream);

	//�ͷ���Դ
	SafeRelease(pIStream);

	//�����ж�
	if ((m_pImage==NULL)||(m_pImage->GetLastStatus()!=Ok))
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//�滭ͼ��
bool CPngImage::DrawImage(CDC * pDC, INT nXPos, INT nYPos)
{
	//�����ж�
	ASSERT(m_pImage!=NULL);
	if (m_pImage==NULL) return false;

	//������Ļ
	ASSERT(pDC!=NULL);
	Graphics graphics(pDC->GetSafeHdc());

	//��ȡ����
	INT nImageWidth=m_pImage->GetWidth();
	INT nImageHeight=m_pImage->GetHeight();

	//����λ��
	RectF rcDrawRect;
	rcDrawRect.X=(REAL)nXPos;
	rcDrawRect.Y=(REAL)nYPos;
	rcDrawRect.Width=(REAL)nImageWidth;
	rcDrawRect.Height=(REAL)nImageHeight;

	//������������
	if (m_bIsScaleImage)
	{
		rcDrawRect.X /= m_rScaleX;
		rcDrawRect.Y /= m_rScaleY;
		SetScaleTransform(graphics); 
	}

	//�滭ͼ��
	graphics.DrawImage(m_pImage,rcDrawRect,0,0,(REAL)nImageWidth,(REAL)nImageHeight,UnitPixel);

	//�Ƿ�����
	m_bIsScaleImage = false;
	m_rScaleX = 1.0;
	m_rScaleY = 1.0;

	return true;
}

//�滭ͼ��
bool CPngImage::DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT nYSrc)
{
	//�����ж�
	ASSERT(m_pImage!=NULL);
	if (m_pImage==NULL) return false;

	//������Ļ
	ASSERT(pDC!=NULL);
	Graphics graphics(pDC->GetSafeHdc());

	//����λ��
	RectF rcDrawRect;
	rcDrawRect.X=(REAL)nXDest;
	rcDrawRect.Y=(REAL)nYDest;
	rcDrawRect.Width=(REAL)nDestWidth;
	rcDrawRect.Height=(REAL)nDestHeight;

	//������������
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

	//�滭ͼ��
	graphics.DrawImage(m_pImage, rcDrawRect, (REAL)rXScr, (REAL)rYScr, (REAL)rDestWidth, (REAL)rDestHeight, UnitPixel);

	//�ָ�
	m_bIsScaleImage = false;
	m_rScaleX = 1.0;
	m_rScaleY = 1.0;

	return true;
}

//�滭ͼ��
bool CPngImage::DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT nYSrc, INT nSrcWidth, INT nSrcHeight)
{
	//�����ж�
	ASSERT(m_pImage!=NULL);
	if (m_pImage==NULL) return false;

	//������Ļ
	ASSERT(pDC!=NULL);
	Graphics graphics(pDC->GetSafeHdc());

	//����λ��
	RectF rcDrawRect;
	rcDrawRect.X=(REAL)nXDest;
	rcDrawRect.Y=(REAL)nYDest;
	rcDrawRect.Width=(REAL)nDestWidth;
	rcDrawRect.Height=(REAL)nDestHeight;

	//������������
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

	//�滭ͼ��
	graphics.DrawImage(m_pImage, rcDrawRect, (REAL)rXScr, (REAL)rYScr, (REAL)nSrcWidth, (REAL)nSrcHeight, UnitPixel);

	//�ָ�
	m_bIsScaleImage = false;
	m_rScaleX = 1.0;
	m_rScaleY = 1.0;

	return true;
}

//��ϻ滭
bool CPngImage::AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, BYTE cbAlphaDepth)
{
	//͸���滭
	AlphaDrawImage(pDestDC,xDest,yDest,GetWidth(),GetHeight(),0,0,cbAlphaDepth);

	return true;
}

//��ϻ滭
bool CPngImage::AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, BYTE cbAlphaDepth)
{
	//�����ж�
	ASSERT(m_pImage!=NULL);
	if (m_pImage==NULL) return false;

	//������Ļ
	ASSERT(pDestDC!=NULL);
	Graphics graphics(pDestDC->GetSafeHdc());

	//����λ��
	RectF rcDrawRect;
	rcDrawRect.X=(REAL)xDest;
	rcDrawRect.Y=(REAL)yDest;
	rcDrawRect.Width=(REAL)cxDest;
	rcDrawRect.Height=(REAL)cyDest;

	//͸������
	ColorMatrix Matrix=
	{
		1.0f,0.0f,0.0f,0.0f,0.0f, 
		0.0f,1.0f,0.0f,0.0f,0.0f, 
		0.0f,0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,cbAlphaDepth/255.0f,0.0f, 
		0.0f,0.0f,0.0f,0.0f,1.0f
	};

	//��������
	ImageAttributes Attributes; 
	Attributes.SetColorMatrix(&Matrix,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap); 

	//������������
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

	//�滭ͼ��
	graphics.DrawImage(m_pImage, rcDrawRect, (REAL)rxSrc, (REAL)rySrc, (REAL)rcxDest, (REAL)rcyDest, UnitPixel, &Attributes);

	//�ָ�
	m_bIsScaleImage = false;
	m_rScaleX = 1.0;
	m_rScaleY = 1.0;

	return true;
}

//��ϻ滭
bool CPngImage::AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, INT cxSrc, INT cySrc, BYTE cbAlphaDepth)
{
	//��������
	if ((cxDest!=cxSrc)||(cyDest!=cySrc))
	{
		//�����ж�
		ASSERT(m_pImage!=NULL);
		if (m_pImage==NULL) return false;

		//������Ļ
		ASSERT(pDestDC!=NULL);
		Graphics graphics(pDestDC->GetSafeHdc());

		//����λ��
		RectF rcDrawRect;
		rcDrawRect.X=(REAL)xDest;
		rcDrawRect.Y=(REAL)yDest;
		rcDrawRect.Width=(REAL)cxDest;
		rcDrawRect.Height=(REAL)cyDest;

		//͸������
		ColorMatrix Matrix=
		{
			1.0f,0.0f,0.0f,0.0f,0.0f, 
			0.0f,1.0f,0.0f,0.0f,0.0f, 
			0.0f,0.0f,1.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,cbAlphaDepth/255.0f,0.0f, 
			0.0f,0.0f,0.0f,0.0f,1.0f
		};

		//��������
		ImageAttributes Attributes; 
		Attributes.SetColorMatrix(&Matrix,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap); 

		//������������
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
		
		//�滭ͼ��
		graphics.DrawImage(m_pImage, rcDrawRect, (REAL)rxSrc, (REAL)rySrc, (REAL)cxSrc, (REAL)cySrc, UnitPixel, &Attributes);
	}
	else
	{
		//��ͨ����
		AlphaDrawImage(pDestDC,xDest,yDest,cxDest,cyDest,xSrc,ySrc,cbAlphaDepth);
	}

	//�ָ�
	m_bIsScaleImage = false;
	m_rScaleX = 1.0;
	m_rScaleY = 1.0;

	return true;
}

//��������
bool CPngImage::SetScaleTransform(Gdiplus::Graphics &gdi)
{
	if (gdi.ScaleTransform(m_rScaleX, m_rScaleY) == 0) return true;

	return false;
}

//ƽ�̻滭
bool CPngImage::DrawImageTile(CDC * pDC, CRect & rcDestRect)
{
	//ƽ�̻滭
	return DrawImageTile(pDC, rcDestRect.left, rcDestRect.top, rcDestRect.Width(), rcDestRect.Height());
}
//ƽ�̻滭
bool CPngImage::DrawImageTile(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//Ч��״̬
	ASSERT(IsNull() == false);
	if (IsNull() == true) return false;

	//��Ч����
	CRect rcDirty;
	pDC->GetClipBox(&rcDirty);

	//��������
	INT nImageWidth = GetWidth();
	INT nImageHeight = GetHeight();

	//ƽ�̻滭
	for (INT nXDrawPos = 0; nXDrawPos<nWidth; nXDrawPos += nImageWidth)
	{
		for (INT nYDrawPos = 0; nYDrawPos<nHeight; nYDrawPos += nImageHeight)
		{
			//����λ��
			INT nDrawWidth = __min(nWidth - nXDrawPos, nImageWidth);
			INT nDrawHeight = __min(nHeight - nYDrawPos, nImageHeight);

			//�滭ͼ��
			bool bIntersect = IsRectIntersect(rcDirty, nXDrawPos + nXPos, nYDrawPos + nYPos, nDrawWidth, nDrawHeight);
			if (bIntersect == true) DrawImage(pDC, nXDrawPos + nXPos, nYDrawPos + nYPos, nDrawWidth, nDrawHeight, 0, 0, nDrawWidth, nDrawHeight);
		}
	}

	return true;
}

//���μ��
bool CPngImage::IsRectIntersect(const CRect & rcRect, INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//��Χ���
	if ((nXPos>rcRect.right) || (nYPos>rcRect.bottom)) return false;
	if (((nXPos + nWidth)<rcRect.left) || ((nYPos + nHeight)<rcRect.top)) return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
