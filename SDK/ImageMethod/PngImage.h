#ifndef PNG_IMAGE_HEAD_FILE
#define PNG_IMAGE_HEAD_FILE

#pragma once

#include "YCImageHead.h"

//////////////////////////////////////////////////////////////////////////////////

//ͼƬ����
class YC_IMAGE_CLASS CPngImage
{
	//��������
protected:
	Image *							m_pImage;							//ͼƬ����
	TCHAR							m_strImageName[MAX_PATH];			//ͼƬ����

//������������
public:
	bool m_bIsScaleImage;
	REAL m_rScaleX;		//���ű�=Ŀ��/Դ
	REAL m_rScaleY;
	
	//��������
protected:
	//��������
	bool SetScaleTransform(Gdiplus::Graphics &gdi);
	//���μ��
	bool IsRectIntersect(const CRect & rcRect, INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//��������
public:
	//���캯��
	CPngImage();
	//��������
	virtual ~CPngImage();

	//״̬����
public:
	//�Ƿ����
	bool IsNull();
	//��ȡ����
	INT GetWidth();
	//��ȡ�߶�
	INT GetHeight();

	//��������
public:
	//����ͼƬ
	bool DestroyImage();
	//����ͼƬ
	bool LoadImage(LPCTSTR pszFileName);
	//����ͼƬ
	bool LoadImage(HINSTANCE hInstance, LPCTSTR pszResourceName);

	//�滭����
public:
	//�滭ͼ��
	bool DrawImage(CDC * pDC, INT nXPos, INT nYPos);
	//�滭ͼ��
	bool DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT nYSrc);
	//�滭ͼ��
	bool DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT nYSrc, INT nSrcWidth, INT nSrcHeight);

	//͸���滭
public:
	//��ϻ滭
	bool AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, BYTE cbAlphaDepth);
	//��ϻ滭
	bool AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, BYTE cbAlphaDepth);
	//��ϻ滭
	bool AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, INT cxSrc, INT cySrc, BYTE cbAlphaDepth);

	//�滭ͼƬ
public:
	//ƽ�̻滭
	bool DrawImageTile(CDC * pDC, CRect & rcDestRect);
	//ƽ�̻滭
	bool DrawImageTile(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight);
};

//////////////////////////////////////////////////////////////////////////////////

#endif