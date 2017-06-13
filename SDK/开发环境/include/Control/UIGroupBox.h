#ifndef __UIGROUPBOX_H__
#define __UIGROUPBOX_H__

#pragma once

#define  GROUPBOX_TEXT_OFFSET		20		//定义GroupBox中的Text相对于左边的偏移

namespace DuiLib
{

	class UILIB_API CGroupBoxUI : public CVerticalLayoutUI
	{
		DECLARE_DUICONTROL(CGroupBoxUI)
	public:
		CGroupBoxUI();
		~CGroupBoxUI();
		 LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void SetTextColor(DWORD dwTextColor);
		DWORD GetTextColor() const;
		void SetDisabledTextColor(DWORD dwTextColor);
		DWORD GetDisabledTextColor() const;
		void SetFont(int index);
		int GetFont() const;

	protected:	
		//Paint
		virtual void PaintText(HDC hDC);
		virtual void PaintBorder(HDC hDC);
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	private:
		SIZE CalcrectSize(SIZE szAvailable);

	protected:
		DWORD m_dwTextColor;
		DWORD m_dwDisabledTextColor;
		int m_iFont;
		UINT m_uTextStyle;
		RECT m_rcTextPadding;
	};
}
#endif // __UIGROUPBOX_H__