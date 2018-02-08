#ifndef GifAnimUI_h__
#define GifAnimUI_h__

#pragma once

#ifndef _USE_GDIPLUS
#	define _USE_GDIPLUS 1
#endif

#ifdef _USE_GDIPLUS
#include <GdiPlus.h>
#pragma comment( lib, "GdiPlus.lib" )
using namespace Gdiplus;
class DUILIB_API Gdiplus::RectF;
struct DUILIB_API Gdiplus::GdiplusStartupInput;
#endif

namespace DuiLib
{
	class CControl;

	#define EVENT_TIEM_ID	100

	class DUILIB_API CGifAnimUI : public CControlUI
	{
	public:
		CGifAnimUI(void);
		~CGifAnimUI(void);

		LPCTSTR	GetClass() const;
		LPVOID	GetInterface(LPCTSTR pstrName);
		void	DoInit() override;
		bool	DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
		void	DoEvent(TEventUI& event);
		void	SetVisible(bool bVisible = true );
		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void	SetBkImage(LPCTSTR pStrImage);
		LPCTSTR GetBkImage();

		void	SetAutoPlay(bool bIsAuto = true );
		bool	IsAutoPlay() const;
		void	SetAutoSize(bool bIsAuto = true );
		bool	IsAutoSize() const;
		void	PlayGif();
		void	PauseGif();
		void	StopGif();

	private:
		void	InitGifImage();
		void	DeleteGif();
		void    OnTimer( UINT_PTR idEvent );
		///add by aweigh 20170329,����ؼ���֧��exe��Դ����gif������
		///<summary>������Դ·��,����:"res='2011' restype='gif'" => sImageName="2011",sImageResType="gif"</summary>
		///<param name="pstrImage">·������,����:"res='2011' restype='gif'"</param>
		///<param name="sImageName">��Դ����,����:"2011"</param>
		///<param name="sImageResType">��Դ����,����:"gif"</param>
		///<returns></returns>
		void	ParseFileNameBy(IN LPCTSTR pstrImage,OUT CDuiString& sImageName,OUT CDuiString& sImageResType);
		void	DrawFrame( HDC hDC );		// ����GIFÿ֡
		Gdiplus::Image*	LoadGifFromFile(LPCTSTR pstrGifPath);
		Gdiplus::Image* LoadGifFromMemory( LPVOID pBuf,size_t dwSize );

	private:
		Gdiplus::Image	*m_pGifImage;
		UINT			m_nFrameCount;				// gifͼƬ��֡��
		UINT			m_nFramePosition;			// ��ǰ�ŵ��ڼ�֡
		Gdiplus::PropertyItem*	m_pPropertyItem;	// ֡��֮֡����ʱ��

		CDuiString		m_sBkImage;
		bool			m_bIsAutoPlay;				// �Ƿ��Զ�����gif
		bool			m_bIsAutoSize;				// �Ƿ��Զ�����ͼƬ���ô�С
		bool			m_bIsPlaying;
		
		///<remark by="Aweigh" date="2017/3/29">
		/// bug:������ֻ��CGifAnimUI�ؼ���Ϊû�г�ʼ��GDI+���¼���gifͼƬʧ��
		///</remark>
#ifdef _USE_GDIPLUS
		ULONG_PTR				m_gdiplusToken;
		GdiplusStartupInput		m_gdiplusStartupInput;
#endif
	};
}

#endif // GifAnimUI_h__
