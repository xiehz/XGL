#pragma once

#include "TutorialFactory.h"

namespace XGLModel {

	class XVAOMesh;
	class SimpleVAO : public TutorialInterface
	{
	public:
		SimpleVAO();
		~SimpleVAO();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;


		GLuint g_mv;
		GLuint g_pers;
		GLuint g_sampler2d;
		XVAOMesh* m_Mesh;
		XVAOMesh* m_Mesh1;
	};


}

