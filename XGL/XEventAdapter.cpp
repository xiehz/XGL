#include "stdafx.h"
#include "XEventAdapter.h"

using namespace XGL;

XEventAdapter::XEventAdapter()
{
	this->valid = true;
}

XGL::XEventAdapter::XEventAdapter(const XEventAdapter & event):XEvent(event)
{
	this->x = event.x;
	this->y = event.y;
	this->_buttonMask = event._buttonMask;
	this->_button = event._button;

	this->_windowX = event._windowX;
	this->_windowY = event._windowY;
	this->_windowWidth = event._windowWidth;
	this->_windowHeight = event._windowHeight;

	this->_Xmax = event._Xmax;
	this->_Xmin = event._Xmin;
	this->_Ymax = event._Ymax;
	this->_Ymin = event._Ymin;

	this->eventType = event.eventType;
	this->setScrollingMotion(event.getScrollingMotion());
	this->valid = event.valid;
}

XEventAdapter & XGL::XEventAdapter::operator=(const XEventAdapter & event)
{
	// TODO: 在此处插入 return 语句
	this->x = event.x;
	this->y = event.y;
	this->_buttonMask = event._buttonMask;
	this->_button = event._button;

	this->_windowX = event._windowX;
	this->_windowY = event._windowY;
	this->_windowWidth = event._windowWidth;
	this->_windowHeight = event._windowHeight;

	this->_Xmax = event._Xmax;
	this->_Xmin = event._Xmin;
	this->_Ymax = event._Ymax;
	this->_Ymin = event._Ymin;

	this->eventType = event.eventType;
	this->setScrollingMotion(event.getScrollingMotion());
	this->valid = event.valid;
	return *this;
}


XEventAdapter::~XEventAdapter()
{
}

void XGL::XEventAdapter::setInputRange(float Xmin, float Ymin, float Xmax, float Ymax)
{
	_Xmin = Xmin;
	_Ymin = Ymin;
	_Xmax = Xmax;
	_Ymax = Ymax;
}
