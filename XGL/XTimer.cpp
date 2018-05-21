#include "stdafx.h"
#include "XTimer.h"

using namespace XGL;
XTimer::XTimer()
{
	LARGE_INTEGER frequency;
	if (QueryPerformanceFrequency(&frequency))
	{
		_secsPerTick = 1.0 / (double)frequency.QuadPart;
	}
	else
	{
		_secsPerTick = 1.0;
	}

	setStartTick();
}


XTimer::~XTimer()
{
}

XTimer* XTimer::instance()
{
	static XTimer s_timer;
	return &s_timer;
}

XTimer::Timer_t XTimer::tick() const
{
	LARGE_INTEGER qpc;
	if (QueryPerformanceCounter(&qpc))
	{
		return qpc.QuadPart;
	}
	else
	{
		return 0;
	}
}
