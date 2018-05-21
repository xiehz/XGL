#include "stdafx.h"
#include "XEventQueue.h"


XGL::XEventQueue::XEventQueue()
{
	this->mutex = new std::mutex();
	this->queues = new std::queue<XEventAdapter*>();
	this->_accumulateEventState = new XGL::XEventAdapter();
	this->setStartTick(XTimer::instance()->getStartTick());
}


XGL::XEventQueue::~XEventQueue()
{
	delete this->mutex;
	this->mutex = nullptr;
	delete this->queues;
	this->queues = nullptr;
	delete this->_accumulateEventState;
	this->_accumulateEventState = nullptr;
}

void XGL::XEventQueue::clear()
{
	std::lock_guard<std::mutex> locker(*this->mutex);
	XEventAdapter* temp;
	while (!queues->empty())
	{
		temp = queues->front();
		queues->pop();
		delete temp;
		temp = nullptr;
	}
}

bool XGL::XEventQueue::isEmpty()
{
	std::lock_guard<std::mutex> locker(*this->mutex);
	return queues->empty();
}

void XGL::XEventQueue::addEvent(XEventAdapter* event)
{
	std::lock_guard<std::mutex> locker(*this->mutex);
	queues->push(event);
}


void XGL::XEventQueue::takeEvents(std::queue<XEventAdapter>& events)
{
	std::lock_guard<std::mutex> locker(*this->mutex);
	XEventAdapter* temp;
	while (!queues->empty())
	{
		temp = queues->front();
		events.push(*temp);
		//events.push(XEventAdapter(*temp));
		queues->pop();
		delete temp;
		temp = nullptr;
	}
}


int XGL::XEventQueue::lButtonDown(WPARAM state, int x, int y)
{
	_accumulateEventState->setX(x);
	_accumulateEventState->setY(y);

	_accumulateEventState->setButtonMask(XEventAdapter::LEFT_MOUSE_BUTTON | _accumulateEventState->getButtonMask());

	XEventAdapter* event = new XEventAdapter(*_accumulateEventState);
	event->setEventType(XEventAdapter::PUSH);
	event->setButton(XEventAdapter::LEFT_MOUSE_BUTTON);
	event->setTime(getTime());


	addEvent(event);

	return 0;
}

int XGL::XEventQueue::lButtonUp(WPARAM state, int x, int y)
{
	_accumulateEventState->setX(x);
	_accumulateEventState->setY(y);

	_accumulateEventState->setButtonMask(~ XEventAdapter::LEFT_MOUSE_BUTTON & _accumulateEventState->getButtonMask());

	XEventAdapter* event = new XEventAdapter(*_accumulateEventState);
	event->setEventType(XEventAdapter::RELEASE);
	event->setTime(getTime());

	event->setButton(XEventAdapter::LEFT_MOUSE_BUTTON);

	addEvent(event);
	return 0;
}

int XGL::XEventQueue::mButtonDown(WPARAM state, int x, int y)
{
	_accumulateEventState->setX(x);
	_accumulateEventState->setY(y);

	_accumulateEventState->setButtonMask(XEventAdapter::MIDDLE_MOUSE_BUTTON | _accumulateEventState->getButtonMask());

	XEventAdapter* event = new XEventAdapter(*_accumulateEventState);
	event->setEventType(XEventAdapter::PUSH);
	event->setButton(XEventAdapter::MIDDLE_MOUSE_BUTTON);
	event->setTime(getTime());


	addEvent(event);
	return 0;
}

int XGL::XEventQueue::mButtonUp(WPARAM state, int x, int y)
{
	_accumulateEventState->setX(x);
	_accumulateEventState->setY(y);

	_accumulateEventState->setButtonMask(~XEventAdapter::MIDDLE_MOUSE_BUTTON & _accumulateEventState->getButtonMask());

	XEventAdapter* event = new XEventAdapter(*_accumulateEventState);
	event->setEventType(XEventAdapter::RELEASE);
	event->setTime(getTime());

	event->setButton(XEventAdapter::MIDDLE_MOUSE_BUTTON);

	addEvent(event);
	return 0;
}

int XGL::XEventQueue::rButtonDown(WPARAM state, int x, int y)
{
	_accumulateEventState->setX(x);
	_accumulateEventState->setY(y);

	_accumulateEventState->setButtonMask(XEventAdapter::RIGHT_MOUSE_BUTTON | _accumulateEventState->getButtonMask());

	XEventAdapter* event = new XEventAdapter(*_accumulateEventState);
	event->setEventType(XEventAdapter::PUSH);
	event->setButton(XEventAdapter::LEFT_MOUSE_BUTTON);
	event->setTime(getTime());

	addEvent(event);

	return 0;
}

int XGL::XEventQueue::rButtonUp(WPARAM state, int x, int y)
{
	_accumulateEventState->setX(x);
	_accumulateEventState->setY(y);

	_accumulateEventState->setButtonMask(~XEventAdapter::RIGHT_MOUSE_BUTTON & _accumulateEventState->getButtonMask());

	XEventAdapter* event = new XEventAdapter(*_accumulateEventState);
	event->setEventType(XEventAdapter::RELEASE);
	event->setTime(getTime());

	event->setButton(XEventAdapter::RIGHT_MOUSE_BUTTON);

	addEvent(event);
	return 0;
}

int XGL::XEventQueue::mouseMove(WPARAM state, int x, int y)
{
	_accumulateEventState->setX(x);
	_accumulateEventState->setY(y);

	XEventAdapter* event = new XEventAdapter(*_accumulateEventState);
	event->setEventType(event->getButtonMask() ? XEventAdapter::DRAG : XEventAdapter::MOVE);
	event->setTime(getTime());


	addEvent(event);

	return 0;
}

int XGL::XEventQueue::mouseWheel(int state, int delta, int x, int y)
{
	XEventAdapter* event = new XEventAdapter(*_accumulateEventState);
	event->setEventType(XEventAdapter::SCROLL);
	event->setScrollingMotion(delta<0 ? XEventAdapter::SCROLL_DOWN :
		XEventAdapter::SCROLL_UP );
	event->setTime(getTime());


	addEvent(event);
	return 0;
}

int XGL::XEventQueue::windowSize(int x, int y, int width, int height)
{
	int rect[4];
	rect[0] = x;
	rect[1] = y;
	rect[2] = width;
	rect[3] = height;

	_accumulateEventState->setWindowRect(rect);
	_accumulateEventState->setInputRange(x, y, width, height);
	
	XEventAdapter* event = new XGL::XEventAdapter(*_accumulateEventState);
	event->setEventType(XGL::XEventAdapter::RESIZE);
	event->setTime(getTime());

	addEvent(event);
	return 0;
}
