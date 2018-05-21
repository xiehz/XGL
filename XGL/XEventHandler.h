#pragma once
#include "XGL.h"
#include "XEventAdapter.h"

namespace XGL {
	class XGL_API XEventHandler
	{
	public:
		XEventHandler();
		virtual ~XEventHandler();

	public:
		virtual bool handle(const XEventAdapter& event) { return false; }
	};
}


