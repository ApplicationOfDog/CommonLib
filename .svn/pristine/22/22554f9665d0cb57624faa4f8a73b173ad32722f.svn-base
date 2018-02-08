#include "stdafx.h"
#include "DateTime.hpp"
#include "Convert.hpp"

namespace System
{
	#define  MaxYear 0x270f
	#define  MaxTicks  0x2bca2875f4373fffL
	#define  TicksPerMillisecond  0x2710L
	#define  DatePartDay  3
	#define  DatePartDayOfYear  1
	#define  DatePartMonth  2
	#define  DatePartYear  0
	#define  TicksPerHour  0x861c46800L
	#define  TicksPerDay  0xc92a69c000L
	#define  TicksPerMinute  600000000/*0x23c34600L*/
	#define  TicksPerSecond  10000000/*0x989680L*/
	#define  KindUnspecified  0L
	#define  KindUtc  0x4000000000000000L
	#define  MillisPerDay  86400000/*0x5265c00*/
	#define  MillisPerHour  3600000/*0x36ee80*/
	#define  MillisPerMinute  60000/*0xea60*/
	#define  MillisPerSecond  1000/*0x3e8*/
	#define  DaysPer400Years  0x23ab1
	#define  DaysPer100Years  36524/*0x8eac*/
	#define  DaysPer4Years  1461/*0x5b5*/
	#define  DaysPerYear  365/*0x16d*/
	#define  TicksMask  0x3fffffffffffffffL
	#define  MaxMillis  0x11efae44cb400L
	#define  KindLocalAmbiguousDst  13835058055282163712L
	#define  FileTimeOffset  0x701ce1722770000L

	int DateTime::DaysToMonth365[] = { 0, 0x1f, 0x3b, 90, 120, 0x97, 0xb5, 0xd4, 0xf3, 0x111, 0x130, 0x14e, DaysPerYear };
	int DateTime::DaysToMonth366[] = { 0, 0x1f, 60, 0x5b, 0x79, 0x98, 0xb6, 0xd5, 0xf4, 0x112, 0x131, 0x14f, 0x16e };


