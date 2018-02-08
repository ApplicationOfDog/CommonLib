#ifndef __UIANIBUTTON_H__
#define __UIANIBUTTON_H__

#pragma once

namespace DuiLib
{
	/**
	 * @brief png������ť
	 * �������ʱ��ʼ���Ŷ���������Ƴ�ʱ���������˻���֡
	 * �ð�ť������3�����ԣ�
	 * elapse: �������ż������λ���룬���(�����)10ms��Ĭ��30ms
	 * frames: ����֡������ˮƽ�ָ�ɼ��ȷ�
	 * alwaysplay: �Ƿ�ѭ�����Ŷ�����Ĭ����������ֻ����һ�Σ������ŵ����һֹ֡ͣ����
	 * autoplay: �Ƿ��Զ����ţ�Ĭ���������󲥷š�autoplayΪtrueʱ��������������룬���ɲ���
	 * ʹ��ʾ����<AniButton width="175" height="175" frames="18" bkimage="image\scanning.png" />
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
		UINT		m_nElapse; // ���ż�������10ms
		UINT		m_nFrames; // ֡��
		UINT		m_nCurrentFrame; // ��ǰ֡
		bool		m_bAlwaysPlay; // �Ƿ�һֱ����
		bool		m_bAutoPlay; // �Ƿ��Զ�����
	};

} // namespace DuiLib

#endif // __UIANIBUTTON_H__