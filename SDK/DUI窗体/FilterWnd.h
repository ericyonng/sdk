//引入文件
#include "GuiFoundationHead.h"
//#include "Stdafx.h"

#ifndef FILTER_WND_H_
#define FILTER_WND_H_

#pragma once

//参数
#define FILTER_DEFAULT_TRANSPARENCY				128				//默认透明度

//颜色
#define FILTER_BACKGROUND_DEFAULT_COLOR			RGB(0,0,0)		//滤镜默认颜色


class GUI_FOUNDATION_CLASS CFilterWnd :public CFrameWnd
{
	//窗体属性
protected:
	INT							m_nTransparency;				//透明度
	COLORREF					m_crFilterColor;				//背景色
	CRect						m_rcFilter;						//位置

	//构造函数
public:
	CFilterWnd();
	~CFilterWnd();
	
	//功能函数
public:
	//设置透明度
	VOID	SetTransparency(INT nTransparency);
	//设置滤镜颜色
	VOID SetFilterColor(COLORREF crFilterColor);
	//设置位置尺寸
	VOID SetFilterRect(CRect rcFilter){ m_rcFilter = rcFilter; }

	//消息函数
protected:
	//重绘事件
	afx_msg void OnPaint();
	//创建窗口
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//消息处理函数
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//创建预处理
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


	DECLARE_MESSAGE_MAP()	//声明消息映射
};



#endif