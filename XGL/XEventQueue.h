#pragma once
#include <mutex>
#include <queue>
#include "XGL.h"
#include "XTimer.h"
#include "XEventAdapter.h"

namespace XGL {
	class XGL_API XEventQueue 
	{
	public:
		XEventQueue();
		virtual ~XEventQueue();

	public:
		void clear();
		bool isEmpty();
		//主线程
		void addEvent(XEventAdapter* event);
		//渲染线程
		void takeEvents(std::queue<XEventAdapter>& events);

		void setStartTick(XTimer::Timer_t tick) { _startTick = tick; clear(); }
		XTimer::Timer_t getStartTick() const { return _startTick; }
		double getTime() const { return XTimer::instance()->delta_s(_startTick, XTimer::instance()->tick()); }
	public:
		//主线程
		virtual  int lButtonDown(WPARAM state, int x, int y);
		virtual  int lButtonUp(WPARAM state, int x, int y);
		virtual  int mButtonDown(WPARAM state, int x, int y);
		virtual  int mButtonUp(WPARAM state, int x, int y);
		virtual  int rButtonDown(WPARAM state, int x, int y) ;
		virtual  int rButtonUp(WPARAM state, int x, int y) ;
		virtual  int mouseMove(WPARAM state, int x, int y) ;
		virtual  int mouseWheel(int state, int delta, int x, int y) ;
		virtual int  windowSize(int x, int y, int width, int height);
	private:
		mutable std::mutex* mutex;
		std::queue<XEventAdapter*>* queues;
		//累积状态
		XEventAdapter* _accumulateEventState;
		XTimer::Timer_t                _startTick;
	};
}