	bool DateTime::operator>(DateTime& dt){
		return internalTicks()>dt.internalTicks();
	}
	bool DateTime::operator >= (DateTime& dt){
		return internalTicks()>=dt.internalTicks();
	}
	bool DateTime::operator < (DateTime& dt){
		return internalTicks()<dt.internalTicks();
	}
	bool DateTime::operator <= (DateTime& dt){
		return internalTicks()<=dt.internalTicks();
	}
	bool DateTime::operator == (DateTime& dt){
		return internalTicks()==dt.internalTicks();
	}
	bool DateTime::operator != (DateTime& dt){
		return internalTicks()!=dt.internalTicks();
	}
	DateTime DateTime::operator +(TimeSpan& t)
	{
		LONG64 innerTicks = internalTicks();
		LONG64 num2 = t.Ticks();
		if ((num2 > (MaxTicks - innerTicks)) || (num2 < -innerTicks)){
			throw new out_of_range("t:ArgumentOutOfRange_DateArithmetic");
		}
		return DateTime(((LONG64) (innerTicks + num2)) | internalKind());
	}
	TimeSpan DateTime::operator -(DateTime& d){
		return TimeSpan(internalTicks() - d.internalTicks());
	}
	DateTime DateTime::operator - (TimeSpan& t)
	{
		LONG64 innerTicks = internalTicks();
		LONG64 num2 = t.Ticks();
		if ((innerTicks < num2) || ((innerTicks - MaxTicks) > num2)){
			throw new out_of_range("t:ArgumentOutOfRange_DateArithmetic");
		}
		return DateTime(((LONG64) (innerTicks - num2)) | internalKind());
	}
	DateTime::DateTime(){
		_dateData=0;
	}
	DateTime::DateTime(LONG64 ticks)
	{
		if ((ticks < 0L) || (ticks > MaxTicks)){
			throw out_of_range("ticks:ArgumentOutOfRange_DateTimeBadTicks");
		}
		_dateData= (ULONG64) ticks;
	}
	DateTime::DateTime(LONG64 ticks, DateTimeKind kind)
	{
		if ((ticks < 0L) || (ticks > MaxTicks))
		{
			throw out_of_range("ticks:ArgumentOutOfRange_DateTimeBadTicks");
		}
		if ((kind < Unspecified) || (kind > Local))
		{
			throw invalid_argument("Argument_InvalidDateTimeKind,kind");
		}
		_dateData = (ULONG64) (ticks | (((LONG64) kind) << 0x3e));
	}
	DateTime::DateTime(int year, int month, int day)
	{
		_dateData = (ULONG64) dateToTicks(year, month, day);
	}
	DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond)
	{
		if ((millisecond < 0) || (millisecond >= MillisPerSecond)){
			throw out_of_range("millisecond:ArgumentOutOfRange_Range");
		}
		LONG64 num = dateToTicks(year, month, day) + timeToTicks(hour, minute, second);
		num += millisecond * TicksPerMillisecond;
		if ((num < 0L) || (num > MaxTicks)){
			throw invalid_argument("Arg_DateTimeRange");
		}
		_dateData = (ULONG64) num;
	}
	int DateTime::Year(){
		return getDatePart(DatePartYear);
	}
	int DateTime::Month(){
		return getDatePart(DatePartMonth);
	}
	int DateTime::Day(){
		return getDatePart(DatePartDay);
	}		
	int DateTime::DayOfYear(){
		return getDatePart(DatePartDayOfYear);
	}
	int DateTime::Hour(){
		return (int) ((internalTicks() / TicksPerHour) % 0x18L);
	}
	int DateTime::Minute(){
		return (int) ((internalTicks() / TicksPerMinute) % 60L);
	}
	int DateTime::Second(){
		return (int) ((internalTicks() / TicksPerSecond) % 60L);
	}
	int DateTime::Millisecond(){
		return (int) ((internalTicks() / TicksPerMillisecond) % MillisPerSecond);
	}
	System::DayOfWeek DateTime::DayOfWeek()
	{
		//[TargetedPatchingOptOut("Performance critical to inline across NGen image boundaries"), __DynamicallyInvokable]
		return (System::DayOfWeek) ((int) (((internalTicks() / TicksPerDay) + 1L) % 7L));
	}
	LONG64 DateTime::Ticks(){
		return internalTicks();
	}
	bool DateTime::IsLeapYear(int year)
	{
		if ((year < 1) || (year > MaxYear)){
			throw out_of_range("year:ArgumentOutOfRange_Year");
		}
		if ((year % 4) != 0)
		{
			return FALSE;
		}
		if ((year % 100) == 0)
		{
			return ((year % 400) == 0);
		}
		return TRUE;
	}
	int DateTime::DaysInMonth(int year, int month)
	{
		if ((month < 1) || (month > 12)){
			throw out_of_range("month:ArgumentOutOfRange_Month");
		}
		int* numArray = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
		return (numArray[month] - numArray[month - 1]);
	}
	DateTime DateTime::AddMonths(int months)
	{
		if ((months < -120000) || (months > 0x1d4c0)){
			throw out_of_range("months:ArgumentOutOfRange_DateTimeBadMonths");
		}
		int datePart = this->getDatePart(DatePartYear);
		int month = this->getDatePart(DatePartMonth);
		int day = this->getDatePart(DatePartDay);
		int num4 = (month - 1) + months;
		if (num4 >= 0)
		{
			month = (num4 % 12) + 1;
			datePart += num4 / 12;
		}
		else
		{
			month = 12 + ((num4 + 1) % 12);
			datePart += (num4 - 11) / 12;
		}
		if ((datePart < 1) || (datePart > MaxYear/*0x270f*/))
		{
			throw out_of_range("months:ArgumentOutOfRange_DateArithmetic");
		}
		int num5 = DaysInMonth(datePart, month);
		if (day > num5)
		{
			day = num5;
		}
		return DateTime(((ULONG64) (dateToTicks(datePart, month, day) + (internalTicks() % TicksPerDay))) | internalKind());
	}
	DateTime DateTime::AddTicks(LONG64 value)
	{
		LONG64 innerTicks = internalTicks();
		if ((value > (MaxTicks - innerTicks)) || (value < -innerTicks))
		{
			throw out_of_range("value:ArgumentOutOfRange_DateArithmetic");
		}
		return DateTime(((ULONG64) (innerTicks + value)) | internalKind());
	}
	DateTime DateTime::AddYears(int value)
	{
		if ((value < -10000) || (value > TicksPerMillisecond))
		{
			throw out_of_range("years:ArgumentOutOfRange_DateTimeBadYears");
		}
		return this->AddMonths(value * 12);
	}
	int DateTime::Compare(DateTime t1, DateTime t2)
	{
		LONG64 internalTicks = t1.internalTicks();
		LONG64 num2 = t2.internalTicks();
		if (internalTicks > num2)
		{
			return 1;
		}
		if (internalTicks < num2)
		{
			return -1;
		}
		return 0;
	}
	int DateTime::CompareTo(DateTime value)
	{
		LONG64 innerTicks = value.internalTicks();
		LONG64 num2 = internalTicks();
		if (num2 > innerTicks)
		{
			return 1;
		}
		if (num2 < innerTicks)
		{
			return -1;
		}
		return 0;
	}
	DateTimeKind DateTime::Kind()
	{
		//[TargetedPatchingOptOut("Performance critical to inline across NGen image boundaries"), __DynamicallyInvokable]
		switch (internalKind())
		{
		case KindUnspecified:
			return Unspecified;

		case KindUtc:
			return Utc;
		}
		return Local;
	}
	//DateTime DateTime::UtcNow()
	//{
	//	FILETIME ft;
	//	GetSystemTimeAsFileTime(&ft);
	//	ULONG64 ftime = ft.dwLowDateTime | ((ULONG64)ft.dwHighDateTime<<32) ;

	//	//[TargetedPatchingOptOut("Performance critical to inline across NGen image boundaries"), SecuritySafeCritical, __DynamicallyInvokable]
	//	return DateTime((ULONG64) ((ftime + FileTimeOffset) | KindUtc));
	//}
	DateTime DateTime::Now()
	{
		SYSTEMTIME st;
		::GetLocalTime(&st); 
		return DateTime(st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	}
	DateTime DateTime::Today()
	{
		SYSTEMTIME st;
		::GetLocalTime(&st); 
		return DateTime(st.wYear,st.wMonth,st.wDay,0,0,0,0);
	}
	DateTime DateTime::Add(TimeSpan value)
	{
		return this->AddTicks(value.Ticks());
	}
	DateTime DateTime::AddDays(double value)
	{
		return this->add(value, MillisPerDay);
	}
	DateTime DateTime::AddHours(double value)
	{
		return this->add(value, MillisPerHour);
	}
	DateTime DateTime::AddMilliseconds(double value)
	{
		return this->add(value, 1);
	}
	DateTime DateTime::AddMinutes(double value)
	{
		return this->add(value, MillisPerMinute);
	}
	DateTime DateTime::AddSeconds(double value)
	{
		return this->add(value, MillisPerSecond);
	}
	


	String DateTime::ToString()
	{
		return ToString(NULL,true);
	}
	String DateTime::ToString(LPCTSTR format,bool bEn)
	{
		String formatTemp;
		if(format!=NULL && *format!=0) formatTemp = format;
		if(formatTemp.IsEmptyOrWhiteSpace()) formatTemp = _T("yyyy-MM-dd HH:mm:ss");

		TCHAR* MMMArrEn[]={_T("Jan"),_T("Feb"),_T("Mar"),_T("Apr"),_T("May"),_T("Jun"),_T("Jul"),_T("Aug"),_T("Sep"),_T("Oct"),_T("Nov"),_T("Dec")};
		TCHAR* MMMArrCN[]={_T("一月"),_T("二月"),_T("三月"),_T("四月"),_T("五月"),_T("六月"),_T("七月"),_T("八月"),_T("九月"),_T("十月"),_T("十一月"),_T("十二月")};
		TCHAR* WeekArrEn[]={_T("Sun"),_T("Mon"),_T("Tue"),_T("Web"),_T("Thu"),_T("Fri"),_T("Sat")};
		TCHAR* WeekArrCN[]={_T("星期日"),_T("星期一"),_T("星期二"),_T("星期三"),_T("星期四"),_T("星期五"),_T("星期六")};
		
		TCHAR** MMMArr=bEn?MMMArrEn:MMMArrCN;
		TCHAR** WeekArr=bEn?WeekArrEn:WeekArrCN;

		String year(Year());
		String tyear(Year()%100);
		String month(Month());
		String tmonth(month.length()==1?_T("0")+month:month);
		String day(Day());
		String tday=day.length()==1?_T("0")+day:day;
		String hour(Hour());
		String thour(hour.length()==1?_T("0")+hour:hour);
		String minute(Minute()),tminute(minute.length()==1?_T("0")+minute:minute);
		String second(Second()),tsecond(second.length()==1?_T("0")+second:second);
		String millisec(Millisecond()),week(DayOfYear());
		double oNumber=Millisecond()/1000;

		formatTemp.Replace(_T("yyyy"),year,FALSE);
		formatTemp.Replace(_T("yyy"),year,FALSE);
		formatTemp.Replace(_T("yy"),tyear,FALSE);
		formatTemp.Replace(_T("y"),tyear,FALSE);

		formatTemp.Replace(_T("MMM"),MMMArr[Month()-1],FALSE);
		formatTemp.Replace(_T("MM"),tmonth,FALSE);
		formatTemp.Replace(_T("M"),month,FALSE);

		formatTemp.Replace(_T("dd"),tday,FALSE);
		formatTemp.Replace(_T("d"),day,FALSE);

		formatTemp.Replace(_T("HH"),thour,FALSE);
		formatTemp.Replace(_T("H"),hour,FALSE);
		formatTemp.Replace(_T("hh"),thour,FALSE);
		formatTemp.Replace(_T("h"),hour,FALSE);

		formatTemp.Replace(_T("mm"),tminute,FALSE);
		formatTemp.Replace(_T("m"),minute,FALSE);

		formatTemp.Replace(_T("ss"),tsecond,FALSE);
		formatTemp.Replace(_T("s"),second,FALSE);

		formatTemp.Replace(_T("fff"),millisec,FALSE);
		formatTemp.Replace(_T("ff"),Convert::ToString(Millisecond()/10),FALSE);
		formatTemp.Replace(_T("f"),Convert::ToString(Millisecond()/100),FALSE);

		formatTemp.Replace(_T("EEE"),month,FALSE);
		return formatTemp;
	}

	//bool DateTime::TryParse(IN LPCTSTR s,OUT DateTime* result)
	//{
	//	if(s==NULL||*s==0) return false;

	//	//2016-04-22 10:24:34
	//}


	////////////////////////////////////////////////////////////////////////////////////////////////
	///private 
	DateTime DateTime::add(double value, int scale)
	{
		LONG64 num = (LONG64) ((value * scale) + ((value >= 0.0) ? 0.5 : -0.5));
		if ((num <= -315537897600000L) || (num >= MaxMillis)){
			throw out_of_range("value:ArgumentOutOfRange_AddValue");
		}
		return this->AddTicks(num * TicksPerMillisecond);
	}
	ULONG64 DateTime::internalKind(){
		return (_dateData & KindLocalAmbiguousDst);
	}
	LONG64 DateTime::internalTicks()
	{
		return ((LONG64) _dateData) & TicksMask;
	}
	LONG64 DateTime::dateToTicks(int year, int month, int day)
	{
		if (((year >= 1) && (year <= MaxYear/*0x270f*/)) && ((month >= 1) && (month <= 12)))
		{
			int* numArray = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
			if ((day >= 1) && (day <= (numArray[month] - numArray[month - 1])))
			{
				int num = year - 1;
				int num2 = ((((((num * DaysPerYear) + (num / 4)) - (num / 100)) + (num / 400)) + numArray[month - 1]) + day) - 1;
				return (LONG64)(num2 * TicksPerDay);
			}
		}
		throw new out_of_range("ArgumentOutOfRange_BadYearMonthDay");
	}
	LONG64 DateTime::timeToTicks(int hour, int minute, int second)
	{
		if ((((hour < 0) || (hour >= 0x18)) || ((minute < 0) || (minute >= 60))) || ((second < 0) || (second >= 60)))
		{
			throw out_of_range("ArgumentOutOfRange_BadHourMinuteSecond");
		}
		return timeSpan_TimeToTicks(hour, minute, second);
	}
	LONG64 DateTime::timeSpan_TimeToTicks(int hour, int minute, int second)
	{
		LONG64 num = ((hour * 0xe10L) + (minute * 60L)) + second;
		if ((num > 0xd6bf94d5e5L) || (num < -922337203685L))
		{
			throw out_of_range("Overflow_TimeSpanTooLong");
		}
		return (num * TicksPerSecond);
	}
	int DateTime::getDatePart(int part)
	{
		int num2 = (int) (internalTicks() / TicksPerDay);
		int num3 = num2 / DaysPer400Years;
		num2 -= num3 * DaysPer400Years;
		int num4 = num2 / DaysPer100Years;
		if (num4 == 4)
		{
			num4 = 3;
		}
		num2 -= num4 * DaysPer100Years;
		int num5 = num2 / DaysPer4Years;
		num2 -= num5 * DaysPer4Years;
		int num6 = num2 / DaysPerYear;
		if (num6 == 4)
		{
			num6 = 3;
		}
		if (part == 0)
		{
			return (((((num3 * 400) + (num4 * 100)) + (num5 * 4)) + num6) + 1);
		}
		num2 -= num6 * DaysPerYear;
		if (part == 1)
		{
			return (num2 + 1);
		}
		int* numArray = ((num6 == 3) && ((num5 != 0x18) || (num4 == 3))) ? DaysToMonth366 : DaysToMonth365;
		int index = num2 >> 6;
		while (num2 >= numArray[index])
		{
			index++;
		}
		if (part == 2)
		{
			return index;
		}
		return ((num2 - numArray[index - 1]) + 1);
	}
}