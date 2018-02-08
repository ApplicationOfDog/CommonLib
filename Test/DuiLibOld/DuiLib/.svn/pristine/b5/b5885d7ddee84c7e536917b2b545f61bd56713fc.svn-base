#ifndef __UIICON_H__
#define __UIICON_H__

#pragma once

namespace DuiLib
{
	/**
	* 自定义的图标显示控件，用于显示ico类型
	* @brief Icon图标控件
	* @author sundh
	*/
	class DUILIB_API CIconUI : public CControlUI
	{
	public:
		CIconUI();
		~CIconUI();

		//! 获取类名
		LPCTSTR GetClass() const;

		//! 获取类名对应的类指针
		LPVOID GetInterface(LPCTSTR pstrName);

		//! 绘制控件
		bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

		//! 设置属性
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		//! 设置控件要显示的图标
		void SetIcon(HICON hIcon);

		//! 设置获取的Icon尺寸，0:正常、<0:小图标、>0:大图标
		//! 写xml时，主要iconsize属性要放在file之前
		void SetIconSize(int iSize);

		//! 设置控件要显示的图标文件
		void SetFile(CDuiString strFile);

		//! 获取文件路径
		CDuiString GetFile();

	protected:
		HICON			m_hIcon;		//!< 图标句柄
		CDuiString		m_strFile;		//!< 图标文件
		int				m_iSize;		//!< 图标尺寸
	};

} // namespace DuiLib

#endif // __UIICON_H__