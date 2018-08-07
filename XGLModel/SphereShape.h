#pragma once

#include "TutorialFactory.h"
#include "IXMesh.h"

namespace XGLModel {

	class SphereShape : public TutorialInterface
	{
	public:
		SphereShape();
		~SphereShape();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;
	public:
		void loadModel();
		void setCamera(const Matrixf& m );
		void setModel(const Matrixf& m);
		void render();
		void setupPers();
	public:
		GLuint g_mv;
		GLuint g_pers;
		GLuint g_sampler2d;
		IXMesh m_sphere;
		Matrixf model;
	};

}

