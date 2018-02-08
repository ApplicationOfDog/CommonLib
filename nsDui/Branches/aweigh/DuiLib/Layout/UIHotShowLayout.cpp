#include "stdafx.h"
#include "UIHotShowLayout.h"

namespace DuiLib
{
	#define TIMER_MOUSEENTER		11
	#define TIMER_MOUSELEAVE		12

	CHotShowLayoutUI::CHotShowLayoutUI()
		: m_uState(0)
		, m_nElapse(-1)
	{

	}

	LPCTSTR CHotShowLayoutUI::GetClass() const
	{
		return DUI_CTR_HOTSHOWLAYOUT;
	}

	LPVOID CHotShowLayoutUI::GetInterface( LPCTSTR pstrName )
	{
		if( _tcscmp(pstrName, GetClass()) == 0 )
			return static_cast<CHotShowLayoutUI*>(this);

		return __super::GetInterface(pstrName);
	}

	void CHotShowLayoutUI::DoInit()
	{
		__super::DoInit();
		SetHotShow(false);
	}

	void CHotShowLayoutUI::DoEvent( TEventUI& event )
	{
		__super::DoEvent(event);

		switch (event.Type)
		{
		case UIEVENT_MOUSEENTER:
			if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() && (m_uState & UISTATE_HOT) == 0 )
			{
				m_uState |= UISTATE_HOT;
				SetHotShow(true);
				if (m_pManager)
				{
					// 10ms内就当成立即触发
					if (m_nElapse > 10) {
						// 如果鼠标不小心移出去了，立马又移回来，则不需要再次触发通知
						if (m_pManager->KillTimer(this, TIMER_MOUSELEAVE) == false)
							m_pManager->SetTimer(this, TIMER_MOUSEENTER, m_nElapse);
					}
					else if (m_nElapse >= 0) {
						m_pManager->SendNotify(this, DUI_MSGTYPE_MOUSEENTER, event.wParam, event.lParam);
					}
				}
			}
			break;

		case UIEVENT_MOUSELEAVE:
			if( !::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() && (m_uState & UISTATE_HOT) != 0)
			{
				m_uState &= ~UISTATE_HOT;
				SetHotShow(false);
				if(m_pManager)
				{
					m_pManager->RemoveMouseLeaveNeeded(this);
					if (m_nElapse > 10) {
						// 如果鼠标是快速掠过，则不需要触发通知
						if (m_pManager->KillTimer(this, TIMER_MOUSEENTER) == false)
							m_pManager->SetTimer(this, TIMER_MOUSELEAVE, m_nElapse);
					}
					else if (m_nElapse >= 0) {
						m_pManager->SendNotify(this, DUI_MSGTYPE_MOUSELEAVE, event.wParam, event.lParam);
					}
				}
			}
			else
			{
				if(m_pManager) m_pManager->AddMouseLeaveNeeded(this);
			}
			break;

		case UIEVENT_TIMER:
			if (TIMER_MOUSEENTER == event.wParam)
			{
				if (m_pManager)
				{
					m_pManager->KillTimer(this, event.wParam);
					m_pManager->SendNotify(this, DUI_MSGTYPE_MOUSEENTER, event.wParam, event.lParam);
				}
			}
			else if (TIMER_MOUSELEAVE == event.wParam)
			{
				if (m_pManager)
				{
					m_pManager->KillTimer(this, event.wParam);
					m_pManager->SendNotify(this, DUI_MSGTYPE_MOUSELEAVE, event.wParam, event.lParam);
				}
			}
			break;

		default: break;
		}
	}

	void CHotShowLayoutUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
	{
		if (_tcscmp(pstrName, _T("elapse")) == 0) SetElapse(_ttoi(pstrValue));
		else __super::SetAttribute(pstrName, pstrValue);
	}

	int CHotShowLayoutUI::GetElapse()
	{
		return m_nElapse;
	}

	void CHotShowLayoutUI::SetElapse( int nElapse )
	{
		m_nElapse = nElapse;
	}

	int CHotShowLayoutUI::SetHotShow( bool bHot )
	{
		int count = 0;
		for (int i = 0; i < GetCount(); i++)
		{
			CControlUI* pControl = GetItemAt(i);
			LPCTSTR pValue = pControl ? pControl->GetCustomAttribute(_T("hotshow")) : NULL;
			if (pValue)
			{
				bool bVisible = _tcscmp(pValue, _T("true")) == 0 ? bHot : !bHot;
				pControl->SetVisible(bVisible);
				count++;
			}
		}
		return count;
	}

} // namespace DuiLib
