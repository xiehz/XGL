#include "stdafx.h"
#include "XEvent.h"

using namespace XGL;
XEvent::XEvent():handled(false)
{
}

XGL::XEvent::XEvent(const XEvent & e)
{
	this->handled = e.handled;
	this->time = e.time;
}


XEvent::~XEvent()
{
}
