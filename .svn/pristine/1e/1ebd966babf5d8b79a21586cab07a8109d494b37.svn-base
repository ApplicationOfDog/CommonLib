#ifndef __DLG_MAIN_H__
#define __DLG_MAIN_H__
#include "plugin-common.h"

extern extra_parameters* g_pluginParms;

class CDlgMain : public WindowImplBase
{
private:
	enum TIMER_ID
	{
		TIMER_URL = 1,
		TIMER_MSG,
		TIMER_UNIN,
		TIMER_OTHER,
	};
	CDuiString m_URL;
	int m_ms;
	bool m_bGetSize;
public:
	CDlgMain();
	~CDlgMain();
	void SetWindowClassName(TCHAR * szName) { m_winClsName = szName; };
	void SetXmlName(TCHAR * szName) { m_xmlName = szName; };
	void NaviUrl(CDuiString szUrl, int timeOutSecs);
	LPCTSTR GetWindowClassName() const { return m_winClsName; };
	CPaintManagerUI* GetPM() { return &m_PaintManager; };
	void SaveToControlCallbackMap(CDuiString ctlName, int callback)
	{
		m_controlCallbackMap[ctlName] = callback;
	}
	std::map<CDuiString, int> GetCBMap()
	{
		return m_controlCallbackMap;
	}
	void OnClick(TNotifyUI& msg);
protected:
	CDuiString GetSkinFolder() { return (TCHAR*)0; }
	CDuiString GetSkinFile() { return m_xmlName; }
	CDuiString GetZIPFileName()const { return _T("skin.zip"); }
	UILIB_RESOURCETYPE GetResourceType() const { return UILIB_ZIP; }

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	virtual void Notify(TNotifyUI& msg);
	std::map<CDuiString, int> m_controlCallbackMap;
private:
	CDuiString m_winClsName;
	CDuiString m_xmlName;
	CWndShadow m_WndShadow;
};
/*DWORD64 GetFolderSize(LPCTSTR szPath, DWORD *dwFiles = NULL, DWORD *dwFolders = NULL);*/
#endif