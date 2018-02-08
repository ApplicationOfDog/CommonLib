#include "stdafx.h"
#include "UIAniPicture.h"

namespace DuiLib
{
#define TIMER_HOTANIMATION		12
#define TIMER_AUTOPLAY			13

	CAniPictureUI::CAniPictureUI()
		: m_nElapse(30)
		, m_nFrames(1)
		, m_nCurrentFrame(0)
		, m_bAlwaysPlay(false)
		, m_bMouseEnter(false)
		, m_bAutoPlay(false)
		, m_bRollback(true)
	{
		
	}

	LPCTSTR CAniPictureUI::GetClass() const
	{
		return DUI_CTR_ANIPICTURE;
	}

	LPVOID CAniPictureUI::GetInterface( LPCTSTR pstrName )
	{
		if( _tcscmp(pstrName, GetClass()) == 0 )
			return static_cast<CAniPictureUI*>(this);
		return __super::GetInterface(pstrName);
	}

	void CAniPictureUI::DoInit()
	{
		__super::DoInit();
		DrawImage(NULL, m_diBk);
		if (m_diBk.pImageInfo && m_nFrames > 0)
		{
			m_diBk.rcBmpPart.left   = 0;
			m_diBk.rcBmpPart.right  = m_diBk.pImageInfo->nX / m_nFrames;
			m_diBk.rcBmpPart.top    = 0;
			m_diBk.rcBmpPart.bottom = m_diBk.pImageInfo->nY;
		}

		if (m_bAutoPlay)
		{
			if(m_pManager) m_pManager->SetTimer(this, TIMER_AUTOPLAY, m_nElapse);
		}
	}

	void CAniPictureUI::DoEvent( TEventUI& event )
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND )
		{
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else __super::DoEvent(event);
			return;
		}

		switch (event.Type)
		{
		case UIEVENT_MOUSEENTER:
			if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() && (!m_bAutoPlay))
			{
				m_bMouseEnter = true;
				if(m_pManager) m_pManager->SetTimer(this, TIMER_HOTANIMATION, m_nElapse);
			}
			break;

		case UIEVENT_MOUSELEAVE:
			if( !::PtInRect(&m_rcItem, event.ptMouse)  && (!m_bAutoPlay) && m_bRollback)
			{
				m_bMouseEnter = false;
				if(m_pManager) {
					m_pManager->RemoveMouseLeaveNeeded(this);
					m_pManager->SetTimer(this, TIMER_HOTANIMATION, m_nElapse);
				}
			}
			else
			{
				if(m_pManager) m_pManager->AddMouseLeaveNeeded(this);
			}
			break;

		case UIEVENT_TIMER:
			if (TIMER_HOTANIMATION == event.wParam)
			{
				if (!m_bRollback)
				{
					if (m_nCurrentFrame < m_nFrames - 1) m_nCurrentFrame++;
					else if (m_bAlwaysPlay) m_nCurrentFrame = 0;
					else if (m_pManager) 
					{
						m_pManager->KillTimer(this, event.wParam);
						m_nCurrentFrame = 0;
					}
				}
				else
				{
					if (m_bMouseEnter) {
						if (m_nCurrentFrame < m_nFrames - 1) m_nCurrentFrame++;
						else if (m_bAlwaysPlay) m_nCurrentFrame = 0;
						else if (m_pManager) m_pManager->KillTimer(this, event.wParam);
					}
					else {
						if (m_nCurrentFrame > 0) m_nCurrentFrame--;
						else if (m_pManager) m_pManager->KillTimer(this, event.wParam);
					}
				}
				
				Invalidate();
			}
			else if (TIMER_AUTOPLAY == event.wParam)
			{
				if (m_nCurrentFrame < m_nFrames - 1) m_nCurrentFrame++;
				else if (m_bAlwaysPlay) m_nCurrentFrame = 0;
				else if (m_pManager) m_pManager->KillTimer(this, event.wParam);
				Invalidate();
			}
			break;

		default:
			__super::DoEvent(event);
			break;
		}
	}

	void CAniPictureUI::PaintBkImage( HDC hDC )
	{
		DrawImage(NULL, m_diBk); // ÏÈ»ñÈ¡Í¼Æ¬³ß´ç
		if (m_diBk.pImageInfo && m_nFrames > 0)
		{
			if (m_nCurrentFrame >= m_nFrames) m_nCurrentFrame = m_nFrames - 1;

			int iWidth = m_diBk.pImageInfo->nX / m_nFrames;
			m_diBk.rcBmpPart.left   = m_nCurrentFrame * iWidth;
			m_diBk.rcBmpPart.right  = m_diBk.rcBmpPart.left + iWidth;
			m_diBk.rcBmpPart.top    = 0;
			m_diBk.rcBmpPart.bottom = m_diBk.pImageInfo->nY;
		}
		DrawImage(hDC, m_diBk);
	}

	void CAniPictureUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
	{
		if (_tcscmp(pstrName, _T("elapse")) == 0) SetElapse(_ttoi(pstrValue));
		else if (_tcscmp(pstrName, _T("frames")) == 0) SetFrames(_ttoi(pstrValue));
		else if (_tcscmp(pstrName, _T("alwaysplay")) == 0) SetAlwaysPlay(_tcscmp(pstrValue, _T("true")) == 0);
		else if (_tcscmp(pstrName, _T("autoplay")) == 0) SetAutoPlay(_tcscmp(pstrValue, _T("true")) == 0);
		else if (_tcscmp(pstrName, _T("rollback")) == 0) SetRollback(_tcscmp(pstrValue, _T("true")) == 0);
		else __super::SetAttribute(pstrName, pstrValue);
	}

	UINT CAniPictureUI::GetElapse()
	{
		return m_nElapse;
	}

	void CAniPictureUI::SetElapse( UINT nElapse )
	{
		if (nElapse > 10) m_nElapse = nElapse;
		else m_nElapse = 10;
	}

	UINT CAniPictureUI::GetFrames()
	{
		return m_nFrames;
	}

	void CAniPictureUI::SetFrames( UINT nFrames )
	{
		if (nFrames > 0)
		{
			m_nFrames = nFrames;
			m_nCurrentFrame = 0;
		}
	}

	bool CAniPictureUI::IsAlwaysPlay()
	{
		return m_bAlwaysPlay;
	}

	void CAniPictureUI::SetAlwaysPlay( bool bAlways )
	{
		m_bAlwaysPlay = bAlways;
	}

	bool CAniPictureUI::IsAutoPlay()
	{
		return m_bAutoPlay;
	}

	void CAniPictureUI::SetAutoPlay( bool bAutoPlay )
	{
		m_bAutoPlay = bAutoPlay;
	}

	bool CAniPictureUI::IsRollback()
	{
		return m_bRollback;
	}

	void CAniPictureUI::SetRollback(bool bRollback)
	{
		m_bRollback = bRollback;
	}

} // namespace DuiLib
