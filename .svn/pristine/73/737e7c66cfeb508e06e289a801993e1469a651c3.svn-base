#include "StdAfx.h"
#include "AmHttpSocket.h"
#include <atlbase.h>
#include <limits.h>

#define AgentName _T("Nimo Software HTTP Retriever 1.0")

//case insensitive search functions...
#ifdef UNICODE
#define _tcsustr wcsustr
#else
#define _tcsustr strustr
#endif

char* strustr(char *source, char *s)
{
	//make an uppercase copy af source and s
	char *csource = strdup(source);
	char *cs = strdup(s);
	strupr(csource);
	strupr(cs);
	//find cs in csource...
	char *result = strstr(csource, cs);
	if (result != NULL)
	{
		//cs is somewhere in csource
		int pos = result - csource;
		result = source;
		result += pos;
	}
	//clean up
	free(csource);
	free(cs);
	return result;
}

wchar_t* wcsustr(wchar_t *source, wchar_t *s)
{
	//make an uppercase copy af source and s
	wchar_t *csource = wcsdup(source);
	wchar_t *cs = wcsdup(s);
	wcsupr(csource);
	wcsupr(cs);
	//find cs in csource...
	wchar_t *result = wcsstr(csource, cs);
	if (result != NULL)
	{
		//cs is somewhere in csource
		int pos = result - csource;
		result = source;
		result += pos;
	}
	//clean up
	free(csource);
	free(cs);
	return result;
}

// 注释：多字节包括GBK和UTF-8
int GBK2UTF8(char *szGbk, char *szUtf8, int Len)
{
	// 先将多字节GBK（CP_ACP或ANSI）转换成宽字符UTF-16
	// 得到转换后，所需要的内存字符数
	int n = MultiByteToWideChar(CP_ACP, 0, szGbk, -1, NULL, 0);
	// 字符数乘以 sizeof(WCHAR) 得到字节数
	WCHAR *str1 = new WCHAR[sizeof(WCHAR) * n];
	// 转换
	MultiByteToWideChar(CP_ACP,  // MultiByte的代码页Code Page
		0,            //附加标志，与音标有关
		szGbk,        // 输入的GBK字符串
		-1,           // 输入字符串长度，-1表示由函数内部计算
		str1,         // 输出
		n             // 输出所需分配的内存
		);

	// 再将宽字符（UTF-16）转换多字节（UTF-8）
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	if (n > Len)
	{
		delete[]str1;
		return -1;
	}
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, szUtf8, n, NULL, NULL);
	delete[]str1;
	str1 = NULL;

	return 0;
}

//UTF-8 GBK
int UTF82GBK(char *szUtf8, char *szGbk, int Len)
{
	int n = MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, NULL, 0);
	WCHAR * wszGBK = new WCHAR[sizeof(WCHAR) * n];
	memset(wszGBK, 0, sizeof(WCHAR) * n);
	MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, wszGBK, n);

	n = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	if (n > Len)
	{
		delete[]wszGBK;
		return -1;
	}
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGbk, n, NULL, NULL);
	delete[]wszGBK;
	wszGBK = NULL;
	return 0;
}

