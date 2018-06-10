///////////////////////////////////////////////////////////////////////////////
// ModelGL.h
// =========
// Model component of OpenGL
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2016-05-29
// UPDATED: 2016-07-21
///////////////////////////////////////////////////////////////////////////////

#ifndef MODEL_GL_H
#define MODEL_GL_H

#include <functional>
#include <vector>
#include <string>
#include <GL\glew.h>
#include "XGL\XGL.h"
#include "xgl\Vec3f"
#include "xgl\Matrix"
#include "xgl\Quat"
#include "xgl\XCamera.h"

#include "ObjModel.h"
#include "XGL\BoundingBox.h"
#include "BitmapFont.h"
#include "Vertices.h"
#include "XGLModel.h"

#ifndef WM_XGL_MODEL
#define WM_XGL_MODEL WM_USER+1
#endif // !WM_XGL_MODEL


namespace XGLModel
{
	 class XGLMODEL_API ModelGL
	{
	public:
		ModelGL();
		~ModelGL();

		void init();                            // initialize OpenGL states
		inline void quit() {}                            // clean up OpenGL objects
		virtual void draw();

		void setViewport(int viewport[4]);
		int* getViewport() { return viewport; }
		XCamera* getCamera() { return camera; }
		virtual void project(float left, float right, float bottom, float top, float near, float) ;
		void setMousePath(const std::vector<Vector3>& path) { this->sphere_Path = path; }
		void setMouseVector(const Vector3& v) { this->sphere_Vector = v; }

		virtual void postViewMsg(int id, std::string& msg) 
		{
			PostMessage(handle, WM_XGL_MODEL, WPARAM(id), (LPARAM)msg.c_str());
		}
		void bindHandle(HWND handle) { this->handle = handle; }
	protected:
		void draw3D();
		void preFrame();
		void postFrame();
		void drawAxis(float size);
		virtual void initGL();
		virtual void initCamera();
		virtual void initProject();
		virtual void initShader();
		void initLights();
	protected:

		Matrixf projectMatrix;
		int windowWith;
		int windowHeight;
		int viewport[4];
		float sphere_Radius;
		XGL::Vector3 sphere_Vector;
		std::vector<XGL::Vector3> sphere_Path;
		XGL::XCamera* camera;
	private:
		HWND handle;
	};
}

#endif
