#pragma once

namespace System
{
	/// <summary>
	/// ��ʾһ��ʱ����
	/// </summary>
	struct API_CORE TimeSpan
	{
	private:
		LONG64 _ticks;

	public:
		TimeSpan(LONG64 ticks);
		TimeSpan(int hours, int minutes, int seconds);
		TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds);

	public:
		///<summary> ��ȡ��ǰ System.TimeSpan �ṹ����ʾ��ʱ��������������</summary>
		///<returns>��ʵ�����������֡� ����ֵ����������Ҳ�����Ǹ���</returns>
		int Days();

		///<summary>��ȡ��ǰ System.TimeSpan �ṹ����ʾ��ʱ������Сʱ������</summary>
		///<returns>��ǰ System.TimeSpan �ṹ��Сʱ������ ����ֵ�ķ�ΧΪ -23 �� 23</returns>
		int Hours();

		///<summary>��ȡ��ǰ System.TimeSpan �ṹ����ʾ��ʱ�����ĺ���������</summary>
		///<returns>��ǰ System.TimeSpan �ṹ�ĺ�������� ����ֵ�ķ�ΧΪ -999 �� 999</returns>
		int Milliseconds();

		///<summary>��ȡ��ǰ System.TimeSpan �ṹ����ʾ��ʱ�����ķ���������</summary>
		///<returns>��ǰ System.TimeSpan �ṹ�ķ��ӷ����� ����ֵ�ķ�ΧΪ -59 �� 59</returns>
		int Minutes();

		///<summary>��ȡ��ǰ System.TimeSpan �ṹ����ʾ��ʱ��������������</summary>
		///<returns>��ǰ System.TimeSpan �ṹ��������� ����ֵ�ķ�ΧΪ -59 �� 59</returns>
		int Seconds();

		///<summary>��ȡ��ʾ��ǰ System.TimeSpan �ṹ��ֵ�Ŀ̶���</summary>
		///<returns>��ʵ�������Ŀ̶���</returns>
		LONG64 Ticks();

		///<summary>��ȡ�������������С�����ֱ�ʾ�ĵ�ǰ System.TimeSpan �ṹ��ֵ</summary>
		///<returns>��ʵ����ʾ��������</returns>
		double TotalDays();

		///<summary>��ȡ����Сʱ����Сʱ��С�����ֱ�ʾ�ĵ�ǰ System.TimeSpan �ṹ��ֵ</summary>
		///<param name=""></param>
		///<returns>��ʵ����ʾ����Сʱ��</returns>
		double TotalHours();

		///<summary>��ȡ�����������ͺ����С�����ֱ�ʾ�ĵ�ǰ System.TimeSpan �ṹ��ֵ</summary>
		///<returns>��ʵ����ʾ���ܺ�����</returns>
		double TotalMilliseconds();

		///<summary>��ȡ�����������ͷ��ӵ�С�����ֱ�ʾ�ĵ�ǰ System.TimeSpan �ṹ��ֵ</summary>
		///<returns>��ʵ����ʾ���ܷ�����</returns>
		double TotalMinutes();

		///<summary>��ȡ�������������С�����ֱ�ʾ�ĵ�ǰ System.TimeSpan �ṹ��ֵ</summary>
		///<returns>��ʵ����ʾ��������</returns>
		double TotalSeconds();

		///<summary>������ֵΪָ���� System.TimeSpan ����ʹ�ʵ���ĺ͵��µ� System.TimeSpan ����</summary>
		///<param name="ts">����ӵ�ʱ����</param>
		///<returns>һ���¶��󣬱�ʾ��ʵ����ֵ�� ts ��ֵ</returns>
		TimeSpan Add(TimeSpan ts);

		///<summary>�Ƚ����� System.TimeSpan ֵ��������һ��������������ָʾ��һ��ֵ�Ƕ��ڡ����ڻ��ǳ��ڵڶ���ֵ</summary>
		///<param name="t1">Ҫ�Ƚϵĵ�һ��ʱ����</param>
		///<param name="t2">Ҫ�Ƚϵĵڶ���ʱ����</param>
		///<returns>����ֵ֮һ�� ֵ ˵�� -1 t1 ���� t2�� 0 t1 ���� t2�� 1 t1 ���� t2��</returns>
		static int Compare(TimeSpan t1, TimeSpan t2);
		int CompareTo(TimeSpan value);

		///<summary>���ر�ʾָ�������� System.TimeSpan�����ж�������ָ����ȷ����ӽ��ĺ���</summary>
		///<param name="value">��������ȷ����ӽ��ĺ���</param>
		///<returns>��ʾ value �Ķ���</returns>
		static TimeSpan FromDays(double value);

		///<summary>���ر�ʾָ��Сʱ���� System.TimeSpan�����ж�Сʱ����ָ����ȷ����ӽ��ĺ���</summary>
		///<param name="value">��ȷ����ӽ��ĺ����Сʱ��</param>
		///<returns>��ʾ value �Ķ���</returns>
		static TimeSpan FromHours(double value);

		///<summary>���ر�ʾָ���������� System.TimeSpan</summary>
		///<param name="value">������</param>
		///<returns>��ʾ value �Ķ���</returns>
		static TimeSpan FromMilliseconds(double value);

		///<summary>���ر�ʾָ���������� System.TimeSpan�����жԷ�������ָ����ȷ����ӽ��ĺ���</summary>
		///<param name="value"> ����������ȷ����ӽ��ĺ���</param>
		///<returns>��ʾ value �Ķ���</returns>
		static TimeSpan FromMinutes(double value);

		///<summary>���ر�ʾָ�������� System.TimeSpan�����ж�������ָ����ȷ����ӽ��ĺ���</summary>
		///<param name="value">��������ȷ����ӽ��ĺ���</param>
		///<returns>��ʾ value �Ķ���</returns>
		static TimeSpan FromSeconds(double value);

		///<summary>���ر�ʾָ��ʱ��� System.TimeSpan�����ж�ʱ���ָ���Կ̶�Ϊ��λ</summary>
		///<param name="value">��ʾʱ��Ŀ̶���</param>
		///<returns>��ʾ value �Ķ���</returns>
		static TimeSpan FromTicks(LONG64 value);


	private:
		static TimeSpan Interval(double value, int scale);
		static LONG64 TimeToTicks(int hour, int minute, int second);
	};
}