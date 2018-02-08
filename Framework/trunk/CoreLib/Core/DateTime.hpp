#pragma once

namespace System
{
	/// <summary>
	/// 指定System.DateTime对象是表示本地时间、协调通用时间 (UTC)，还是既不指定为本地时间，也不指定为 UTC
	/// </summary>
	enum DateTimeKind
	{
		Unspecified = 0,//表示的时间既未指定为本地时间，也未指定为协调通用时间 (UTC)。
		Utc = 1,		//表示的时间为 UTC。
		Local = 2,		//表示的时间为本地时间。
	};

	/// <summary>
	/// 指定一周的某天
	/// </summary>
	enum DayOfWeek
	{
		Sunday = 0,		//星期日。
		Monday = 1,		//星期一。
		Tuesday = 2,	//星期二。
		Wednesday = 3,	//星期三。
		Thursday = 4,	//星期四。
		Friday = 5,		//星期五。
		Saturday = 6,	//星期六。
	};

	/// <summary>
	/// 表示时间上的一刻，通常以日期和当天的时间表示。
	/// </summary>
	struct API_CORE DateTime
	{
	private:
		ULONG64 _dateData;
		static int DaysToMonth365[];
		static int DaysToMonth366[];

	public:
		int Year();
		int Month();
		int Day();		
		int DayOfYear();
		int Hour();
		int Minute();
		int Second();
		int Millisecond();
		LONG64 Ticks();//获取表示此实例的日期和时间的计时周期数

	public:
		DateTime();
		DateTime(IN LONG64 ticks);
		DateTime(IN LONG64 ticks,IN DateTimeKind kind);
		DateTime(IN int year,IN int month,IN int day);
		DateTime(IN int year,IN int month,IN int day,IN int hour,IN int minute,IN int second=0,IN int millisecond=0);

		bool operator > (IN DateTime& dt);
		bool operator >= (IN DateTime& dt);
		bool operator < (IN DateTime& dt);
		bool operator <= (IN DateTime& dt);
		bool operator == (IN DateTime& dt);
		bool operator != (IN DateTime& dt);
		DateTime operator + (IN TimeSpan& t);
		TimeSpan operator - (IN DateTime& d);
		DateTime operator - (IN TimeSpan& t);

		///<summary>返回一个新的 System.DateTime，它将指定的月数加到此实例的值上</summary>
		///<param name="months">月份数。 months 参数可以是负数也可以是正数</param>
		///<returns>一个对象，其值是此实例所表示的日期和时间与 months 所表示的时间之和</returns>
		DateTime AddMonths(IN int months);

		///<summary>返回一个新的 System.DateTime，它将指定的计时周期数加到此实例的值上</summary>
		///<param name="value">以 100 纳秒为单位的计时周期数。 value 参数可以是正数也可以是负数</param>
		///<returns> 一个对象，其值是此实例所表示的日期和时间与 value 所表示的时间之和</returns>
		DateTime AddTicks(IN LONG64 value);

		///<summary>返回一个新的 System.DateTime，它将指定的年份数加到此实例的值上</summary>
		///<param name="value">年份数。 value 参数可以是负数也可以是正数</param>
		///<returns>一个对象，其值是此实例所表示的日期和时间与 value 所表示的年份数之和</returns>
		DateTime AddYears(IN int value);

		///<summary>获取此实例所表示的日期是星期几</summary>
		DayOfWeek DayOfWeek();

		///<summary>获取一个值，该值指示由此实例表示的时间是基于本地时间、协调世界时 (UTC)，还是两者皆否</summary>
		///<returns>用于指示当前时间表示的含义的枚举值之一。 默认值为 System.DateTimeKind.Unspecified</returns>
		DateTimeKind Kind();

		///<summary>获取一个 System.DateTime 对象，该对象设置为此计算机上的当前日期和时间，表示为本地时间</summary>
		///<returns>其值为当前日期和时间的对象</returns>
		static DateTime Now();

		///<summary> 获取一个 System.DateTime 对象，该对象设置为此计算机上的当前日期和时间，表示为协调通用时间 (UTC)</summary>
		///<returns>其值为当前 UTC 日期和时间的对象</returns>
		//static DateTime UtcNow();

		///<summary>获取当前日期</summary>
		///<returns>一个对象，设置为当天日期，其时间组成部分设置为 00:00:00</returns>
		static DateTime Today();

		///<summary>返回指定的年份是否为闰年的指示</summary>
		///<returns>如果 year 为闰年，则为 true；否则为 false</returns>
		static bool IsLeapYear(IN int year);

