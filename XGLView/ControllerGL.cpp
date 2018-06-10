///////////////////////////////////////////////////////////////////////////////
// ControllerGL.cpp
// ================
// Derived Controller class for OpenGL window
// It is the controller of OpenGL rendering window. It initializes DC and RC,
// when WM_CREATE called, then, start new thread for OpenGL rendering loop.
//
// When this class is constructed, it gets the pointers to model and view
// components.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gamil.com)
// CREATED: 2008-09-15
// UPDATED: 2017-10-18
///////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include <list>
#include <queue>
#include <iostream>
#include "XGL\Log.h"
#include "XGL\ViewGL.h"
#include "XGLModel\ModelGL.h"
#include "XGL\TrackballCamera.h"
#include "xgl\XEventHandler.h"
#include "xgl\XCamera.h"
#include "xgl\XTimer.h"
#include "ControllerGL.h"


using namespace XGL;
using namespace XGLModel;
using namespace XGLView;




///////////////////////////////////////////////////////////////////////////////
// default contructor
///////////////////////////////////////////////////////////////////////////////
ControllerGL::ControllerGL() : model(nullptr), view(nullptr), 
loopFlag(false)
{
	eventQueue = new XEventQueue();
}



int XGLView::ControllerGL::setup(XGLModel::ModelGL * model, XGL::ViewGL * view)
{
	this->model = model;
	this->view = view;

	RECT rect;
	GetClientRect(handle,&rect );
	this->eventQueue->windowSize(0, 0, rect.right - rect.left, rect.bottom - rect.top );

	return 1;
}

int XGLView::ControllerGL::render()
{
	// create a thread for OpenGL rendering
	glThread = std::thread(&ControllerGL::runThread, this);
	loopFlag = true;
	log("Created a rendering thread for OpenGL.");

	XTimer::instance()->setStartTick();
	return 1;

}

///////////////////////////////////////////////////////////////////////////////
// handle WM_CLOSE
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::close()
{
    // wait for rendering thread is terminated
    loopFlag = false;
	if (glThread.native_handle() != nullptr)
	{
		if (glThread.joinable())
			glThread.join();
	}

    log("OpenGL window is destroyed.");
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_CREATE
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::create()
{
    // create a OpenGL rendering context
    if(!view->createContext(handle, 32, 24, 8))
    {
        log("[ERROR] Failed to create OpenGL rendering context from ControllerGL::create().");
        return -0;
    }

    return 1;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_PAINT
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::paint()
{
	log("dirty.... paint again");
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_COMMAND
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::command(int id, int cmd, LPARAM msg)
{
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// rendering thread
// initialize OpenGL states and start rendering loop
///////////////////////////////////////////////////////////////////////////////
void ControllerGL::runThread()
{
    // set the current RC in this thread
    BOOL result =::wglMakeCurrent(view->getDC(), view->getRC());

	glewInit();
	if (GLenum res = glewInit() != GLEW_OK) {
		return;
	}
	
	// cofigure projection matrix
	RECT rect;
	::GetClientRect(handle, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// initialize OpenGL states
	model->init();
	log("Initialized OpenGL states.");

	model->setViewport(new int[4] {0,0,width,height});
    log("Initialized OpenGL window size.");

    // rendering loop
    log("Entering OpenGL rendering thread...");
    while(loopFlag)
    {

		//std::this_thread::yield();      // yield to other processes or threads
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));  // yield to other processes or threads

		//event 
		std::queue<XEventAdapter> events;
		this->eventQueue->takeEvents(events);
		int* viewport;
		float x, y;

		while (events.size()) {
			XEventAdapter& e = events.front();

			viewport = model->getViewport();
			XEventAdapter::PointerDataList pdlist;

			x = e.getX();
			y = e.getY();

			y = height - y;
			PointerData pd0(x, 0, width,
				y, 0, height);
			pdlist.push_back(&pd0);

			PointerData pd1((x - viewport[0]) / viewport[2] * 2.0f - 1.0f, -1.0, 1.0,
				(y - viewport[1]) / viewport[3] * 2.0f - 1.0f, -1.0, 1.0);
			pdlist.push_back(&pd1);

			e.setPointerDataList(pdlist);

			model->getCamera()->handle(e);
			events.pop();
		}

		model->draw();
		view->swapBuffers();
    }

    // close OpenGL Rendering Context (RC)
    view->closeContext(handle);
    ::wglMakeCurrent(0, 0);             // unset RC
    log("Closed OpenGL context.");

    log("Exit OpenGL rendering thread.");

}



///////////////////////////////////////////////////////////////////////////////
// handle Left mouse down
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::lButtonDown(WPARAM state, int x, int y)
{
	this->eventQueue->lButtonDown(state, x, y);
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle Left mouse up
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::lButtonUp(WPARAM state, int x, int y)
{
	this->eventQueue->lButtonUp(state, x, y);
    return 0;
}

int XGLView::ControllerGL::mButtonDown(WPARAM state, int x, int y)
{
	this->eventQueue->mButtonDown(state, x, y);
	return 0;
}

int XGLView::ControllerGL::mButtonUp(WPARAM state, int x, int y)
{
	this->eventQueue->mButtonUp(state, x, y);
	return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle reft mouse down
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::rButtonDown(WPARAM state, int x, int y)
{
	this->eventQueue->rButtonDown(state, x, y);
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle reft mouse up
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::rButtonUp(WPARAM state, int x, int y)
{
	this->eventQueue->rButtonUp(state, x, y);
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_MOUSEMOVE
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::mouseMove(WPARAM state, int x, int y)
{
	this->eventQueue->mouseMove(state, x, y);
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_MOUSEWHEEL
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::mouseWheel(int state, int delta, int x, int y)
{
	this->eventQueue->mouseWheel(state, delta,x, y);
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_SIZE
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::size(WPARAM wParam, int w, int h)
{
	this->eventQueue->windowSize(0,0,w,h);

	int viewport[]{ 0,0,w,h };
	model->setViewport(viewport);
    log("Changed OpenGL rendering window size: %dx%d.", w, h);
    return 0;
}
