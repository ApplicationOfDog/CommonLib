#include "stdafx.h"
#include "MainDlg.h"
#include "SignedFileInfo.h"

#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")

#include <Windows.h>
#pragma comment(lib,"version.lib")

#define UIMSG_RESHOW_PATH		(WM_USER + 100)

CDuiString GetSizeString(DWORD dwSize)
{
	CDuiString strSize;
	if (dwSize > (1024*1024*1024))
	{
		strSize.Format(_T("%u MB"), dwSize / (1024*1024));
	}
	else if (dwSize > 1024)
	{
		strSize.Format(_T("%u KB"), dwSize / 1024);
	}
	else
	{
		strSize = _T("1 KB");
	}
	return strSize;
}

CDuiString GetFileVersion(LPCTSTR lpFilePath)
{
	CDuiString strVersion;
	VS_FIXEDFILEINFO *pVerInfo = NULL;
	DWORD dwTemp, dwSize;
	BYTE *pData = NULL;
	UINT uLen;

	dwSize = GetFileVersionInfoSize(lpFilePath, &dwTemp);
	if (dwSize == 0)
	{
		return strVersion;
	}

	pData = new BYTE[dwSize+1];
	if (pData == NULL)
	{
		return strVersion;
	}

	if (!GetFileVersionInfo(lpFilePath, 0, dwSize, pData))
	{
		delete[] pData;
		return strVersion;
	}

	if (!VerQueryValue(pData, TEXT("\\"), (void **)&pVerInfo, &uLen)) 
	{
		delete[] pData;
		return strVersion;
	}

	DWORD verMS = pVerInfo->dwFileVersionMS;
	DWORD verLS = pVerInfo->dwFileVersionLS;
	DWORD major = HIWORD(verMS);
	DWORD minor = LOWORD(verMS);
	DWORD build = HIWORD(verLS);
	DWORD revision = LOWORD(verLS);
	delete[] pData;

	strVersion.Format(TEXT("%d.%d.%d.%d"), major, minor, build, revision);
	return strVersion;
}

BOOL HasParentDirectory(LPCTSTR lpPath)
{
	return _tcslen(lpPath) > 3;
}


DUI_BEGIN_MESSAGE_MAP(CMainDlg, WindowImplBase)
	DUI_ON_MSGTYPE_CTRNAME(DUI_MSGTYPE_ITEMACTIVATE, _T("list_item"), OnDClickItem)
DUI_END_MESSAGE_MAP()

CMainDlg::CMainDlg()
	: m_nDirCount(0)
{

}

CMainDlg::~CMainDlg()
{

}

