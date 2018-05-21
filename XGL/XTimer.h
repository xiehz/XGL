#pragma once
#include "XGL.h"

namespace XGL {

	class XGL_API XTimer
	{
	public:
		XTimer();
		virtual ~XTimer();
	public:
		typedef __int64 Timer_t;

		static XTimer* instance();

		/** Get the timers tick value.*/
		Timer_t tick() const;

		/** Set the start.*/
		void setStartTick() { _startTick = tick(); }
		void setStartTick(Timer_t t) { _startTick = t; }
		Timer_t getStartTick() const { return _startTick; }


		/** Get elapsed time in seconds.*/
		inline double time_s() const { return delta_s(_startTick, tick()); }

		/** Get elapsed time in milliseconds.*/
		inline double time_m() const { return delta_m(_startTick, tick()); }

		/** Get elapsed time in microseconds.*/
		inline double time_u() const { return delta_u(_startTick, tick()); }

		/** Get elapsed time in nanoseconds.*/
		inline double time_n() const { return delta_n(_startTick, tick()); }

		/** Get the time in seconds between timer ticks t1 and t2.*/
		inline double delta_s(Timer_t t1, Timer_t t2) const { return (t2>t1) ? (double)(t2 - t1)*_secsPerTick : -(double)(t1 - t2)*_secsPerTick; }

		/** Get the time in milliseconds between timer ticks t1 and t2.*/
		inline double delta_m(Timer_t t1, Timer_t t2) const { return delta_s(t1, t2)*1e3; }

		/** Get the time in microseconds between timer ticks t1 and t2.*/
		inline double delta_u(Timer_t t1, Timer_t t2) const { return delta_s(t1, t2)*1e6; }

		/** Get the time in nanoseconds between timer ticks t1 and t2.*/
		inline double delta_n(Timer_t t1, Timer_t t2) const { return delta_s(t1, t2)*1e9; }

		/** Get the number of seconds per tick. */
		inline double getSecondsPerTick() const { return _secsPerTick; }

	protected:

		Timer_t _startTick;
		double  _secsPerTick;
	};
}
