#pragma once

namespace System
{
	/// <summary>
	/// 表示一个时间间隔
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
		///<summary> 获取当前 System.TimeSpan 结构所表示的时间间隔的天数部分</summary>
		///<returns>此实例的天数部分。 返回值可以是正数也可以是负数</returns>
		int Days();

		///<summary>获取当前 System.TimeSpan 结构所表示的时间间隔的小时数部分</summary>
		///<returns>当前 System.TimeSpan 结构的小时分量。 返回值的范围为 -23 到 23</returns>
		int Hours();

		///<summary>获取当前 System.TimeSpan 结构所表示的时间间隔的毫秒数部分</summary>
		///<returns>当前 System.TimeSpan 结构的毫秒分量。 返回值的范围为 -999 到 999</returns>
		int Milliseconds();

		///<summary>获取当前 System.TimeSpan 结构所表示的时间间隔的分钟数部分</summary>
		///<returns>当前 System.TimeSpan 结构的分钟分量。 返回值的范围为 -59 到 59</returns>
		int Minutes();

		///<summary>获取当前 System.TimeSpan 结构所表示的时间间隔的秒数部分</summary>
		///<returns>当前 System.TimeSpan 结构的秒分量。 返回值的范围为 -59 到 59</returns>
		int Seconds();

		///<summary>获取表示当前 System.TimeSpan 结构的值的刻度数</summary>
		///<returns>此实例包含的刻度数</returns>
		LONG64 Ticks();

		///<summary>获取以整天数和天的小数部分表示的当前 System.TimeSpan 结构的值</summary>
		///<returns>此实例表示的总天数</returns>
		double TotalDays();

		///<summary>获取以整小时数和小时的小数部分表示的当前 System.TimeSpan 结构的值</summary>
		///<param name=""></param>
		///<returns>此实例表示的总小时数</returns>
		double TotalHours();

		///<summary>获取以整毫秒数和毫秒的小数部分表示的当前 System.TimeSpan 结构的值</summary>
		///<returns>此实例表示的总毫秒数</returns>
		double TotalMilliseconds();

		///<summary>获取以整分钟数和分钟的小数部分表示的当前 System.TimeSpan 结构的值</summary>
		///<returns>此实例表示的总分钟数</returns>
		double TotalMinutes();

		///<summary>获取以整秒数和秒的小数部分表示的当前 System.TimeSpan 结构的值</summary>
		///<returns>此实例表示的总秒数</returns>
		double TotalSeconds();

		///<summary>返回其值为指定的 System.TimeSpan 对象和此实例的和的新的 System.TimeSpan 对象</summary>
		///<param name="ts">待添加的时间间隔</param>
		///<returns>一个新对象，表示此实例的值加 ts 的值</returns>
		TimeSpan Add(TimeSpan ts);

		///<summary>比较两个 System.TimeSpan 值，并返回一个整数，该整数指示第一个值是短于、等于还是长于第二个值</summary>
		///<param name="t1">要比较的第一个时间间隔</param>
		///<param name="t2">要比较的第二个时间间隔</param>
		///<returns>以下值之一。 值 说明 -1 t1 短于 t2。 0 t1 等于 t2。 1 t1 长于 t2。</returns>
		static int Compare(TimeSpan t1, TimeSpan t2);
		int CompareTo(TimeSpan value);

		///<summary>返回表示指定天数的 System.TimeSpan，其中对天数的指定精确到最接近的毫秒</summary>
		///<param name="value">天数，精确到最接近的毫秒</param>
		///<returns>表示 value 的对象</returns>
		static TimeSpan FromDays(double value);

		///<summary>返回表示指定小时数的 System.TimeSpan，其中对小时数的指定精确到最接近的毫秒</summary>
		///<param name="value">精确到最接近的毫秒的小时数</param>
		///<returns>表示 value 的对象</returns>
		static TimeSpan FromHours(double value);

		///<summary>返回表示指定毫秒数的 System.TimeSpan</summary>
		///<param name="value">毫秒数</param>
		///<returns>表示 value 的对象</returns>
		static TimeSpan FromMilliseconds(double value);

		///<summary>返回表示指定分钟数的 System.TimeSpan，其中对分钟数的指定精确到最接近的毫秒</summary>
		///<param name="value"> 分钟数，精确到最接近的毫秒</param>
		///<returns>表示 value 的对象</returns>
		static TimeSpan FromMinutes(double value);

		///<summary>返回表示指定秒数的 System.TimeSpan，其中对秒数的指定精确到最接近的毫秒</summary>
		///<param name="value">秒数，精确到最接近的毫秒</param>
		///<returns>表示 value 的对象</returns>
		static TimeSpan FromSeconds(double value);

		///<summary>返回表示指定时间的 System.TimeSpan，其中对时间的指定以刻度为单位</summary>
		///<param name="value">表示时间的刻度数</param>
		///<returns>表示 value 的对象</returns>
		static TimeSpan FromTicks(LONG64 value);


	private:
		static TimeSpan Interval(double value, int scale);
		static LONG64 TimeToTicks(int hour, int minute, int second);
	};
}