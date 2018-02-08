#include "stdafx.h"
#include "TimeSpan.hpp"

namespace System
{
	#define  TicksPerMillisecond  0x2710L
	#define  TicksPerSecond  0x989680L
	#define  TicksPerMinute  0x23c34600L
	#define  TicksPerHour  0x861c46800L
	#define  TicksPerDay  0xc92a69c000L
	#define  MaxSeconds  0xd6bf94d5e5L
	#define  MinSeconds  -922337203685L
	#define  MaxMilliSeconds  0x346dc5d638865L
	#define  MinMilliSeconds  -922337203685477L
	#define  TicksPerTenthSecond  0xf4240L
	#define  MillisecondsPerTick  0.0001
	#define  SecondsPerTick  1E-07
	#define  MinutesPerTick  1.6666666666666667E-09
	#define  HoursPerTick  2.7777777777777777E-11
	#define  DaysPerTick  1.1574074074074074E-12
	#define  MillisPerSecond  0x3e8
	#define  MillisPerMinute  0xea60
	#define  MillisPerHour  0x36ee80
	#define  MillisPerDay  0x5265c00

	TimeSpan::TimeSpan(LONG64 ticks)
	{
		_ticks = ticks;
	}
	TimeSpan::TimeSpan(int hours, int minutes, int seconds)
	{
		_ticks = TimeToTicks(hours, minutes, seconds);
	}
	TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds)
	{
		LONG64 num = ((((((days * 0xe10L) * 0x18L) + (hours * 0xe10L)) + (minutes * 60L)) + seconds) * 0x3e8L) + milliseconds;
		if ((num > MaxMilliSeconds) || (num < MinMilliSeconds))
		{
			throw out_of_range("Overflow_TimeSpanTooLong");
		}
		_ticks = num * TicksPerMillisecond;
	}
	LONG64 TimeSpan::Ticks(){
		return _ticks;
	}
	int TimeSpan::Days()
	{
		return (int) (_ticks / TicksPerDay/*TicksPerHour*/);
	}
	int TimeSpan::Hours()
	{
		return (int) ((_ticks / TicksPerHour) % 0x18L);
	}
	int TimeSpan::Milliseconds()
	{
		return (int) ((_ticks / TicksPerMillisecond) % 0x3e8L);
	}
	int TimeSpan::Minutes()
	{
		return (int) ((_ticks / TicksPerMinute) % 60L);
	}
	int TimeSpan::Seconds()
	{
		return (int) ((_ticks / TicksPerSecond) % 60L);
	}
	double TimeSpan::TotalDays()
	{
		return (_ticks * DaysPerTick);
	}
	double TimeSpan::TotalHours()
	{
		return (_ticks * HoursPerTick);
	}
	double TimeSpan::TotalMilliseconds()
	{
		double num = _ticks * MillisecondsPerTick;
		if (num > 922337203685477)
		{
			return 922337203685477;
		}
		if (num < MinMilliSeconds)
		{
			return MinMilliSeconds;
		}
		return num;
		
	}
	double TimeSpan::TotalMinutes()
	{
		return (_ticks * MinutesPerTick);
	}
	double TimeSpan::TotalSeconds()
	{
		//[TargetedPatchingOptOut("Performance critical to inline across NGen image boundaries"), __DynamicallyInvokable]
		return (_ticks * SecondsPerTick);
	}
	TimeSpan TimeSpan::Add(TimeSpan ts)
	{
		LONG64 ticks = _ticks + ts._ticks;
		if (((_ticks >> 0x3f) == (ts._ticks >> 0x3f)) && ((_ticks >> 0x3f) != (ticks >> 0x3f)))
		{
			throw overflow_error("Overflow_TimeSpanTooLong");
		}
		return TimeSpan(ticks);
	}

	int TimeSpan::Compare(TimeSpan t1, TimeSpan t2)
	{
		if (t1._ticks > t2._ticks)
		{
			return 1;
		}
		if (t1._ticks < t2._ticks)
		{
			return -1;
		}
		return 0;
	}
	int TimeSpan::CompareTo(TimeSpan value)
	{
		LONG64 num = value._ticks;
		if (_ticks > num)
		{
			return 1;
		}
		if (_ticks < num)
		{
			return -1;
		}
		return 0;
	}
	TimeSpan TimeSpan::FromDays(double value)
	{
		return Interval(value, MillisPerDay);
	}
	TimeSpan TimeSpan::FromHours(double value)
	{
		return Interval(value, MillisPerHour);
	}
	TimeSpan TimeSpan::FromMilliseconds(double value)
	{
		return Interval(value, 1);
	}
	TimeSpan TimeSpan::FromMinutes(double value)
	{
		return Interval(value, MillisPerMinute);
	}
	TimeSpan TimeSpan::FromSeconds(double value)
	{
		return Interval(value, MillisPerSecond);
	}
	TimeSpan FromTicks(LONG64 value)
	{
		return TimeSpan(value);
	}
	///////////////////////////////////////////////////////////////////
	/// private 
	TimeSpan TimeSpan::Interval(double value, int scale)
	{
		/*if (double.IsNaN(value))
		{
			throw new ArgumentException(Environment.GetResourceString("Arg_CannotBeNaN"));
		}*/
		double num = value * scale;
		double num2 = num + ((value >= 0.0) ? 0.5 : -0.5);
		if ((num2 > 922337203685477) || (num2 < MinMilliSeconds))
		{
			throw overflow_error("Overflow_TimeSpanTooLong");
		}
		return TimeSpan(((LONG64) num2) * TicksPerMillisecond);
	}
	LONG64 TimeSpan::TimeToTicks(int hour, int minute, int second)
	{
		LONG64 num = ((hour * 0xe10L) + (minute * 60L)) + second;
		if ((num > MaxSeconds) || (num < MinSeconds))
		{
			throw out_of_range("Overflow_TimeSpanTooLong");
		}
		return (num * TicksPerSecond);
	}
}