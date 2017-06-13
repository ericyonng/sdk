//�����ļ�
#include "GuiFoundationHead.h"
//#include "Stdafx.h"

#ifndef FILTER_WND_H_
#define FILTER_WND_H_

#pragma once

//����
#define FILTER_DEFAULT_TRANSPARENCY				128				//Ĭ��͸����

//��ɫ
#define FILTER_BACKGROUND_DEFAULT_COLOR			RGB(0,0,0)		//�˾�Ĭ����ɫ


class GUI_FOUNDATION_CLASS CFilterWnd :public CFrameWnd
{
	//��������
protected:
	INT							m_nTransparency;				//͸����
	COLORREF					m_crFilterColor;				//����ɫ
	CRect						m_rcFilter;						//λ��

	//���캯��
public:
	CFilterWnd();
	~CFilterWnd();
	
	//���ܺ���
public:
	//����͸����
	VOID	SetTransparency(INT nTransparency);
	//�����˾���ɫ
	VOID SetFilterColor(COLORREF crFilterColor);
	//����λ�óߴ�
	VOID SetFilterRect(CRect rcFilter){ m_rcFilter = rcFilter; }

	//��Ϣ����
protected:
	//�ػ��¼�
	afx_msg void OnPaint();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��Ϣ������
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//����Ԥ����
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


	DECLARE_MESSAGE_MAP()	//������Ϣӳ��
};



#endif