		///<summary>返回指定年和月中的天数</summary>
		///<param name="year">年</param>
		///<param name="month">月（介于 1 到 12 之间的一个数字）</param>
		///<returns>指定 year 中 month 的天数。 例如，如果 month 等于 2（表示二月），则返回值为 28 或 29，具体取决于 year 是否为闰年</returns>
		static int DaysInMonth(IN int year,IN int month);

		///<summary>对两个 System.DateTime 的实例进行比较，并返回一个指示第一个实例是早于、等于还是晚于第二个实例的整数</summary>
		///<param name="t1">要比较的第一个对象</param>
		///<param name="t2">要比较的第二个对象</param>
		///<returns>有符号数字，指示 t1 和 t2 的相对值。 值类型 Condition 小于零 t1 早于 t2。 零 t1 与 t2 相同。 大于零 t1 晚于t2</returns>
		static int Compare(IN DateTime t1,IN DateTime t2);
		int CompareTo(IN DateTime value);

		///<summary>返回一个新的 System.DateTime，它将指定 System.TimeSpan 的值加到此实例的值上</summary>
		///<param name="value">正或负时间间隔</param>
		///<returns>一个对象，其值是此实例所表示的日期和时间与 value 所表示的时间间隔之和</returns>
		DateTime Add(IN TimeSpan value);

		///<summary>返回一个新的 System.DateTime，它将指定的天数加到此实例的值上</summary>
		///<param name="value">由整数和小数部分组成的天数。 value 参数可以是负数也可以是正数</param>
		///<returns>一个对象，其值是此实例所表示的日期和时间与 value 所表示的天数之和</returns>
		DateTime AddDays(IN double value);

		///<summary>返回一个新的 System.DateTime，它将指定的小时数加到此实例的值上</summary>
		///<param name="value">由整数和小数部分组成的小时数。 value 参数可以是负数也可以是正数</param>
		///<returns>一个对象，其值是此实例所表示的日期和时间与 value 所表示的小时数之和</returns>
		DateTime AddHours(IN double value);

		///<summary>返回一个新的 System.DateTime，它将指定的毫秒数加到此实例的值上</summary>
		///<param name="value">由整数和小数部分组成的毫秒数。 value 参数可以是负数也可以是正数。 请注意，该值被舍入到最近的整数</param>
		///<returns>一个对象，其值是此实例所表示的日期和时间与 value 所表示的毫秒数之和</returns>
		DateTime AddMilliseconds(IN double value);

		///<summary>返回一个新的 System.DateTime，它将指定的分钟数加到此实例的值上</summary>
		///<param name="value">由整数和小数部分组成的分钟数。 value 参数可以是负数也可以是正数</param>
		///<returns>一个对象，其值是此实例所表示的日期和时间与 value 所表示的分钟数之和</returns>
		DateTime AddMinutes(IN double value);

		///<summary>返回一个新的 System.DateTime，它将指定的秒数加到此实例的值上</summary>
		///<param name="value">由整数和小数部分组成的秒数。 value 参数可以是负数也可以是正数</param>
		///<returns>一个对象，其值是此实例所表示的日期和时间与 value 所表示的秒数之和</returns>
		DateTime AddSeconds(IN double value);

		///<summary>将日期和时间格式化
		/// 例如:635969174742412109(ticks) => 2016-04-22 10:24:34
		///</summary>
		String ToString();
		///<param name="format">格式化时间字符串,例如:"yyyy-MM-dd HH:mm:ss"</param>
		String ToString(IN LPCTSTR format,IN bool bEn=true);		
		
		///<summary>将日期和时间的指定字符串表示形式转换为其 System.DateTime 等效项，并返回一个指示转换是否成功的值</summary>
		///<param name="s">包含要转换的日期和时间的字符串,例如:2007-</param>
		///<param name="result">当此方法返回时,如果转换成功,则包含与s中包含的日期和时间等效的DateTime值</param>
		///<returns>如果 s 参数成功转换，则为 true；否则为 false</returns>
		//static bool TryParse(IN LPCTSTR s,OUT DateTime* result);

	private:
		DateTime add(double value, int scale);
		ULONG64 internalKind();
		LONG64 internalTicks();
		LONG64 dateToTicks(int year, int month, int day);
		LONG64 timeToTicks(int hour, int minute, int second);
		LONG64 timeSpan_TimeToTicks(int hour, int minute, int second);
		int getDatePart(int part);
	};
}