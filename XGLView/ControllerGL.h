///////////////////////////////////////////////////////////////////////////////
// ControllerGL.h
// ==============
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

#ifndef WIN_CONTROLLER_GL_H
#define WIN_CONTROLLER_GL_H

#include <thread>
#include "XGLInterface\Controller.h"

namespace XGL {
	class ViewGL;
	class Camera;
}

namespace XGLModel {
	class ModelGL;
}

namespace XGLView
{
    class ControllerGL : public XGLInterface::Controller
    {
    public:
        ControllerGL();
        ~ControllerGL() {};

		int setup(XGLModel::ModelGL* model, XGL::ViewGL* view, XGL::Camera* camera);
		virtual int render();
       virtual int close();                                // close the RC and OpenGL window
	   virtual  int command(int id, int cmd, LPARAM msg);   // for WM_COMMAND
       virtual  int create();                               // create RC for OpenGL window and start new thread for rendering
       virtual  int paint();
       virtual  int lButtonDown(WPARAM state, int x, int y);
       virtual  int lButtonUp(WPARAM state, int x, int y);
       virtual  int rButtonDown(WPARAM state, int x, int y);
       virtual  int rButtonUp(WPARAM state, int x, int y);
       virtual  int mouseMove(WPARAM state, int x, int y);
       virtual  int mouseWheel(int state, int delta, int x, int y); // for WM_MOUSEWHEEL:state, delta, x, y
       virtual  int size(WPARAM wParam,int w, int h);      // for WM_SIZE: width, height, type(SIZE_MAXIMIZED...)


    private:
        void runThread();                           // thread for OpenGL rendering

        XGLModel::ModelGL* model;                             // pointer to model component
        XGL::ViewGL* view;                               // pointer to view component
		XGL::Camera* camera;
        std::thread glThread;                       // opengl rendering thread object
        volatile bool loopFlag;                     // rendering loop flag
		volatile bool dirty;
    };
}

#endif