typedef BOOL (WINAPI *LPFUNC)(HWND,UINT,DWORD,PCHANGEFILTERSTRUCT);
void CMainDlg::InitWindow()
{
	// WIN8管理员权限不让拖拽文件，这里要设置下允许接收拖拽消息
	HMODULE hLib = ::LoadLibraryA("user32.dll");
	if (hLib)
	{
		LPFUNC func = (LPFUNC)GetProcAddress(hLib, "ChangeWindowMessageFilterEx");
		if (func)
		{
			func(m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
			func(m_hWnd, WM_COPYDATA, MSGFLT_ALLOW, NULL);
			func(m_hWnd, 0x0049, MSGFLT_ALLOW, NULL); // 0x0049 - WM_COPYGLOBALDATA
		}
	}
}

LRESULT CMainDlg::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (uMsg)
	{
	case WM_DROPFILES:
		OnDropFiles((HDROP)wParam);
		break;

	case UIMSG_RESHOW_PATH:
		ShowPath(m_strPath);
		return 0;

	default: break;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

BOOL CMainDlg::ShowPath( LPCTSTR lpPath )
{
	if (!lpPath || !::PathIsDirectory(lpPath))
	{
		return FALSE;
	}

	CControlUI* pCtrlPath = FindControl(_T("edit_path"));
	if (pCtrlPath) pCtrlPath->SetText(lpPath);

	CListUI* pCtrlList = FindControl<CListUI>(_T("list_files"));
	if (!pCtrlList) return FALSE;

	pCtrlList->GetList()->SetDelayedDestroy(false);
	pCtrlList->RemoveAll();
	m_strPath = lpPath;
	m_nDirCount = 0;

	if (HasParentDirectory(lpPath))
	{
		m_nDirCount++;
		CDialogBuilder builder;
		CControlUI* pItem = builder.Create(_T("list_item.xml"), NULL, this);
		if (pItem)
		{
			CIconUI* pCtrlIcon = FindControl<CIconUI>(_T("icon_file"), pItem);
			if (pCtrlIcon) pCtrlIcon->SetFile(lpPath);

			CLabelUI* pCtrlLabel = FindControl<CLabelUI>(_T("label_filename"), pItem);
			if (pCtrlLabel) pCtrlLabel->SetText(_T(".."));

			pCtrlList->Add(pItem);
		}
	}

	TCHAR szWildcards[MAX_PATH] = {0};
	::PathCombine(szWildcards, lpPath, _T("*.*"));

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = ::FindFirstFile(szWildcards, &findFileData);

	BOOL bRet = INVALID_HANDLE_VALUE != hFind;
	while (bRet)
	{
		if (findFileData.cFileName[0] != _T('.'))
		{
			AddFile(pCtrlList, findFileData);
		}
		bRet = ::FindNextFile(hFind, &findFileData);
	}

	if (hFind) ::FindClose(hFind);

	return TRUE;
}

void CMainDlg::OnDropFiles( HDROP hDrop )
{
	UINT uNumFiles = ::DragQueryFile(hDrop, -1, NULL, 0);
	for (UINT i = 0; i < uNumFiles; i++)
	{
		TCHAR szPath[MAX_PATH] = {0};
		::DragQueryFile(hDrop, i, szPath, MAX_PATH);
		if (ShowPath(szPath)) break;
	}

	::DragFinish(hDrop);
}

void CMainDlg::OnDClickItem( TNotifyUI& msg )
{
	CControlUI* pCtrlLabel = FindControl(_T("label_filename"), msg.pSender);
	if (!pCtrlLabel) return;

	TCHAR szPath[MAX_PATH] = {0};
	CDuiString strText = pCtrlLabel->GetText();
	if (strText == _T(".."))
	{
		if (!HasParentDirectory(m_strPath)) return;
		_tcscpy_s(szPath, m_strPath.GetData());
		TCHAR* p = _tcsrchr(szPath, _T('\\'));
		if (p) *p = 0;
	}
	else
	{
		::PathCombine(szPath, m_strPath, pCtrlLabel->GetText());
		if (!::PathIsDirectory(szPath)) return;
	}

	//ShowPath(szPath);
	m_strPath = szPath;
	::PostMessage(GetHWND(), UIMSG_RESHOW_PATH, 0, 0);
}

BOOL CMainDlg::AddFile( CListUI* pCtrlList, WIN32_FIND_DATA& fileData )
{
	CDialogBuilder builder;
	CControlUI* pItem = builder.Create(_T("list_item.xml"), NULL, this);
	if (!pItem) return FALSE;

	BOOL bIsDir = (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
	if (bIsDir)
	{
		pCtrlList->AddAt(pItem, m_nDirCount);
		m_nDirCount++;
	}
	else
	{
		pCtrlList->Add(pItem);
	}

	TCHAR szPath[MAX_PATH] = {0};
	::PathCombine(szPath, m_strPath.GetData(), fileData.cFileName);

	CIconUI* pCtrlIcon = FindControl<CIconUI>(_T("icon_file"), pItem);
	if (pCtrlIcon) pCtrlIcon->SetFile(szPath);

	CLabelUI* pCtrlLabel = FindControl<CLabelUI>(_T("label_filename"), pItem);
	if (pCtrlLabel) pCtrlLabel->SetText(fileData.cFileName);

	if (!bIsDir)
	{
		pCtrlLabel = FindControl<CLabelUI>(_T("label_filesize"), pItem);
		if (pCtrlLabel) pCtrlLabel->SetText(GetSizeString(fileData.nFileSizeLow));
	}

	pCtrlLabel = FindControl<CLabelUI>(_T("label_lasttime"), pItem);
	if (pCtrlLabel)
	{
		FILETIME fileTime = {0};
		SYSTEMTIME sysTime = {0};
		::FileTimeToLocalFileTime(&fileData.ftLastWriteTime, &fileTime);
		::FileTimeToSystemTime(&fileTime, &sysTime);
		CDuiString strTime;
		strTime.Format(_T("%04u/%02u/%02u %02u:%02u"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute);
		pCtrlLabel->SetText(strTime);
	}

	pCtrlLabel = FindControl<CLabelUI>(_T("label_version"), pItem);
	if (pCtrlLabel)
	{
		CDuiString strVersion = GetFileVersion(szPath);
		pCtrlLabel->SetText(strVersion);
	}

	pCtrlLabel = FindControl<CLabelUI>(_T("label_cert"), pItem);
	if (pCtrlLabel)
	{
		SignerCertificate cert;
		GetSignatureInfo(szPath, cert);
		pCtrlLabel->SetText(cert.SubjectName.c_str());
	}

	return TRUE;
}
