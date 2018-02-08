#ifndef __UIICON_H__
#define __UIICON_H__

#pragma once

namespace DuiLib
{
	/**
	* �Զ����ͼ����ʾ�ؼ���������ʾico����
	* @brief Iconͼ��ؼ�
	* @author sundh
	*/
	class DUILIB_API CIconUI : public CControlUI
	{
	public:
		CIconUI();
		~CIconUI();

		//! ��ȡ����
		LPCTSTR GetClass() const;

		//! ��ȡ������Ӧ����ָ��
		LPVOID GetInterface(LPCTSTR pstrName);

		//! ���ƿؼ�
		bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

		//! ��������
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		//! ���ÿؼ�Ҫ��ʾ��ͼ��
		void SetIcon(HICON hIcon);

		//! ���û�ȡ��Icon�ߴ磬0:������<0:Сͼ�ꡢ>0:��ͼ��
		//! дxmlʱ����Ҫiconsize����Ҫ����file֮ǰ
		void SetIconSize(int iSize);

		//! ���ÿؼ�Ҫ��ʾ��ͼ���ļ�
		void SetFile(CDuiString strFile);

		//! ��ȡ�ļ�·��
		CDuiString GetFile();

	protected:
		HICON			m_hIcon;		//!< ͼ����
		CDuiString		m_strFile;		//!< ͼ���ļ�
		int				m_iSize;		//!< ͼ��ߴ�
	};

} // namespace DuiLib

#endif // __UIICON_H__