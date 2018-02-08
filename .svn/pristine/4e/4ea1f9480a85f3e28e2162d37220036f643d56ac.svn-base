#ifndef __UIHOTSHOWLAYOUT_H__
#define __UIHOTSHOWLAYOUT_H__

#pragma once

namespace DuiLib
{
	/**
	 * @brief 响应鼠标悬浮事件的容器
	 * 鼠标移入容器时，显示带有自定义属性hotshow="true"的子控件，隐藏带有属性hotshow="false"的子控件
	 * 如果容器设置属性elapse值大于等于0，则容器鼠标移入移出时会向窗口发送Notify消息
	 * 使用示例：
	 * <HotShowLayout elapse="500">
	 *   <Control text="一般控件" />
	 *   <Control text="悬浮时显示" hotshow="true" />
	 *   <Control text="悬浮时隐藏" hotshow="false" />
	 * </HotShowLayout>
	 */
	class DUILIB_API CHotShowLayoutUI : public CVerticalLayoutUI
	{
	public:
		CHotShowLayoutUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void DoInit();
		void DoEvent(TEventUI& event);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		int  GetElapse();
		void SetElapse(int nElapse);

		int  SetHotShow(bool bHot);

	private:
		UINT	m_uState; // 控件状态
		int		m_nElapse; // hover事件触发延迟，单位毫秒，默认-1，即不启用该功能
	};

} // namespace DuiLib

#endif // __UIHOTSHOWLAYOUT_H__