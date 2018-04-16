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


#include <vector>
#include <string>
#include <GL\glew.h>
#include "XGL\XGL.h"
#include "XGL\Matrices.h"
#include "XGL\Vectors.h"
#include "XGL\Quaternion.h"
#include "ObjModel.h"
#include "XGL\BoundingBox.h"
#include "BitmapFont.h"
#include "XGL\OrbitCamera.h"
#include "Vertices.h"
#include "XGLModel.h"

namespace XGLModel
{

	 class XGLMODEL_API ModelGL
	{
	public:
		ModelGL();
		~ModelGL();

		virtual void init();                            // initialize OpenGL states
		inline void quit() {}                            // clean up OpenGL objects
		virtual void draw();
		void setViewport(int viewport[4]);
		void setProject(const Matrix4& projectMatrix);
		void setViewMatrix(const Matrix4& viewMatrix) {  cameraMatrix = viewMatrix; }
		void setMousePath(const std::vector<Vector3>& path) { this->sphere_Path = path; }
		void setMouseVector(const Vector3& v) { this->sphere_Vector = v; }
	protected:
		void draw3D();
		void preFrame();
		void postFrame();
		void drawAxis(float size);
		void initGL();
		void initLights();
	protected:
		Matrix4 cameraMatrix;
		Matrix4 projectMatrix;
		int viewport[4];
		float sphere_Radius;
		XGL::Vector3 sphere_Vector;
		std::vector<XGL::Vector3> sphere_Path;
	};
}

#endif
