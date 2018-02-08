#include "stdafx.h"
#include "UIAniButton.h"

namespace DuiLib
{
	#define TIMER_HOTANIMATION		12

	CAniButtonUI::CAniButtonUI()
		: m_nElapse(30)
		, m_nFrames(1)
		, m_nCurrentFrame(0)
		, m_bAlwaysPlay(false)
	{

	}

	LPCTSTR CAniButtonUI::GetClass() const
	{
		return DUI_CTR_ANIBUTTON;
	}

	LPVOID CAniButtonUI::GetInterface( LPCTSTR pstrName )
	{
		if( _tcscmp(pstrName, GetClass()) == 0 )
			return static_cast<CAniButtonUI*>(this);
		return __super::GetInterface(pstrName);
	}

	void CAniButtonUI::DoInit()
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
	}

	void CAniButtonUI::DoEvent( TEventUI& event )
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
			if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() )
			{
				if((m_uButtonState & UISTATE_HOT) == 0) m_uButtonState |= UISTATE_HOT;
				if(m_pManager) m_pManager->SetTimer(this, TIMER_HOTANIMATION, m_nElapse);
			}
			break;

		case UIEVENT_MOUSELEAVE:
			if( !::PtInRect(&m_rcItem, event.ptMouse) )
			{
				if((m_uButtonState & UISTATE_HOT) != 0) m_uButtonState &= ~UISTATE_HOT;
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
				if ((m_uButtonState & UISTATE_HOT) != 0) {
					if (m_nCurrentFrame < m_nFrames - 1) m_nCurrentFrame++;
					else if (m_bAlwaysPlay) m_nCurrentFrame = 0;
					else if (m_pManager) m_pManager->KillTimer(this, event.wParam);
				}
				else {
					if (m_nCurrentFrame > 0) m_nCurrentFrame--;
					else if (m_pManager) m_pManager->KillTimer(this, event.wParam);
				}
				Invalidate();
			}
			break;

		default:
			__super::DoEvent(event);
			break;
		}
	}

	void CAniButtonUI::PaintBkImage( HDC hDC )
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

	void CAniButtonUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
	{
		if (_tcscmp(pstrName, _T("elapse")) == 0) SetElapse(_ttoi(pstrValue));
		else if (_tcscmp(pstrName, _T("frames")) == 0) SetFrames(_ttoi(pstrValue));
		else if (_tcscmp(pstrName, _T("alwaysplay")) == 0) SetAlwaysPlay(_tcscmp(pstrValue, _T("true")) == 0);
		else __super::SetAttribute(pstrName, pstrValue);
	}

	UINT CAniButtonUI::GetElapse()
	{
		return m_nElapse;
	}

	void CAniButtonUI::SetElapse( UINT nElapse )
	{
		if (nElapse > 10) m_nElapse = nElapse;
		else m_nElapse = 10;
	}

	UINT CAniButtonUI::GetFrames()
	{
		return m_nFrames;
	}

	void CAniButtonUI::SetFrames( UINT nFrames )
	{
		if (nFrames > 0)
		{
			m_nFrames = nFrames;
			m_nCurrentFrame = 0;
		}
	}

	bool CAniButtonUI::IsAlwaysPlay()
	{
		return m_bAlwaysPlay;
	}

	void CAniButtonUI::SetAlwaysPlay( bool bAlways )
	{
		m_bAlwaysPlay = bAlways;
	}

} // namespace DuiLib
