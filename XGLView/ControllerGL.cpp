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
#include "XGL\Log.h"
#include "XGL\ViewGL.h"
#include "XGLModel\ModelGL.h"
#include "XGL\TrackballCamera.h"
#include "ControllerGL.h"


using namespace XGL;
using namespace XGLModel;
using namespace XGLView;




///////////////////////////////////////////////////////////////////////////////
// default contructor
///////////////////////////////////////////////////////////////////////////////
ControllerGL::ControllerGL() : model(nullptr), view(nullptr), camera(nullptr),
loopFlag(false),
dirty(false)
{
}



int XGLView::ControllerGL::setup(XGLModel::ModelGL * model, XGL::ViewGL * view, XGL::Camera * camera)
{
	this->model = model;
	this->view = view;
	this->camera = camera;
	return 1;
}

int XGLView::ControllerGL::render()
{
	// create a thread for OpenGL rendering
	glThread = std::thread(&ControllerGL::runThread, this);
	dirty = true;
	loopFlag = true;
	log("Created a rendering thread for OpenGL.");
	return 1;

}

///////////////////////////////////////////////////////////////////////////////
// handle WM_CLOSE
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::close()
{
    // wait for rendering thread is terminated
    loopFlag = false;
	dirty = false;
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
	dirty = true;
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
		if (dirty)
		{
			//std::this_thread::yield();      // yield to other processes or threads
			//std::this_thread::sleep_for(std::chrono::milliseconds(1));  // yield to other processes or threads
			model->draw();
			view->swapBuffers();
			dirty = false;
		}
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
	this->camera->lButtonDown(state, x, y);
	this->model->setViewMatrix(this->camera->getViewMatrix());
	dirty = true;
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle Left mouse up
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::lButtonUp(WPARAM state, int x, int y)
{
	this->camera->lButtonUp(state, x, y);
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle reft mouse down
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::rButtonDown(WPARAM state, int x, int y)
{
	this->camera->rButtonDown(state, x, y);
	this->model->setViewMatrix(this->camera->getViewMatrix());
	dirty = true;
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle reft mouse up
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::rButtonUp(WPARAM state, int x, int y)
{
	this->camera->rButtonUp(state, x, y);
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_MOUSEMOVE
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::mouseMove(WPARAM state, int x, int y)
{
	this->camera->mouseMove(state, x, y);
	this->model->setViewMatrix(this->camera->getViewMatrix());
	dirty = true;
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_MOUSEWHEEL
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::mouseWheel(int state, int delta, int x, int y)
{
	this->camera->mouseWheel(state, delta,x, y);
	this->model->setViewMatrix(this->camera->getViewMatrix());
	dirty = true;
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_SIZE
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::size(WPARAM wParam, int w, int h)
{
	int viewport[]{ 0,0,w,h };
	model->setViewport(viewport);
    log("Changed OpenGL rendering window size: %dx%d.", w, h);
    return 0;
}
