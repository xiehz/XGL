#pragma once
#include "XGL.h"

namespace XGL {
	class XGL_API XEvent
	{
	public:
		XEvent();
		XEvent(const XEvent& e);
		~XEvent();
	public:
		void setHandled(bool h) { handled = h; }
		bool getHandled()const { return handled; }

		void setTime(double t) { time = t; }
		double getTime()const { return time; }
	private:
		mutable bool handled;
		double time;
	};
}


