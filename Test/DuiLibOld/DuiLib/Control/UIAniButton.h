#ifndef __UIANIBUTTON_H__
#define __UIANIBUTTON_H__

#pragma once

namespace DuiLib
{
	/**
	 * @brief png动画按钮
	 * 鼠标移入时开始播放动画，鼠标移出时，动画倒退回首帧
	 * 该按钮有以下3个属性：
	 * elapse: 动画播放间隔，单位毫秒，最低(即最快)10ms，默认30ms
	 * frames: 动画帧数，即水平分割成几等份
	 * alwaysplay: 是否循环播放动画，默认鼠标移入后只播放一次，即播放到最后一帧停止动画
	 * autoplay: 是否自动播放，默认鼠标移入后播放。autoplay为true时，则无须鼠标移入，即可播放
	 * 使用示例：<AniButton width="175" height="175" frames="18" bkimage="image\scanning.png" />
	 */
	class DUILIB_API CAniButtonUI : public CButtonUI
	{
	public:
		CAniButtonUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void DoInit();
		void DoEvent(TEventUI& event);

		void PaintBkImage(HDC hDC);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		UINT GetElapse();
		void SetElapse(UINT nElapse);

		UINT GetFrames();
		void SetFrames(UINT nFrames);

		bool IsAlwaysPlay();
		void SetAlwaysPlay(bool bAlways);

		bool IsAutoPlay();
		void SetAutoPlay(bool AutoPlay);

	private:
		UINT		m_nElapse; // 播放间隔，最低10ms
		UINT		m_nFrames; // 帧数
		UINT		m_nCurrentFrame; // 当前帧
		bool		m_bAlwaysPlay; // 是否一直播放
		bool		m_bAutoPlay; // 是否自动播放
	};

} // namespace DuiLib

#endif // __UIANIBUTTON_H__