CAmHttpSocket::CAmHttpSocket()
{
	LastError = 0;
	ReceivedData = NULL;
	Headers = NULL;
	hIO = NULL;
	hIS = NULL;
	hCO = NULL;
	hIO = InternetOpen(AgentName, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
}

CAmHttpSocket::~CAmHttpSocket()
{
	if (ReceivedData != NULL) free(ReceivedData);
	if (Headers != NULL) free(Headers);
	if (hIO != NULL) InternetCloseHandle(hIO);
	if (hIS != NULL) InternetCloseHandle(hIS);
	if (hCO != NULL) InternetCloseHandle(hCO);
}

bool CAmHttpSocket::OpenUrl(const TCHAR *url)
{
	if (hIS != NULL) InternetCloseHandle(hIS);
	hIS = InternetOpenUrl(hIO, url, NULL, 0, HTTP_QUERY_DATE, 0);
	if (hIS != NULL) return true;
	else
	{
		LastError = GetLastError();
		return false;
	}
}

bool CAmHttpSocket::PostUrl(const TCHAR *url, const char *PostData, int PostDataLength)
{
	//check length of postdata
	if (PostDataLength == -1)
		PostDataLength = strlen(PostData);
	//some variable that we need...
	URL_COMPONENTS uc;
	//let's split the url...
	uc.dwStructSize = sizeof(uc);
	uc.lpszScheme = NULL;
	uc.dwSchemeLength = 0;
	uc.lpszHostName = NULL;
	uc.dwHostNameLength = 1;
	uc.nPort = 0;
	uc.lpszUserName = NULL;
	uc.dwUserNameLength = 0;
	uc.lpszPassword = NULL;
	uc.dwPasswordLength = 0;
	uc.lpszUrlPath = NULL;
	uc.dwUrlPathLength = 1;
	uc.lpszExtraInfo = NULL;
	uc.dwExtraInfoLength = 0;
	InternetCrackUrl(url, _tcslen(url), 0, &uc);
	//post the data...
	if (hCO != NULL) InternetCloseHandle(hCO);
	TCHAR *HostName = _tcsdup(uc.lpszHostName);
	HostName[uc.dwHostNameLength] = '\0';
	TCHAR *FileName = _tcsdup(uc.lpszUrlPath);
	FileName[uc.dwUrlPathLength] = '\0';
	if (hIS != NULL) InternetCloseHandle(hIS); //if open, close the handle to the connection
	DWORD flags;
	if (uc.nPort == 80)
	{
		//we are talking plain http
		flags = INTERNET_FLAG_NO_CACHE_WRITE;
	}
	else
	{
		//we are talking secure https
		flags = INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_SECURE |
			INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
	}
	TCHAR headers[] = _T("Content-Type: application/x-www-form-urlencoded"); //content type for post...
	TCHAR szAccept[] = _T("*/*"); //we accept everything...
	LPTSTR AcceptTypes[2] = { 0 };
	AcceptTypes[0] = szAccept;
	hCO = InternetConnect(hIO, HostName, uc.nPort, _T(""), _T(""), INTERNET_SERVICE_HTTP, INTERNET_FLAG_NO_CACHE_WRITE, 0);
	hIS = HttpOpenRequest(hCO, _T("POST"), FileName, NULL, NULL, (LPCTSTR*)AcceptTypes, flags, 0);
	if (!HttpSendRequest(hIS, headers, _tcslen(headers), (TCHAR*)PostData, PostDataLength))
	{
		LastError = GetLastError();
		free(HostName);
		free(FileName);
		return false;
	}
	free(HostName);
	free(FileName);
	return true;
}

TCHAR* CAmHttpSocket::GetHeaders(const TCHAR *url)
{
	//did we get an url?
	if (url == NULL)
	{
		LastError = -1;
		return NULL;
	}
	//open the url...
	OpenUrl(url);
	//delete old headers...
	if (Headers != NULL) free(Headers);
	Headers = (TCHAR*)calloc(1, sizeof(TCHAR));
	//get the size headers
	DWORD d = 1, d2 = 0;
	int i = HttpQueryInfo(hIS, HTTP_QUERY_RAW_HEADERS, Headers, &d, &d2);
	//alloc some space for the headers
	Headers = (TCHAR*)realloc(Headers, d * sizeof(TCHAR));
	if (!HttpQueryInfo(hIS, HTTP_QUERY_RAW_HEADERS, Headers, &d, &d2)) return NULL;
	return Headers;
}

char* CAmHttpSocket::GetPage(const TCHAR *url, bool Post, const char *PostData, int PostDataLength)
{
	//did we get an url?
	if (url == NULL)
	{
		LastError = -1;
		return NULL;
	}
	//get the page and store it in ReceivedData...
	if (Post)
	{
		//use http post...
		if (!PostUrl(url, PostData, PostDataLength)) return NULL;
	}
	else
	{
		//use http get
		if (!OpenUrl(url)) return NULL;
	}
	const int rdsize = 8192;
	char mr[rdsize];
	DWORD rd;
	int curpos = 0;
	if (ReceivedData != NULL) free(ReceivedData);
	ReceivedData = (char*)calloc(rdsize + 1, sizeof(char));
	while (InternetReadFile(hIS, mr, rdsize - 1, &rd))
	{
		if (rd == 0) break;
		mr[rd] = '\0';
		curpos += rd;
		ReceivedData[curpos] = '\0';
		strcat(ReceivedData, mr);
		ReceivedData = (char*)realloc(ReceivedData, curpos + rdsize);
	}
	return ReceivedData;
}

TCHAR* CAmHttpSocket::GetHeaderLine(TCHAR *s)
{
	//find a line in the headers that contains s, and return a pointer to the line...
	if (Headers == NULL) return NULL;
	TCHAR *ts = Headers;
	if (_tcsustr(ts, s) != NULL) return ts;
	while (1)
	{
		if (*ts == '\0' && ts[1] == '\0') break;
		if (*ts == '\0')
		{
			ts++;
			if (_tcsustr(ts, s) != NULL) return ts;
		}
		else ts++;
	}
	return NULL;
}

int CAmHttpSocket::GetPageStatusCode()
{
	//get the correct header line
	TCHAR *s = GetHeaderLine(_T("http"));
	if (s == NULL) return 0; //no headers
							 //find the 3 digit code...
	if (_tcslen(s) < 3) return 0; //some error,  the string is too short...
	while (!(isdigit(s[0]) && isdigit(s[1]) && isdigit(s[2])))
	{
		if (s[3] == '\0') return 0; //we have reached the end of the string, without finding the number...
		s++;
	}
	//make a copy of s, and return the code
	TCHAR *code = _tcsdup(s);
	code[3] = '\0'; //remove all text after the 3 digit response code
	int result = _ttoi(code);
	free(code);
	return result;
}