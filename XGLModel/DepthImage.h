#pragma once

#include "TutorialFactory.h"
#include "IXMesh.h"

namespace XGLModel {
	class DepthImage :public TutorialInterface
	{
	public:
		DepthImage();
		~DepthImage();

		void loadModel();
		void BindTexture(GLenum target, GLuint texUnit, GLint texObj);
	public:
		inline void setCamera(const XGL::Matrixf& camera) { this->camera->setByInverseMatrix(camera); }
		inline void setModel(const XGL::Matrixf& model) { this->model = model; }
		void render();
	private:
		XGL::Matrixf model;
		GLuint g_mv;
		GLuint g_pers;
		GLuint g_sampler2d;
		IXMesh m_quad;

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;
	};

}

