#pragma once

class CMainDlg : public WindowImplBase
{
public:
	CMainDlg();
	~CMainDlg();

	LPCTSTR		GetWindowClassName() const   {   return _T("SeeFileCert");  }
	CDuiString	GetSkinFile()                {   return _T("main.xml");  }
	CDuiString	GetSkinFolder()              {   return _T("");  }

	void InitWindow();

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL ShowPath(LPCTSTR lpPath);

	DUI_DECLARE_MESSAGE_MAP()

protected:
	void OnDropFiles(HDROP hDrop);

	void OnDClickItem(TNotifyUI& msg);

	BOOL AddFile(CListUI* pCtrlList, WIN32_FIND_DATA& fileData);

private:
	CDuiString		m_strPath;
	int				m_nDirCount;
};
