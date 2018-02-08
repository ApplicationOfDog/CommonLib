#include "stdafx.h"
#include "UIIcon.h"


namespace DuiLib
{
	CIconUI::CIconUI()
		: m_hIcon(NULL)
		, m_iSize(0)
	{
	}

	CIconUI::~CIconUI()
	{
		if (m_hIcon)
		{
			::DeleteObject(m_hIcon);
			m_hIcon = NULL;
		}
	}

	LPCTSTR CIconUI::GetClass() const
	{
		return DUI_CTR_ICON;
	}

	LPVOID CIconUI::GetInterface( LPCTSTR pstrName )
	{
		if (_tcscmp(pstrName, GetClass()) == 0)
			return static_cast<CIconUI*>(this);

		return __super::GetInterface(pstrName);
	}

	bool CIconUI::DoPaint( HDC hDC, const RECT& rcPaint, CControlUI* pStopControl )
	{
		__super::DoPaint(hDC, rcPaint, pStopControl);

		if (NULL == m_hIcon && !m_strFile.IsEmpty())
		{
			SHFILEINFO info;
			UINT uFlags = SHGFI_SYSICONINDEX | SHGFI_ICON;

			if (m_iSize > 0) uFlags |= SHGFI_LARGEICON;
			else if (m_iSize < 0) uFlags |= SHGFI_SMALLICON;

			if (SHGetFileInfo(m_strFile, 0, &info, sizeof(info), uFlags))
			{
				m_hIcon = info.hIcon;
			}
		}

		if (m_hIcon)
		{
			RECT rc = m_rcItem;
			::DrawIconEx(hDC, rc.left, rc.top, m_hIcon, rc.right-rc.left, rc.bottom-rc.top, 0, NULL, DI_NORMAL);
		}

		return true;
	}

	void CIconUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
	{
		if ( _tcsicmp(pstrName, _T("file")) == 0 ) SetFile(pstrValue);
		else if ( _tcsicmp(pstrName, _T("iconsize")) == 0 ) SetIconSize(_ttoi(pstrValue));
		else __super::SetAttribute(pstrName, pstrValue);
	}

	void CIconUI::SetIcon( HICON hIcon )
	{
		if (m_hIcon == hIcon) return;
		if (m_hIcon) ::DeleteObject(m_hIcon);
		m_hIcon = hIcon;
	}

	void CIconUI::SetIconSize( int iSize )
	{
		m_iSize = iSize;
	}

	void CIconUI::SetFile( CDuiString strFile )
	{
		// 同一文件且已获取过，则不再获取
		if (m_strFile == strFile && NULL != m_hIcon)
			return;

		m_strFile = strFile;
		if (m_hIcon)
		{
			::DeleteObject(m_hIcon);
			m_hIcon = NULL;
		}

		/*if (!strFile.IsEmpty())
		{
			SHFILEINFO info;
			UINT uFlags = SHGFI_SYSICONINDEX | SHGFI_ICON;

			if (m_iSize > 0) uFlags |= SHGFI_LARGEICON;
			else if (m_iSize < 0) uFlags |= SHGFI_SMALLICON;

			if (SHGetFileInfo(strFile, 0, &info, sizeof(info), uFlags))
			{
				m_hIcon = info.hIcon;
			}
		}*/
	}

	CDuiString CIconUI::GetFile()
	{
		return m_strFile;
	}

} // namespace DuiLib
