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
		///add by aweigh 20170329,解决控件不支持exe资源加载gif的问题
		///<summary>分析资源路径,例如:"res='2011' restype='gif'" => sImageName="2011",sImageResType="gif"</summary>
		///<param name="pstrImage">路径名称,例如:"res='2011' restype='gif'"</param>
		///<param name="sImageName">资源名称,例如:"2011"</param>
		///<param name="sImageResType">资源类型,例如:"gif"</param>
		///<returns></returns>
		void	ParseFileNameBy(IN LPCTSTR pstrImage,OUT CDuiString& sImageName,OUT CDuiString& sImageResType);
		void	DrawFrame( HDC hDC );		// 绘制GIF每帧
		Gdiplus::Image*	LoadGifFromFile(LPCTSTR pstrGifPath);
		Gdiplus::Image* LoadGifFromMemory( LPVOID pBuf,size_t dwSize );

	private:
		Gdiplus::Image	*m_pGifImage;
		UINT			m_nFrameCount;				// gif图片总帧数
		UINT			m_nFramePosition;			// 当前放到第几帧
		Gdiplus::PropertyItem*	m_pPropertyItem;	// 帧与帧之间间隔时间

		CDuiString		m_sBkImage;
		bool			m_bIsAutoPlay;				// 是否自动播放gif
		bool			m_bIsAutoSize;				// 是否自动根据图片设置大小
		bool			m_bIsPlaying;
		
		///<remark by="Aweigh" date="2017/3/29">
		/// bug:当窗口只有CGifAnimUI控件因为没有初始化GDI+导致加载gif图片失败
		///</remark>
#ifdef _USE_GDIPLUS
		ULONG_PTR				m_gdiplusToken;
		GdiplusStartupInput		m_gdiplusStartupInput;
#endif
	};
}

#endif // GifAnimUI_h__
